#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct no{
  int simb;
  unsigned int freq;
  struct no* esq;
  struct no* dir;
} No;

typedef struct heap{
  unsigned tam, capacidade;
  No** v;
} Heap;

typedef struct h{
  int simb;
  int freq;
  int* bits;
  int n;
} Histograma;

typedef struct s{
  int simb;
  int* bits;
} Simbolos;

Heap* criarHeap(unsigned capacidade) {
  Heap* h = (Heap*)malloc(sizeof(Heap));
  h->tam = 0;
  h->capacidade = capacidade;
  h->v = (No**)malloc(h->capacidade * sizeof(No*));
  return h;
}

void trocar(No** a, No** b) {
  No* temp = *a;
  *a = *b;
  *b = temp;
}

void heapfy(Heap* h, int i) {
  int pai = i;
  int esq = 2 * i + 1;
  int dir = 2 * i + 2;

  if (esq < h->tam && h->v[esq]->freq < h->v[pai]->freq)
    pai = esq;
  if (dir < h->tam && h->v[dir]->freq < h->v[pai]->freq)
    pai = dir;
  if (pai != i) {
    trocar(&h->v[pai], &h->v[i]);
    heapfy(h, pai);
  }
}

void inserir(Heap* h, int i) {
  while (i > 0) {
    int pai = (i - 1) / 2;
    if (h->v[i]->freq < h->v[pai]->freq) {
      trocar(&h->v[i], &h->v[pai]);
      i = pai;
    } else {
      break;
    }
  }
}

No* extrair(Heap* h) {
  No* menor = h->v[0];
  h->v[0] = h->v[h->tam - 1];
  h->tam--;
  heapfy(h, 0);
  return menor;
}

void construirHeap(Heap* h) {
  int n = h->tam;
  for (int i = (n / 2) - 1; i >= 0; i--)
    heapfy(h, i);
}

void pegarBit(int bits[], int n, Histograma h[], int pos) {
  h[pos].bits = malloc(n * sizeof(int));
  h[pos].n = n;
  for (int i = 0; i < n; i++) {
    h[pos].bits[i] = bits[i];
  }
}

void inserirPrimeiroI(Heap* h, unsigned int freq, int simb) {
  No* novo = (No*)malloc(sizeof(No));
  novo->esq = NULL;
  novo->dir = NULL;
  novo->simb = simb;
  novo->freq = freq;
  h->v[h->tam] = novo;
  h->tam++;
}

void inserirPrimeiroC(Heap* h, unsigned int freq, No* esq, No* dir) {
  No* novo = (No*)malloc(sizeof(No));
  novo->esq = esq;
  novo->dir = dir;
  novo->simb = '\0';
  novo->freq = freq;
  h->v[h->tam] = novo;
  inserir(h, h->tam);
  h->tam++;
}

No* arvoreHuf(int simb[], int freq[], int tam) {
  Heap* h = criarHeap(tam);

  for (int i = 0; i < tam; i++)
    inserirPrimeiroI(h, freq[i], simb[i]);

  construirHeap(h);

  while (h->tam != 1) {
    No* x = extrair(h);
    No* y = extrair(h);
    inserirPrimeiroC(h, x->freq + y->freq, x, y);
  }
  return extrair(h);
}

void pegarCod(No* raiz, int cod[], int t, Histograma h[]) {
  if (raiz->esq) {
    cod[t] = 0;
    pegarCod(raiz->esq, cod, t + 1, h);
  }

  if (raiz->dir) {
    cod[t] = 1;
    pegarCod(raiz->dir, cod, t + 1, h);
  }

  if (raiz->esq == NULL && raiz->dir == NULL)
    pegarBit(cod, t, h, raiz->simb);
}

void huf(int simb[], int freq[], int tam, Histograma h[]) {
  No* raiz = arvoreHuf(simb, freq, tam);
  int cod[10000], t = 0;
  pegarCod(raiz, cod, t, h);
}

int hexaDec(char hex[]) {
  int dec = 0;
  int i = 0;

  while (hex[i] != '\0') {
    char c = hex[i];
    int val;

    if (c >= '0' && c <= '9') val = c - '0';
    else if (c >= 'A' && c <= 'F') val = c - 'A' + 10;
    else if (c >= 'a' && c <= 'f') val = c - 'a' + 10;
    else {
      i++;
      continue;
    }

    dec = dec * 16 + val;
    i++;
  }

  return dec;
}

char* decBin(int n, int m) {
  char* p = (char*)malloc(m + 2);
  int cont = 0;

  for (int j = m; j >= 0; j--) {
    int d = n >> j;
    if (d & 1)
      p[cont++] = '1';
    else
      p[cont++] = '0';
  }

  p[cont] = '\0';
  return p;
}

void exibirBinHexa(long int bin, FILE* output) {
  long int hex = 0, i = 1, resto;

  while (bin != 0) {
    resto = bin % 10;
    hex += resto * i;
    i *= 2;
    bin /= 10;
  }

  fprintf(output, "%.2lX", hex);
}

void exibirHexBin(int byte, FILE* output) {
  const char* hex = "0123456789ABCDEF";
  fputc(hex[(byte >> 4) & 0xF], output);
  fputc(hex[byte & 0xF], output);
}

void converteRLE(char freq[], char simb[], FILE* output) {
  int n = atoi(freq);
  exibirBinHexa(n, output);
  n = atoi(simb);
  exibirBinHexa(n, output);
}

void RLE(int tam, Simbolos simbs[], FILE* output) {
  int i = 0;

  while (i < tam) {
    int cont = 1;
    while (i + cont < tam && simbs[i].simb == simbs[i + cont].simb && cont < 255)
      cont++;
    char freq[9], simbString[9];
    strcpy(freq, decBin(cont, 7));
    strcpy(simbString, decBin(simbs[i].simb, 7));
    converteRLE(freq, simbString, output);
    i += cont;
  }
}

void lerSimbolos(int tam, Simbolos simbs[], Histograma h[], FILE* input) {
  for (int i = 0; i < tam; i++) {
    char hex[5];
    fscanf(input, "%s", hex);
    int n = hexaDec(hex);
    simbs[i].simb = n;
    h[n].simb = n;
    h[n].freq++;
  }
}

void inicializarHist(Histograma h[]) {
  for (int i = 0; i < 256; i++) {
    h[i].simb = i;
    h[i].freq = 0;
    h[i].bits = NULL;
    h[i].n = 0;
  }
}

void inicializarSimbs(Simbolos simbs[], int tam) {
  for (int i = 0; i < tam; i++) {
    simbs[i].simb = 0;
    simbs[i].bits = NULL;
  }
}

void exibir(int m, float taxaHuf, float taxaRLE, int tam, Simbolos simbs[],
            Histograma h[], int bytesHuf, int bytesRLE, FILE* output) {

  if (bytesHuf < bytesRLE) {
    fprintf(output, "%d->HUF(%.2f%%)=", m, taxaHuf);
    int byteAtual = 0, bitsByte = 0;

    for (int j = 0; j < tam; j++) {
      int s = simbs[j].simb;
      for (int k = 0; k < h[s].n; k++) {
        byteAtual = (byteAtual << 1) | h[s].bits[k];
        bitsByte++;
        if (bitsByte == 8) {
          exibirHexBin(byteAtual, output);
          byteAtual = 0;
          bitsByte = 0;
        }
      }
    }

    if (bitsByte > 0) {
      while (bitsByte < 8) {
        byteAtual <<= 1;
        bitsByte++;
      }
      exibirHexBin(byteAtual, output);
    }
    fprintf(output, "\n");
  } else {
    if (bytesHuf <= bytesRLE) {
      fprintf(output, "%d->HUF(%.2f%%)=", m, taxaHuf);
      int byteAtual = 0, bitsByte = 0;

      for (int j = 0; j < tam; j++) {
        int s = simbs[j].simb;
        for (int k = 0; k < h[s].n; k++) {
          byteAtual = (byteAtual << 1) | h[s].bits[k];
          bitsByte++;
          if (bitsByte == 8) {
            exibirHexBin(byteAtual, output);
            byteAtual = 0;
            bitsByte = 0;
          }
        }
      }

      if (bitsByte > 0) {
        while (bitsByte < 8) {
          byteAtual <<= 1;
          bitsByte++;
        }
        exibirHexBin(byteAtual, output);
      }
      fprintf(output, "\n");
    }

    if (bytesRLE <= bytesHuf) {
      fprintf(output, "%d->RLE(%.2f%%)=", m, taxaRLE);
      RLE(tam, simbs, output);
      fprintf(output, "\n");
    }
  }
}

void processarEntrada(FILE* input, FILE* output) {
  int qtd_seq;
  fscanf(input, "%d", &qtd_seq);

  for (int m = 0; m < qtd_seq; m++) {
    int tam;
    fscanf(input, "%d", &tam);

    Histograma h[256];
    inicializarHist(h);

    Simbolos simbs[tam];
    inicializarSimbs(simbs, tam);

    lerSimbolos(tam, simbs, h, input);

    int listaSimbs[256], listaFreq[256], qtdSimbolos = 0;
    int simbUnico = -1;

    for (int i = 0; i < 256; i++) {
      if (h[i].freq > 0) {
        listaSimbs[qtdSimbolos] = i;
        listaFreq[qtdSimbolos] = h[i].freq;
        simbUnico = i;
        qtdSimbolos++;
      }
    }

    if (qtdSimbolos == 1) {
      h[simbUnico].n = 1;
      h[simbUnico].bits = malloc(sizeof(int));
      h[simbUnico].bits[0] = 0;
    } else if (qtdSimbolos > 1) {
      huf(listaSimbs, listaFreq, qtdSimbolos, h);
    }

    int totalBitsHuf = 0;
    for (int i = 0; i < tam; i++)
      totalBitsHuf += h[simbs[i].simb].n;

    int bytesHuf = (totalBitsHuf + 7) / 8;
    float taxaHuf = (bytesHuf * 100.0) / tam;

    int bytesRLE = 0;
    int i = 0;
    while (i < tam) {
      int cont = 1;
      while (i + cont < tam && simbs[i].simb == simbs[i + cont].simb && cont < 255)
        cont++;
      bytesRLE += 2;
      i += cont;
    }
    float taxaRLE = (bytesRLE * 100.0) / tam;

    exibir(m, taxaHuf, taxaRLE, tam, simbs, h, bytesHuf, bytesRLE, output);

    for (int j = 0; j < 256; j++) {
      if (h[j].bits) free(h[j].bits);
    }
  }
}

int main(int argc, char* argv[]) {
  printf("#ARGS = %i\n", argc);
  printf("PROGRAMA = %s\n", argv[0]);
  printf("ARG1 = %s, ARG2 = %s\n", argv[1], argv[2]);

  FILE* input = fopen(argv[1], "r");
  FILE* output = fopen(argv[2], "w");

  processarEntrada(input, output);

  fclose(input);
  fclose(output);
  return 0;
}