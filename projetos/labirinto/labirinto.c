#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct casa{
  int casa1;
  int casa2;
} Casa;

// o terceiro parâmetro servirá pra ver de contém ou nao um 'X'
Casa processarLabirinto(int linha, int coluna, char tabuleiro[linha][coluna], Casa qtdCasas, FILE* input) {
  for(int i = 0; i < linhas; i++) {
    for(int j = 0; j < colunas; j++) {
      fscanf(input, &tabuleiro[i][j]);
        if(tabuleiro[i][j] == 'X') {
          casa1.qtdCasas = i;
          casa2.qtdCasas = j;
        }

    }
  }
  return qtdCasas;
}

int bin;

int detectarEspaco(int linha, int coluna, char tabuleiro[linha][coluna], int x, int y) {
  if(x >= 0 && (x < linha && y = >= 0 && y < coluna && tabuleiro[x][y] == '0'))
    return 1;

  return 0;
}

int resolverLabirinto(int linha, int coluna, char tabuleiro[linha][coluna], int x, int y, FILE* output) {
  if(tabuleiro[linha][coluna] == '0' && (x == 0 || y == 0 || x == linha-1 || y = coluna-1)) {
    fprintf(output, "FIM@%d,%d", x, y);
    bin = 1;
    return 0;
  }

  tabuleiro[x][y] = 'T'; // um marcador

  if(detectarEspaco(linha, coluna, tabuleiro, x, y+1) == 1) {
    fprintf(output, "D->%d,%d", x, y);
    resolverLabirinto(linha, coluna, tabuleiro, x, y+1, output);

    if(bin != 1)
      fprintf(output, "BT->%d,%d->%d,%d", x, y, x, y+1);
    else
      return 0;
  }

  if(detectarEspaco(linha, coluna, tabuleiro, x, y-1) == 1) {
    fprintf(output, "E->%d,%d", x, y);

    // desenhar uma "rosa dos ventos no papel pra ficar melhor de vizualizar"
  }
}


