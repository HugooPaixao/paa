#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    char cod[10];
    char** genes;
    int qtdGenes;
    int porcentagem;
    int idx;
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

void counting_sort(Doenca* A, Doenca* B , int n) {
    int k = 101; // porcentagem maxima +1
    int* C = (int*) caloc(k, sizeof(int));

    for(int i = 0; i < n; i++)
        C[A[i].porcentagem]++;

    for(int i = 1; i < k; i++)
        C[i] = C[i]+C[i-1];

    for(int i = n-1; i >= 0; i--) {
        B[C[A[i].porcentagem] -1] = A[i];
        C[A[i].porcentagem]--;
    }
    free(C);

    for(int i = 0; i < n/2; i++) {
        Doenca temp = B[i];
        B[i] = B[n-1-i];
        B[n-1-i] = temp;
    }

}
Doenca* processarEntrada(FILE* input, int* K, char* Dna, int* M) {
    fscanf(input, "%d", K);
    fscanf(input, "%10004s", Dna);
    fscanf(input, "%d", M);

    Doenca* d = (Doenca*)malloc((*M) * sizeof(Doenca));

    for (int i = 0; i < *M; i++) {
        fscanf(input, "%9s %d", d[i].cod, &d[i].qtdGenes);
        d[i].genes = (char**)malloc(d[i].qtdGenes * sizeof(char*));
        d[i].idx = i;
        d[i].porcentagem = 0;

        for (int j = 0; j < d[i].qtdGenes; j++) {
            d[i].genes[j] = (char*)malloc(1001);
            fscanf(input, "%1000s", d[i].genes[j]);
        }
    }

    return d;
}

void exibir(FILE* output, Doenca* d, int M) {
    for (int i = 0; i < M; i++) {
        fprintf(output, "%s->%d%%\n", d[i].cod, d[i].porcentagem);
    }
}

void calcularPorcentagens(Doenca* d, int M, char* Dna, int K) {
    for (int i = 0; i < M; i++) {
        int genesAtivos = 0;

        for (int g = 0; g < d[i].qtdGenes; g++) {
            int tamGene = strlen(d[i].genes[g]);

            if (tamGene < K) {
                char* gene = d[i].genes[g];
                int ktab[1001];
                int Rcount = 0;
                KMP(ktab, &Rcount, Dna, gene);
                if (Rcount > 0) {
                    genesAtivos++;
                }
                continue;
            }

            int* coberto = (int*)calloc(tamGene, sizeof(int));
            int qtdSubcadeias = tamGene - K + 1;

            for (int pos = 0; pos < qtdSubcadeias; pos++) {
                char sub[1001];
                memcpy(sub, d[i].genes[g] + pos, K);
                sub[K] = '\0';

                int ktab[1001];
                int Rcount = 0;
                KMP(ktab, &Rcount, Dna, sub);

                if (Rcount > 0) {
                    for (int k = 0; k < K; k++) {
                        coberto[pos + k] = 1;
                    }
                }
            }

            int total = 0;
            for (int p = 0; p < tamGene; p++) {
                if (coberto[p]) total++;
            }

            float porcentagemualCoberto = (float)total / (float)tamGene * 100.0;
            if (porcentagemualCoberto >= 90.0) {
                genesAtivos++;
            }

            free(coberto);
        }

        d[i].porcentagem = round((100.0 * genesAtivos / d[i].qtdGenes));
    }
}

void liberarMemoria(Doenca* d, int M) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < d[i].qtdGenes; j++) {
            free(d[i].genes[j]);
        }
        free(d[i].genes);
    }
    free(d);
}

int main(int argc, char* argv[]) {
    printf("#ARGS = %i\n", argc);
    printf("PROGRAMA = %s\n", argv[0]);
    printf("ARG1 = %s, ARG2 = %s\n", argv[1], argv[2]);

    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");

    int K, M;
    char Dna[10005];

    Doenca* d = processarEntrada(input, &K, Dna, &M);
    calcularPorcentagens(d, M, Dna, K);

    Doenca* ordenar = (Doenca*)malloc(M*sizeof(Doenca));
    counting_sort(d, ordenar, M);
    exibir(output, d, M);
    liberarMemoria(d, M);

    fclose(input);
    fclose(output);
    return 0;
}