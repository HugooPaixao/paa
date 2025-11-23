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
} Pacotes;

int max(int a, int b) {
  if(a > b)
    return a;

  return b;
}

/* basicamente é isso aqui, só pensar em como que vai retornar os itens que maximizam,
  até entao só esta retornando o valor que maximiza e depois pensar em como isso vai
  ser lido e escrito mantendo a eficiencia*/

int mochila(int qtdItens, int capacidade, int valor[], int peso[]) {
  int m[qtdItens+1][capacidade+1];

  for(int i = 0; i <= qtdItens; i++) {
    for(int j = 0; j <= capacidade; j++) {
      if(i == 0 || j == 0)
        m[i][j] = 0;
      else if(peso[i-1] <= j)
        m[i][j] = max(valor[i-1]+ m[i-1][j-peso[i-1]], m[i-1][j]);
      else
        m[i][j] = m[i-1][j];
    }
  }
  return m[qtdItens][capacidade];
}


int main() {
  int valor[] = {12,10,20,15};
  int peso[] = {2,1,3,2};
  int capacidade = 5;
  int qtdItens = sizeof(valor)/sizeof(valor[0]);

  printf("\nValor que maximiza: %d\n\n", mochila(qtdItens, capacidade, valor, peso)); // ta retornando o esperado igual na tabela do slde
  return 0;
}