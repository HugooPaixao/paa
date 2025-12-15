#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct veiculo {
    char placa[8];
    float peso, volume;
} Veiculo;

typedef struct pac {
    char cod[14];
    float valor, peso, volume;
    int processado; // 1 = processado, 0 = nao processado
} Pacotes;

float max(float a, float b) {
    return (a > b) ? a : b;
}

int calcularIndice(int i, int j, int k, int w, int v) {
    return (i * (w+1) * (v+1)) + (j * (v+1)) + k;
}

float* alocarMatriz(int qtdItens, int w, int v) {
    int size = (qtdItens+1) * (w+1) * (v+1);
    return (float*)calloc(size, sizeof(float));
}

void mochila(int qtdItens, int peso, int volume, Pacotes itens[], int sel[]) {
    float* m = alocarMatriz(qtdItens, peso, volume);

    for(int i = 1; i <= qtdItens; i++) {
        int p = (int)round(itens[i-1].peso);
        int vol = (int)round(itens[i-1].volume);
        for(int j = 0; j <= peso; j++) {
            for(int k = 0; k <= volume; k++) {
                int pos = calcularIndice(i,j,k,peso,volume);
                int posPrev = calcularIndice(i-1,j,k,peso,volume);
                m[pos] = m[posPrev];
                if(j >= p && k >= vol) {
                    int posAlt = calcularIndice(i-1,j-p,k-vol,peso,volume);
                    m[pos] = max(itens[i-1].valor + m[posAlt], m[posPrev]);
                }
            }
        }
    }

    int aux = peso, aux2 = volume;
    for(int i = qtdItens; i > 0; i--) {
        int pos = calcularIndice(i,aux,aux2,peso,volume);
        int posPrev = calcularIndice(i-1,aux,aux2,peso,volume);
        if(m[pos] != m[posPrev]) {
            sel[i-1] = 1;
            aux -= (int)round(itens[i-1].peso);
            aux2 -= (int)round(itens[i-1].volume);
        } else {
            sel[i-1] = 0;
        }
    }
    free(m);
}

void processarVeiculos(int qtdVeiculos, Veiculo v[], FILE* input) {
    for(int i = 0; i < qtdVeiculos; i++)
        fscanf(input, "%s %f %f", v[i].placa, &v[i].peso, &v[i].volume);
}

void processarPacotes(int qtdPacotes, Pacotes p[], FILE* input) {
    for(int i = 0; i < qtdPacotes; i++) {
        fscanf(input, "%s %f %f %f", p[i].cod, &p[i].valor, &p[i].peso, &p[i].volume);
        p[i].processado = 0;
    }
}

void exibir(Veiculo v, Pacotes qtdItens[], int qtd, int sel[], FILE* output) {
    float volAcumulado = 0, pesoAcumulado = 0, valAcumulado = 0;
    int itemSel = 0;

    for(int i = 0; i < qtd; i++) {
        if(sel[i]) {
            valAcumulado += qtdItens[i].valor;
            pesoAcumulado += qtdItens[i].peso;
            volAcumulado += qtdItens[i].volume;
            itemSel = 1;
        }
    }

    if(itemSel) {
        float porcentagemP = round((pesoAcumulado/v.peso)*100);
        float porcentagemV = round((volAcumulado/v.volume)*100);

        fprintf(output, "[%s]R$%.2f,%.0fKG(%.0f%%),%.0fL(%.0f%%)->",
                v.placa, valAcumulado, pesoAcumulado, porcentagemP, volAcumulado, porcentagemV);

        int virgula = 0;
        for(int i = 0; i < qtd; i++) {
            if(sel[i]) {
                if(virgula) fprintf(output, ",");
                fprintf(output, "%s", qtdItens[i].cod);
                virgula = 1;
            }
        }
        fprintf(output, "\n");
    }
}

void exibirPendentes(Pacotes p[], int qtdPacotes, FILE* output) {
    float volAcumulado = 0, pesoAcumulado = 0, valAcumulado = 0;
    int pendente = 0;

    for(int i = 0; i < qtdPacotes; i++) {
        if(!p[i].processado) {
            valAcumulado += p[i].valor;
            pesoAcumulado += p[i].peso;
            volAcumulado += p[i].volume;
            pendente = 1;
        }
    }

    if(pendente) {
        fprintf(output, "PENDENTE:R$%.2f,%.0fKG,%.0fL->", valAcumulado, pesoAcumulado, volAcumulado);

        int virgula = 0;
        for(int i = 0; i < qtdPacotes; i++) {
            if(!p[i].processado) {
                if(virgula) fprintf(output, ",");
                fprintf(output, "%s", p[i].cod);
                virgula = 1;
            }
        }
        fprintf(output, "\n");
    }
}

void escreverArquivo(int qtdVeiculos, Veiculo v[], int qtdPacotes, Pacotes p[], FILE* output) {
    int *sel = malloc(qtdPacotes*sizeof(int));
    int *itensMax = malloc(qtdPacotes*sizeof(int));
    Pacotes *qtdItens = malloc(qtdPacotes*sizeof(Pacotes));

    for(int i = 0; i < qtdVeiculos; i++) {
        int qtd = 0;
        for(int j = 0; j < qtdPacotes; j++) {
            if(!p[j].processado) {
                qtdItens[qtd] = p[j];
                itensMax[qtd] = j;
                qtd++;
            }
        }

        if(qtd > 0) {
            mochila(qtd, (int)round(v[i].peso), (int)round(v[i].volume), qtdItens, sel);

            for(int k = 0; k < qtd; k++) {
                if(sel[k]) p[itensMax[k]].processado = 1;
            }
            exibir(v[i], qtdItens, qtd, sel, output);
        }
    }

    free(sel);
    free(itensMax);
    free(qtdItens);
}

int main(int argc, char* argv[]) {
    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");

    int qtdVeiculos, qtdPacotes;

    fscanf(input, "%d", &qtdVeiculos);
    Veiculo *v = malloc(qtdVeiculos*sizeof(Veiculo));
    processarVeiculos(qtdVeiculos, v, input);

    fscanf(input, "%d", &qtdPacotes);
    Pacotes *p = malloc(qtdPacotes*sizeof(Pacotes));
    processarPacotes(qtdPacotes, p, input);

    escreverArquivo(qtdVeiculos, v, qtdPacotes, p, output);
    exibirPendentes(p, qtdPacotes, output);

    free(v);
    free(p);
    fclose(input);
    fclose(output);

    return 0;
}