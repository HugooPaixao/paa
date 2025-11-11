#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pac{
  int tamanho;
  int prio;
  char *dados;
} Pacote;

void trocar(Pacote *x, Pacote *y) {
  Pacote aux = *x;
  *x = *y;
  *y = aux;
}


int pai(int i) {
  return (i-1)/2;
}

int esquerdo(int i) {
  return 2*i+1;
}

int direito(int i) {
  return 2*i+2;
}

void heapify(Pacote v[], int t, int i) {
  int p = i, e = esquerdo(i), d = direito(i);

  if(e < t && v[e].prio < v[p].prio)
    p = e;

  if(d < t && v[d].prio < v[p].prio)
    p = d;

  if(p != i) {
    trocar(&v[i], &v[p]);
    heapify(v, t, p);
  }
}

void construir_heap(Pacote v[], int n) {
  for(int i = (n/2) - 1; i >= 0; i--)
    heapify(v, n, i);
}

void heapsort(Pacote v[], int n) {
  construir_heap(v, n);

  for(int i = n-1; i > 0; i--) {
    trocar(&v[0], &v[i]);
    heapify(v, i, 0);
  }
}

void processarEntrada(int total, Pacote pacotes[], FILE *input) { // total de pacotes e seu tamanho maximo

    for(int n = 0; n < total; n++) {
        fscanf(input, "%d %d", &pacotes[n].prio, &pacotes[n].tamanho); // tamango do pacote e prioridade

        pacotes[n].dados = (char*) malloc(pacotes[n].tamanho*3); // x3 , byte + o '\0'

        char tam[3];
        for(int j = 0; j < pacotes[n].tamanho; j++) {
            fscanf(input, "%s", tam);
            strcat(pacotes[n].dados, tam);
            if(j < pacotes[n].tamanho-1)
              strcat(pacotes[n].dados, ",");
        }
    }
}

void exibir(int total, Pacote pacotes[], int tam_maximo, FILE *output) {

    int inicio = 0;

    while(inicio < total) {
        int contBytes = 0;
        int cont = 0;
        int capacidade[total];

        for(int i = inicio; i < total; i++) {
            if(contBytes+pacotes[i].tamanho <= tam_maximo) {
                capacidade[cont++] = i;
                contBytes += pacotes[i].tamanho;
            }
            else
              break;
        }

        if(cont == 0) break;

        Pacote pac[cont];
        for(int i = 0; i < cont; i++) {
            pac[i] = pacotes[capacidade[i]];
        }

        heapsort(pac, cont);

        // ver esse trecho aqui, ele esta aumentando o tempo
        for(int i = 0; i < cont; i++) {
            fprintf(output, "|%s", pac[i].dados);
        }
        fprintf(output, "|");

        inicio += cont;
        if(inicio < total)
            fprintf(output, "\n");
    }
}

int main(int argc, char *argv[]) {

    printf("#ARGS = %i\n", argc);
    printf("PROGRAMA = %s\n", argv[0]);
    printf("ARG1 = %s, ARG2 = %s\n", argv[1], argv[2]);


    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");

    int qtdPacotes, tam_maximo;

    fscanf(input, "%d %d", &qtdPacotes, &tam_maximo);
    Pacote *pacotes = (Pacote*) malloc(qtdPacotes*sizeof(Pacote));
    processarEntrada(qtdPacotes, pacotes, input);
    fclose(input);

    exibir(qtdPacotes, pacotes, tam_maximo, output);
    fclose(output);

    for(int i = 0; i < qtdPacotes; i++) {
        free(pacotes[i].dados);
    }
    free(pacotes);

    return 0;
}