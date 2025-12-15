#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    char cod[10];
    char** genes;
    int qtdGenes;
    int percent;
    int idx; // índice original para estabilidade
} Doenca;

void calcular_tabela(int* k, char* P, int m) {
    k[0] = -1;
    for (int i = 1, j = -1; i < m; i++) {
        while (j >= 0 && P[j + 1] != P[i]) j = k[j];
        if (P[j + 1] == P[i]) j++;
        k[i] = j;
    }
}

void inserir(int* R, int pos) {
    (void)pos;
    (*R)++;
}

void KMP(int* k, int* R, char* T, char* P) {
    int n = strlen(T), m = strlen(P);
    calcular_tabela(k, P, m);
    for (int i = 0, j = -1; i < n; i++) {
        while (j >= 0 && P[j + 1] != T[i]) j = k[j];
        if (P[j + 1] == T[i]) j++;
        if (j == m - 1) {
            inserir(R, i - m + 1);
            j = k[j];
        }
    }
}

void merge(Doenca* arr, int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    Doenca* L = (Doenca*)malloc(n1 * sizeof(Doenca));
    Doenca* R = (Doenca*)malloc(n2 * sizeof(Doenca));
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int i = 0; i < n2; i++) R[i] = arr[m + 1 + i];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i].percent > R[j].percent) {
            arr[k++] = L[i++];
        } else if (L[i].percent < R[j].percent) {
            arr[k++] = R[j++];
        } else {
            arr[k++] = (L[i].idx <= R[j].idx) ? L[i++] : R[j++];
        }
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    free(L);
    free(R);
}

void mergesort(Doenca* arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergesort(arr, l, m);
        mergesort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <entrada> <saida>\n", argv[0]);
        return 1;
    }

    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");
    if (!input || !output) {
        fprintf(stderr, "Erro ao abrir arquivos\n");
        return 1;
    }

    int K, M;
    if (fscanf(input, "%d", &K) != 1) { fprintf(stderr, "Erro ao ler K\n"); return 1; }

    char DNA[10005];
    if (fscanf(input, "%10004s", DNA) != 1) { fprintf(stderr, "Erro ao ler DNA\n"); return 1; }

    if (fscanf(input, "%d", &M) != 1) { fprintf(stderr, "Erro ao ler M\n"); return 1; }
    Doenca* d = (Doenca*)malloc(M * sizeof(Doenca));
    if (!d) { perror("malloc"); return 1; }

    for (int i = 0; i < M; i++) {
        if (fscanf(input, "%9s %d", d[i].cod, &d[i].qtdGenes) != 2) {
            fprintf(stderr, "Erro ao ler doença %d\n", i); return 1;
        }
        d[i].genes = (char**)malloc(d[i].qtdGenes * sizeof(char*));
        if (!d[i].genes) { perror("malloc"); return 1; }
        d[i].idx = i;
        d[i].percent = 0;

        for (int j = 0; j < d[i].qtdGenes; j++) {
            d[i].genes[j] = (char*)malloc(1001);
            if (!d[i].genes[j]) { perror("malloc"); return 1; }
            if (fscanf(input, "%1000s", d[i].genes[j]) != 1) {
                fprintf(stderr, "Erro ao ler gene %d da doença %s\n", j, d[i].cod); return 1;
            }
        }
    }

    // Cálculo correto: verificar cobertura de bases do gene
    for (int i = 0; i < M; i++) {
        int genesAtivos = 0;

        for (int g = 0; g < d[i].qtdGenes; g++) {
            int lenGene = strlen(d[i].genes[g]);

            // Se o gene for menor que K, verificamos se o gene inteiro aparece
            if (lenGene < K) {
                char* gene = d[i].genes[g];
                int ktab[1001];
                int Rcount = 0;
                KMP(ktab, &Rcount, DNA, gene);
                if (Rcount > 0) {
                    genesAtivos++; // Gene inteiro encontrado
                }
                continue;
            }

            // Para genes com comprimento >= K
            int* coberto = (int*)calloc(lenGene, sizeof(int));
            int numSub = lenGene - K + 1;

            // Para cada substring de tamanho K no gene
            for (int pos = 0; pos < numSub; pos++) {
                char sub[1001];
                memcpy(sub, d[i].genes[g] + pos, K);
                sub[K] = '\0';

                // Verifica se esta substring aparece no DNA
                int ktab[1001];
                int Rcount = 0;
                KMP(ktab, &Rcount, DNA, sub);

                // Se aparecer, marca as posições cobertas
                if (Rcount > 0) {
                    for (int k = 0; k < K; k++) {
                        coberto[pos + k] = 1;
                    }
                }
            }

            // Conta quantas posições foram cobertas
            int totalCoberto = 0;
            for (int p = 0; p < lenGene; p++) {
                if (coberto[p]) totalCoberto++;
            }

            // Verifica se pelo menos 90% das posições foram cobertas
            float percentualCoberto = (float)totalCoberto / (float)lenGene * 100.0;
            if (percentualCoberto >= 90.0) {
                genesAtivos++;
            }

            free(coberto);
        }

        // Calcula porcentagem final da doença
        d[i].percent = round((100.0 * genesAtivos / d[i].qtdGenes));
    }

    mergesort(d, 0, M - 1);

    for (int i = 0; i < M; i++) {
        fprintf(output, "%s->%d%%\n", d[i].cod, d[i].percent);
    }

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < d[i].qtdGenes; j++) free(d[i].genes[j]);
        free(d[i].genes);
    }
    free(d);
    fclose(input);
    fclose(output);
    return 0;
}