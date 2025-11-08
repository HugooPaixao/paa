#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pac{
  int qtdPacotes;
  char *dados;
  int prio;
} Pacote;

void trocar(Pacote v[], int x, int y) {
  int temp = v[x].qtdPacotes;
  v[x].qtdPacotes = v[y].qtdPacotes;
  v[y].qtdPacotes = temp;
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

  if(e < t && v[e].qtdPacotes > v[p].qtdPacotes)
    p = e;

  if(d < t && v[d].qtdPacotes > v[p].qtdPacotes)
    p = d;

  if(p != i) {
    trocar(v, p, i);
    heapify(v, t, p);
  }
}

void construir_heap(Pacote v[], int n) {

  for(int i = n/(2-1); i >= 0; i--)
    heapify(v, i, n);

}

void heapsort(Pacote v[], int n) {
  construir_heap(v, n);

  for(int i = n-1; i > 0; i--) {
    trocar(v, 0, i);
    heapify(v, 0, i);
  }
}

int main() {


  return 0;
}