  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>

  typedef struct veiculo{
    char placa[8];
    float peso, volume;
  } Veiculo;

  typedef struct pac{
    char cod[14];
    float valor, peso, volume;
    int processado;
  } Pacotes;

  float max(float a, float b) {
    if(a > b)
      return a;

    return b;
  }

  /* basicamente é isso aqui, só pensar em como que vai retornar os itens que maximizam,
    até entao só esta retornando o valor que maximiza e depois pensar em como isso vai
    ser lido e escrito mantendo a eficiencia*/ //ok

  void mochila(int qtdItens, float peso, float volume, Pacotes itens[], int itensMax[]) {
    int w = (int)(peso*100);
    int v = (int)(volume*100);

    float*** m = (float***)malloc(((qtdItens+1)*sizeof(float**)));
    for(int i = 0; i <= qtdItens; i++) {

      m[i] = (float**)malloc(((w+1)*sizeof(float)));
      for(int j = 0; j <= w; j++) {
          m[i][j] = (float*)calloc(v+1, sizeof(float));
        }
      }

    for(int i = 1; i <= qtdItens; i++) {
      for(int j = 0; j <= w; j++) {
        for(int k = 0; k <= v; k++) {
          m[i][j][k] = m[i-1][j][k];

          if(j >= ((int)(itens[i-1].peso*100)) && k >= ((int)(itens[i-1].volume))) {
            int vol = (int)(itens[i-1].valor);
            int p = (int)(itens[i-1].peso);
            m[i-1][j][k] = max( itens[i-1].volume+ m[i-1][j-p][k-vol], m[i-1][j][k]);
          }
        }
      }
    }

    int aux = w, aux2 = v;
    for(int i = qtdItens; i > 0; i--) {
      if(m[i][aux][aux2] != m[i-1][aux][aux2]) {
        itensMax[i-1] = 1;
        aux -= (int)(itens[i-1].peso*100);
        aux2 -= (int)(itens[i-1].volume*100);
      }
      else
        itensMax[i-1] = 0;
    }

    for(int i = 0; i <= qtdItens; i++) {
      for(int j = 0; j <= w; j++) {
        free(m[i][j]);
      }
      free(m[i]);
    }
    free(m);
  }

  void processarEntradaV(int qtdVeiculos, Veiculo v[], FILE* input) {
    for(int i = 0; i < qtdVeiculos; i++)
      fscanf(input, "%s %f %f", v[i].placa, &v[i].peso, &v[i].volume);

  }

  void processarEntradaP(int qtdPacotes, Pacotes p[], FILE* input) {
    for(int i = 0; i < qtdPacotes; i++)
      fscanf(input, "%s %f %f %f", p[i].cod, &p[i].valor, &p[i].peso, &p[i].volume);

  }

  void exibir(Veiculo v, Pacotes p[], int qtd, int itensMax[], FILE* output) {
    float volAcumulado = 0, pesoAcumulado = 0, valAcumulado = 0;
    int sel = 0;

    for(int i = 0; i < qtd; i++) {
      if(itensMax[i]) {
        valAcumulado += p[i].valor;
        pesoAcumulado += p[i].peso;
        volAcumulado += p[i].volume;
        sel = 1;
      }
    }

    if(sel) {
      float porcentagemP = (pesoAcumulado/v.peso)*100;
      float porcentagemV = (volAcumulado/v.volume)*100;

      fprintf(output, "[%s]R$%.2f,%.0fKG(%.0f%%),%.0fL(%.0f%%)->" ,v.placa, valAcumulado, v.peso, porcentagemP,v.volume, porcentagemV);

      // talvez utilizar um merge sort aqui, se nao bater o tempo
      int virgula = 1;
      for(int i = 0; i < qtd; i++) {
        if(itensMax[i]) {
          if(!virgula)
            fprintf(output, ",");

          fprintf(output, "%s", p[i].cod);
        }
      }
      fprintf(output, "\n");
    }
    else {
      for(int i = 0; i < qtd; i++) {
        if(!p[i].processado)
          fprintf(output, "PENDENTE:R$%.2f,%.0fKG,%.0fL->%s\n", p[i].valor, p[i].peso, p[i].volume, p[i].cod);
      }
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
    Veiculo *v = malloc(qtdVeiculos*sizeof(Veiculo));
    processarEntradaV(qtdVeiculos, v, input);

    fscanf(input, "%d", &qtdPacotes);
    Pacotes *p = malloc(qtdPacotes*sizeof(Pacotes));
    processarEntradaP(qtdPacotes, p, input);

    for(int i = 0; i < qtdVeiculos; i++) {
      int *sel = malloc(qtdPacotes*sizeof(int)), qtd = 0;
      int *itensMax = malloc(qtdPacotes*sizeof(int));
      Pacotes *qtdItens = malloc(qtdPacotes*sizeof(Pacotes));

      for(int j = 0; j < qtdPacotes; j++) {
        if(!p[j].processado) {
          qtdItens[qtd] = p[j];
          itensMax[qtd] = j;
          qtd++;
        }
      }

      mochila(qtd, v[i].peso, v[i].volume, qtdItens, sel);

      for(int i = 0; i < qtd; i++) {
        if(sel[i])
          p[itensMax[i]].processado = 1;
      }

      exibir(v[i], p, qtdPacotes, sel, output);


      free(sel);
      free(itensMax);
      free(qtdItens);
    }

    free(v);
    free(p);
    return 0;
  }