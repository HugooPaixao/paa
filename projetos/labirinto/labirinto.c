#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct casa{
  int casa1;
  int casa2;
} Casa;

// o terceiro parâmetro servirá pra ver de contém ou nao um 'X'
Casa processarLabirinto(int altura, int largura, char tabuleiro[altura][largura], Casa qtdCasas, FILE* input) {
  for(int i = 0; i < altura; i++) {
    for(int j = 0; j < largura; j++) {
      fscanf(input, " %c", &tabuleiro[i][j]);
        if(tabuleiro[i][j] == 'X') {
          qtdCasas.casa1 = i;
          qtdCasas.casa2 = j;
        }
    }
  }
  return qtdCasas;
}

int bin;

int detectarEspaco(int altura, int largura, char tabuleiro[altura][largura], int x, int y) {
  if(x >= 0 && x < altura && y >= 0 && y < largura && (tabuleiro[x][y] == '0'))
    return 1;

  return 0;
}

int resolverLabirinto(int altura, int largura, char tabuleiro[altura][largura], int x, int y, FILE* output) {
  if(tabuleiro[x][y] == '0' && (x == 0 || y == 0 || x == altura-1 || y == largura-1)) {
    fprintf(output, "FIM@%d,%d\n", x, y);
    bin = 1;
    return 1;
  }

  tabuleiro[x][y] = 'T'; // um marcador

  if(detectarEspaco(altura, largura, tabuleiro, x, y+1) == 1) {
    fprintf(output, "D->%d,%d|", x, y+1);
    if(resolverLabirinto(altura, largura, tabuleiro, x, y+1, output) == 1)
      return 1;
    fprintf(output, "BT@%d,%d->%d,%d|", x, y+1, x, y);
  }

  if(detectarEspaco(altura, largura, tabuleiro, x-1, y) == 1) {
    fprintf(output, "F->%d,%d|", x-1, y);
    if(resolverLabirinto(altura, largura, tabuleiro, x-1, y, output) == 1)
      return 1;
    fprintf(output, "BT@%d,%d->%d,%d|", x-1, y, x, y);
  }

  if(detectarEspaco(altura, largura, tabuleiro, x, y-1) == 1) {
    fprintf(output, "E->%d,%d|", x, y-1);
    if(resolverLabirinto(altura, largura, tabuleiro, x, y-1, output) == 1)
      return 1;
    fprintf(output, "BT@%d,%d->%d,%d|", x, y-1, x, y);
  }

  if(detectarEspaco(altura, largura, tabuleiro, x+1, y) == 1) {
    fprintf(output, "T->%d,%d|", x+1, y);
    if(resolverLabirinto(altura, largura, tabuleiro, x+1, y, output) == 1)
      return 1;
    fprintf(output, "BT@%d,%d->%d,%d|", x+1, y, x, y);
  }

  return 0;
}

int main(int argc, char* argv[]) {
  printf("#ARGS = %i\n", argc);
  printf("PROGRAMA = %s\n", argv[0]);
  printf("ARG1 = %s, ARG2 = %s\n", argv[1], argv[2]);

  FILE* input = fopen(argv[1], "r");
  FILE* output = fopen(argv[2], "w");

  int qtdLab, altura, largura, i;
  fscanf(input, "%d", &qtdLab);

  for(i = 0; i < qtdLab; i++) {
    fscanf(input, "%d %d", &largura, &altura);
    char tabuleiro[altura][largura];
    Casa casa;
    casa = processarLabirinto(altura, largura, tabuleiro, casa, input);
    fprintf(output, "L%d:INI@%d,%d|", i, casa.casa1, casa.casa2);
    resolverLabirinto(altura, largura, tabuleiro, casa.casa1, casa.casa2, output);
      if(bin == 0)
        fprintf(output, "FIM@-,-\n");
      bin = 0;
  } // ta com erro na escrita ou leitura, deve ser apenas isso

  fclose(input);
  fclose(output);

  return 0;
}