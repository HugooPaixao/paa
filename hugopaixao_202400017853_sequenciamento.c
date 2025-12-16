   #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

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

void KMP(int* k, int* R, char* T, long long n, char* P, int m) {
    calcular_tabela(k, P, m);
    for (long long i = 0, j = -1; i < n; i++) {
        while (j >= 0 && P[j + 1] != T[i]) j = k[j];
        if (P[j + 1] == T[i]) j++;
        if (j == m - 1) {
            inserir(R, i - m + 1);
            j = k[j];
        }
    }
}

int compararDoencas(const void* a, const void* b) {
    Doenca* da = (Doenca*)a;
    Doenca* db = (Doenca*)b;

    // Ordenar por porcentagem decrescente
    if (da->porcentagem > db->porcentagem) return -1;
    if (da->porcentagem < db->porcentagem) return 1;

    // Em caso de empate, ordenar pelo índice original (crescente)
    if (da->idx < db->idx) return -1;
    if (da->idx > db->idx) return 1;

    return 0;
}

Doenca* processarEntrada(FILE* input, int* K, char** Dna, long long* dna_len, int* M) {
    fscanf(input, "%d", K);
    
    // Pular espaços em branco
    int c;
    while ((c = fgetc(input)) != EOF && isspace(c));
    ungetc(c, input);
    
    // Ler DNA dinamicamente
    long long capacidade = 1024;
    *Dna = (char*)malloc(capacidade * sizeof(char));
    if (*Dna == NULL) {
        perror("Erro ao alocar memória para DNA");
        exit(1);
    }
    
    *dna_len = 0;
    while ((c = fgetc(input)) != EOF && !isspace(c)) {
        if (*dna_len >= capacidade - 1) {
            capacidade *= 2;
            *Dna = (char*)realloc(*Dna, capacidade * sizeof(char));
            if (*Dna == NULL) {
                perror("Erro ao realocar memória para DNA");
                exit(1);
            }
        }
        (*Dna)[(*dna_len)++] = c;
    }
    (*Dna)[*dna_len] = '\0';
    
    fscanf(input, "%d", M);
    
    Doenca* d = (Doenca*)malloc((*M) * sizeof(Doenca));
    
    for (int i = 0; i < *M; i++) {
        fscanf(input, "%9s %d", d[i].cod, &d[i].qtdGenes);
        d[i].genes = (char**)malloc(d[i].qtdGenes * sizeof(char*));
        d[i].idx = i;
        d[i].porcentagem = 0;
        
        for (int j = 0; j < d[i].qtdGenes; j++) {
            d[i].genes[j] = (char*)malloc(1001 * sizeof(char));
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

void calcularPorcentagens(Doenca* d, int M, char* Dna, long long dna_len, int K) {
    for (int i = 0; i < M; i++) {
        int genesAtivos = 0;
        
        for (int g = 0; g < d[i].qtdGenes; g++) {
            int tamGene = strlen(d[i].genes[g]);
            
            if (tamGene < K) {
                char* gene = d[i].genes[g];
                int m = tamGene;
                int* ktab = (int*)malloc((m > 0 ? m : 1) * sizeof(int));
                int Rcount = 0;
                KMP(ktab, &Rcount, Dna, dna_len, gene, m);
                free(ktab);
                if (Rcount > 0) {
                    genesAtivos++;
                }
                continue;
            }
            
            int* coberto = (int*)calloc(tamGene, sizeof(int));
            int qtdSubcadeias = tamGene - K + 1;
            
            for (int pos = 0; pos < qtdSubcadeias; pos++) {
                char* sub = (char*)malloc((K + 1) * sizeof(char));
                memcpy(sub, d[i].genes[g] + pos, K);
                sub[K] = '\0';
                
                int* ktab = (int*)malloc(K * sizeof(int));
                int Rcount = 0;
                KMP(ktab, &Rcount, Dna, dna_len, sub, K);
                free(ktab);
                free(sub);
                
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
            
            float percentualCoberto = (float)total / (float)tamGene * 100.0;
            if (percentualCoberto >= 90.0) {
                genesAtivos++;
            }
            
            free(coberto);
        }
        
        d[i].porcentagem = round((100.0 * genesAtivos / d[i].qtdGenes));
    }
}

void liberarMemoria(Doenca* d, int M, char* Dna) {
    free(Dna);
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < d[i].qtdGenes; j++) {
            free(d[i].genes[j]);
        }
        free(d[i].genes);
    }
    free(d);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Uso: %s <entrada> <saida>\n", argv[0]);
        return 1;
    }
    
    FILE* input = fopen(argv[1], "r");
    if (!input) {
        perror("Erro ao abrir arquivo de entrada");
        return 1;
    }
    
    FILE* output = fopen(argv[2], "w");
    if (!output) {
        perror("Erro ao abrir arquivo de saída");
        fclose(input);
        return 1;
    }
    
    int K, M;
    char* Dna;
    long long dna_len;
    
    Doenca* d = processarEntrada(input, &K, &Dna, &dna_len, &M);
    calcularPorcentagens(d, M, Dna, dna_len, K);
    
    qsort(d, M, sizeof(Doenca), compararDoencas);
    
    exibir(output, d, M);
    liberarMemoria(d, M, Dna);
    
    fclose(input);
    fclose(output);
    return 0;
}