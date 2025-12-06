#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    char *codigo;
    char **genes;
    int num_genes;
    double prob;
    int ordem;
} Doenca;

int match90(const char *dna, const char *sub, int k) {
    int n = strlen(dna);
    int min_iguais = (int)ceil(k * 0.9);

    for (int i = 0; i <= n - k; i++) {
        int iguais = 0;
        for (int j = 0; j < k; j++)
            if (dna[i+j] == sub[j]) iguais++;

        if (iguais >= min_iguais)
            return 1;
    }
    return 0;
}

double probGene(const char *dna, const char *gene, int k) {
    int L = strlen(gene);
    int total = L - k + 1;
    int ok = 0;

    char sub[2000];

    for (int i = 0; i < total; i++) {
        strncpy(sub, gene + i, k);
        sub[k] = '\0';
        if (match90(dna, sub, k)) ok++;
    }

    return (100.0 * ok) / total;
}

double probDoenca(const char *dna, Doenca *d, int k) {
    double soma = 0;
    for (int i = 0; i < d->num_genes; i++)
        soma += probGene(dna, d->genes[i], k);
    return soma / d->num_genes;
}

void merge(Doenca *v, int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    Doenca *L = malloc(n1 * sizeof(Doenca));
    Doenca *R = malloc(n2 * sizeof(Doenca));

    for (int i = 0; i < n1; i++) L[i] = v[l+i];
    for (int i = 0; i < n2; i++) R[i] = v[m+1+i];

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2) {
        if (L[i].prob > R[j].prob ||
           (L[i].prob == R[j].prob && L[i].ordem < R[j].ordem))
            v[k++] = L[i++];
        else
            v[k++] = R[j++];
    }
    while (i < n1) v[k++] = L[i++];
    while (j < n2) v[k++] = R[j++];

    free(L);
    free(R);
}

void mergesort(Doenca *v, int l, int r) {
    if (l < r) {
        int m = (l+r)/2;
        mergesort(v,l,m);
        mergesort(v,m+1,r);
        merge(v,l,m,r);
    }
}

int main(int argc, char **argv) {
    FILE *in = fopen(argv[1], "r");
    FILE *out = fopen(argv[2], "w");

    int k, M;
    char dna[100000];
    fscanf(in, "%d", &k);
    fscanf(in, "%s", dna);
    fscanf(in, "%d", &M);

    Doenca *D = calloc(M, sizeof(Doenca));

    for (int i = 0; i < M; i++) {
        char cod[100];
        int ng;
        fscanf(in, "%s %d", cod, &ng);

        D[i].codigo = strdup(cod);
        D[i].num_genes = ng;
        D[i].genes = calloc(ng, sizeof(char*));
        D[i].ordem = i;

        for (int j = 0; j < ng; j++) {
            char g[2000];
            fscanf(in, "%s", g);
            D[i].genes[j] = strdup(g);
        }

        D[i].prob = round(probDoenca(dna, &D[i], k));
    }

    mergesort(D, 0, M-1);

    for (int i = 0; i < M; i++)
        fprintf(out, "%s ->%.0f%%\n", D[i].codigo, D[i].prob);
}
