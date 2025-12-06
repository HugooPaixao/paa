#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct veiculo{
  char placa[8];
  float peso, volume;
} Veiculo;

typedef struct pac{
  char cod[14];
  float valor, peso, volume;
  int processado; // 1 = processado, 0 = nao processado
} Pacotes;

float max(float a, float b) {
  if(a > b)
    return a;

  return b;
}


float*** alocarMatriz(int qtdItens, float peso, float volume) {
  int w = (int)round(peso), v = (int)round(volume);

  float*** m = (float***)malloc((qtdItens+1)*sizeof(float**));
  for(int i = 0; i <= qtdItens; i++) {
    m[i] = (float**)malloc((w+1)*sizeof(float*));
    for(int j = 0; j <= w; j++) {
      m[i][j] = (float*)calloc(v+1, sizeof(float)); // prencher a primeira lihna e primeira coluna com 0s
    }
  }

  return m;
}

  /* basicamente é isso aqui, só pensar em como que vai retornar os itens que maximizam,
    até entao só esta retornando o valor que maximiza e depois pensar em como isso vai
    ser lido e escrito mantendo a eficiencia*/ //ok

void mochila(int qtdItens, float peso, float volume, Pacotes itens[], int sel[]) {

  float*** m = alocarMatriz(qtdItens, peso, volume);

  int w = (int)round(peso), v = (int)round(volume);

  for(int i = 1; i <= qtdItens; i++) {
    for(int j = 0; j <= w; j++) {
      for(int k = 0; k <= v; k++) {
        m[i][j][k] = m[i-1][j][k];

        int vol = (int)round(itens[i-1].volume);
        int p = (int)round(itens[i-1].peso);

        if((j >= p && k >= vol))
          m[i][j][k] = max(itens[i-1].valor+ m[i-1][j-p][k-vol], m[i-1][j][k]);
      }
    }
  }

  int aux = w, aux2 = v;
  for(int i = qtdItens; i > 0; i--) {
    if(m[i][aux][aux2] != m[i-1][aux][aux2]) {
      sel[i-1] = 1;
      aux -= (int)round(itens[i-1].peso);
      aux2 -= (int)round(itens[i-1].volume);
    }
    else
      sel[i-1] = 0;
  }

  for(int i = 0; i <= qtdItens; i++) {
    for(int j = 0; j <= w; j++) {
      free(m[i][j]);
    }
    free(m[i]);
  }
  free(m);
}

void processarVeiculos(int qtdVeiculos, Veiculo v[], FILE* input) {
  for(int i = 0; i < qtdVeiculos; i++)
    fscanf(input, "%s %f %f", v[i].placa, &v[i].peso, &v[i].volume);
}

void processarPacotes(int qtdPacotes, Pacotes p[], FILE* input) {
  for(int i = 0; i < qtdPacotes; i++) {
    fscanf(input, "%s %f %f %f", p[i].cod, &p[i].valor, &p[i].peso, &p[i].volume);
    p[i].processado = 0;
  }
}


// os pendentes nao estao sendo pegos, dps
void exibir(Veiculo v, Pacotes qtdItens[], int qtd, int sel[], Pacotes p[], int qtdPacotes, FILE* output) {
  float volAcumulado = 0, pesoAcumulado = 0, valAcumulado = 0;
  int itemSel = 0;

  for(int i = 0; i < qtd; i++) {
    if(sel[i]) {
      valAcumulado += qtdItens[i].valor;
      pesoAcumulado += qtdItens[i].peso;
      volAcumulado += qtdItens[i].volume;
      itemSel = 1;
    }
  }

  if(itemSel) {
    float porcentagemP = round((pesoAcumulado/v.peso)*100);
    float porcentagemV = round((volAcumulado/v.volume)*100);

   fprintf(output, "[%s]R$%.2f,%.0fKG(%.0f%%),%.0fL(%.0f%%)->" ,v.placa, valAcumulado, pesoAcumulado, porcentagemP, volAcumulado, porcentagemV);
    int virgula = 0; // sepra por virgula  ou n
    for(int i = 0; i < qtd; i++) {
      if(sel[i]) {
        if(virgula)
          fprintf(output, ",");

        fprintf(output, "%s", qtdItens[i].cod);
        virgula = 1;
      }
    }
    fprintf(output, "\n");
  }

}

void escreverArquivo(int qtdVeiculos, Veiculo v[], int qtdPacotes, Pacotes p[], FILE* output ) {

  for(int i = 0; i < qtdVeiculos; i++) { // pegando os veiculos
    int* sel = malloc(qtdPacotes*sizeof(int));
    int* itensMax = malloc(qtdPacotes*sizeof(int));
    Pacotes* qtdItens = malloc(qtdPacotes*sizeof(Pacotes));
    int qtd = 0;

    for(int j = 0; j < qtdPacotes; j++) { // pegando os pacotes pendentes(la ele)
      if(!p[j].processado) {
        qtdItens[qtd] = p[j];
        itensMax[qtd] = j;
        qtd++;
      }
    }

    if(qtd > 0) {
      mochila(qtd, v[i].peso, v[i].volume, qtdItens, sel);

      for(int k = 0; k < qtd; k++) {
        if(sel[k])
          p[itensMax[k]].processado = 1;
      }
      exibir(v[i], qtdItens, qtd, sel, p, qtdPacotes, output);
    }

    free(sel);
    free(itensMax);
    free(qtdItens);
  }

  for(int i = 0; i < qtdPacotes; i++) {
    if(!p[i].processado)
      fprintf(output, "PENDENTE:R$%.2f,%.0fKG,%.0fL->%s\n", p[i].valor, p[i].peso, p[i].volume,  p[i].cod);
  }
}

int main(int argc,char* argv[]) {

  printf("#ARGS = %i\n", argc);
  printf("PROGRAMA %s\n", argv[0]);
  printf("ARG1 = %s, ARG2 = %s\n", argv[1],argv[2]);

  FILE* input = fopen(argv[1], "r");
  FILE* output = fopen(argv[2], "w");

  int qtdVeiculos, qtdPacotes;

  fscanf(input, "%d", &qtdVeiculos);
  Veiculo* v = malloc(qtdVeiculos*sizeof(Veiculo));
  processarVeiculos(qtdVeiculos, v, input);

  fscanf(input, "%d", &qtdPacotes);
  Pacotes* p = malloc(qtdPacotes*sizeof(Pacotes));
  processarPacotes(qtdPacotes, p, input);

  escreverArquivo(qtdVeiculos, v, qtdPacotes, p, output);

  free(v);
  free(p);
  fclose(input);
  fclose(output);

  return 0;
}