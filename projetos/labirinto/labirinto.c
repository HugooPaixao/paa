#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct casa{
  int casa1;
  int casa2;
} Casa;

// o terceiro parâmetro servirá pra ver de contém ou nao um 'X'
Casa processarLabirinto(int linha, int coluna, char tabuleiro[linha][coluna], Casa qtdCasas, FILE* input) {
  for(int i = 0; i < linha; i++) {
    for(int j = 0; j < coluna; j++) {
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

int detectarEspaco(int linha, int coluna, char tabuleiro[linha][coluna], int x, int y) {
  if(x >= 0 && x < linha && y >= 0 && y < coluna && (tabuleiro[x][y] == '0'))
    return 1;

  return 0;
}

int resolverLabirinto(int linha, int coluna, char tabuleiro[linha][coluna], int x, int y, FILE* output) {
  if(tabuleiro[linha][coluna] == '0' && (x == 0 || y == 0 || x == linha-1 || y == coluna-1)) {
    fprintf(output, "FIM@%d,%d\n", x, y);
    bin = 1;
    return 0;
  }

  tabuleiro[x][y] = 'T'; // um marcador

  if(detectarEspaco(linha, coluna, tabuleiro, x, y+1) == 1) {
    fprintf(output, "D->%d,%d|", x, y);
    resolverLabirinto(linha, coluna, tabuleiro, x, y+1, output);

    if(bin != 1)
      fprintf(output, "BT@%d,%d->%d,%d|", x, y, x, y+1);
    else
      return 0;
  }

  if(detectarEspaco(linha, coluna, tabuleiro, x+1, y) == 1) {
    fprintf(output, "F->%d,%d|", x, y);
    resolverLabirinto(linha, coluna, tabuleiro, x+1, y, output);

    if(bin != 1)
      fprintf(output, "BT@%d,%d->%d,%d|", x, y, x+1, y);
    else
      return 0;
  }

  if(detectarEspaco(linha, coluna, tabuleiro, x, y-1) == 1) {
    fprintf(output, "E->%d,%d", x, y);
    resolverLabirinto(linha, coluna, tabuleiro, x, y-1, output);

    if(bin != 1)
      fprintf(output, "BT@%d,%d->%d,%d|", x, y, x, y-1);
    else
      return 0;
  }

  if(detectarEspaco(linha, coluna, tabuleiro, x-1, y) == 1) {
    fprintf(output, "T->%d,%d|", x, y);
    resolverLabirinto(linha, coluna, tabuleiro, x-1, y, output);

    if(bin != 1)
      fprintf(output, "BT@%d,%d->%d,%d|", x, y, x-1, y);
    else
      return 0;
  }
}


int main(int argc, char* argv[]) {
  printf("#ARGS = %i\n", argc);
  printf("PROGRAMA = %s\n", argv[0]);
  printf("ARG1 = %s, ARG2 = %s\n", argv[1], argv[2]);

  FILE* input = fopen(argv[1], "r");
  FILE* output = fopen(argv[2], "w");

  int qtdLab, linha, coluna, i;
  fscanf(input, "%d", &qtdLab);

  for(i = 0; i < qtdLab; i++) {
    fscanf(input, "%d", &coluna);
    char tabuleiro[linha][coluna];
    Casa casa;
    casa = processarLabirinto(linha, coluna, tabuleiro, casa, input);
    fprintf(output, "L%d:", i);
    fprintf(output, "INI@%d,%d|", casa.casa1, casa.casa2);
    resolverLabirinto(linha, coluna, tabuleiro, casa.casa1, casa.casa2, output);
      if(bin == 0)
        fprintf(output, "FIM@-,-\n");
      bin = 0;
  } // ta com erro na escrita ou leitura, deve ser apenas isso

  fclose(input);
  fclose(output);

  return 0;
}
