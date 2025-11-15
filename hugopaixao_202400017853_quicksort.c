#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int contHP = 0;
int contLP = 0;
int contHA = 0;
int contLA = 0;
int contHM = 0;
int contLM = 0;
int modo;

typedef struct tipo{
  char tipo[3];
  int valor;
} Tipo;

void trocar(int *v, int *w) {
  int temp = *v;
  *v = *w;
  *w = temp;
  if(modo == 1) contHP++;
  else if(modo == 2) contLP++;
  else if(modo == 3) contHA++;
  else if(modo == 4) contLA++;
  else if(modo == 5) contHM++;
  else if(modo == 6) contLM++;
}

int hoare(int *v, int i, int j) {
  int p = v[i], x = i-1, y = j+1;

  while(1) {
    while(v[++x] < p);
    while(v[--y] > p);

    if(x < y) {;
      trocar(&v[x], &v[y]);
    }
    else
      return y;
  }
}

void quickSortHP(int *v, int i, int j) {
  modo = 1;
  contHP++;
  if(i < j) {
    int p = hoare(v, i, j);
    quickSortHP(v, i, p);
    quickSortHP(v, p+1, j);
  }
}

int lomuto(int *v, int i, int j) {
  int p = v[j], x = i-1, y = i;

  for(y = i; y < j; y++) {
    if(v[y] <= p) {
      trocar(&v[++x], &v[y]);
    }
  }
  trocar(&v[++x], &v[j]);
    return x;
}

void quickSortLP(int *v, int i, int j) {
  modo = 2;
  contLP++;
  if(i < j) {
    int p = lomuto(v, i, j);
    quickSortLP(v, i, p-1);
    quickSortLP(v, p+1, j);
  }
}

int hoareA(int *v, int i, int j) {
  int n = (j-i+1);
  trocar(&v[i], &v[i+ (abs(v[i]) % n)]);

  return hoare(v, i, j);
}

void quickSortHA(int *v, int i, int j) {
  modo = 3;
  contHA++;
  if(i < j) {
    int p = hoareA(v, i, j);
    quickSortHA(v, i, p);
    quickSortHA(v, p+1, j);
  }
}

int lomutoA(int *v, int i, int j) { // Aleatório: Va = V [ini + |V [ini]| mod n]
  int n = (j-i+1);
  trocar(&v[j], &v[i+ (abs(v[i]) % n)]);

  return lomuto(v, i, j);
}

void quickSortLA(int *v, int i, int j) {
  modo = 4;
  contLA++;
  if(i < j) {
    int p = lomutoA(v, i, j);
    quickSortLA(v, i, p-1);
    quickSortLA(v, p+1, j);
  }
}

int mediana3(int *v, int i, int j) {
  int n = (j-i)+1;
  int i1 = i+(n/4);
  int i2 = i+(n/2);
  int i3 = i+((3*n)/4);

  int v1 =  v[i1];
  int v2 =  v[i2];
  int v3 =  v[i3];

  if((v1 > v2 && v1 < v3) || (v1 < v2 && v1 > v3)) return i1;
  else if((v2 > v1 && v2 < v3) || (v2 < v1 && v2 > v3)) return i2;
  else return i3;
}

int hoareM(int *v, int i, int j) {
  trocar(&v[i], &v[mediana3(v, i, j)]);

  return hoare(v, i, j);
}

void quickSortHM(int *v, int i, int j) {
  modo = 5;
  contHM++;
  if(i < j) {
    int p = hoareM(v, i, j);
    quickSortHM(v, i, p);
    quickSortHM(v, p+1, j);
  }
}

int lomutoM(int *v, int i, int j) {
  trocar(&v[j], &v[mediana3(v, i, j)]);

  return lomuto(v, i, j);
}

void quickSortLM(int *v, int i, int j) {
  modo = 6;
  contLM++;
  if(i < j) {
    int p = lomutoM(v, i, j);
    quickSortLM(v, i, p-1);
    quickSortLM(v, p+1, j);
  }
}

void copiar(int *v, int *w, int n) {
  for(int i = 0; i < n; i++) v[i] = w[i];
}
void copiarTipo(Tipo *v, Tipo *w, int n) {
  for(int i = 0; i < n; i++) v[i] = w[i];
}

void intercalar(Tipo *s, Tipo *e, int i, int m, int j) {
  int i1= i, i2 = m+1, k = i;

  while(i1 <= m && i2 <= j) {
    if(e[i1].valor <= e[i2].valor) s[k++] = e[i1++];
    else s[k++] =  e[i2++];
  }

  if(i1 > m) copiarTipo(&s[k], &e[i2], j-i2+1);
  else copiarTipo(&s[k], &e[i1], m-i1+1);

  copiarTipo(&e[i], &s[i], j-i+1);
}

void mergeSort(Tipo *s, Tipo *e, int i, int j) {
  if(i < j) {
    int m = i+ (j-i)/2;
    mergeSort(s, e, i, m);
    mergeSort(s, e, m+1, j);

    intercalar(s, e, i, m, j);
  }
}

void processarSaida(int *v, int tam, Tipo *qtdOperacoes) {
  int *w = (int*) malloc(tam*sizeof(int));

  copiar(w, v, tam);
  contLP = 0;
  quickSortLP(w, 0, tam-1);
  strcpy(qtdOperacoes[0].tipo, "LP");
  qtdOperacoes[0].valor = contLP;

  copiar(w, v, tam);
  contLM = 0;
  quickSortLM(w, 0, tam-1);
  strcpy(qtdOperacoes[1].tipo, "LM");
  qtdOperacoes[1].valor = contLM;

  copiar(w, v, tam);
  contLA = 0;
  quickSortLA(w, 0, tam-1);
  strcpy(qtdOperacoes[2].tipo, "LA");
  qtdOperacoes[2].valor = contLA;

  copiar(w, v, tam);
  contHP = 0;
  quickSortHP(w, 0, tam-1);
  strcpy(qtdOperacoes[3].tipo, "HP");
  qtdOperacoes[3].valor = contHP;

  copiar(w, v, tam);
  contHM = 0;
  quickSortHM(w, 0, tam-1);
  strcpy(qtdOperacoes[4].tipo, "HM");
  qtdOperacoes[4].valor = contHM;

  copiar(w, v, tam);
  contHA = 0;
  quickSortHA(w, 0, tam-1);
  strcpy(qtdOperacoes[5].tipo, "HA");
  qtdOperacoes[5].valor = contHA;

  free(w);
}

int main(int argc, char *argv[]) {
  printf("#ARGS = %i\n", argc);
  printf("PROGRAMA = %s\n", argv[0]);
  printf("ARG1 = %s, ARG2 = %s\n", argv[1], argv[2]);

  FILE *input = fopen(argv[1], "r");
  FILE *output = fopen(argv[2], "w");

  int nVetores;
  fscanf(input,"%d", &nVetores);

  // se der timeout, e por causa dessa parte
  for(int i = 0; i < nVetores; i++) {
    int tam;
    fscanf(input, "%d", &tam);

    int *v = (int*) malloc(tam*sizeof(int));

    for(int j = 0; j < tam; j++)
      fscanf(input, "%d", &v[j]);

    Tipo qtdVariacoes[6];
    processarSaida(v, tam, qtdVariacoes);

    Tipo ordenar[6];
    mergeSort(ordenar, qtdVariacoes, 0, 5);

    fprintf(output ,"[%d]:", tam);

    for(int i = 0; i < 6; i++) {
      fprintf(output ,"%s(%d)", qtdVariacoes[i].tipo, qtdVariacoes[i].valor);
      if(i < 5)
        fprintf(output, ",");
    }

    if(i < nVetores-1) fprintf(output, "\n");


    free(v);
   }

  fclose(input);
  fclose(output);

  return 0;
}