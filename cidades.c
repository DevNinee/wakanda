#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cidades.h"

Estrada *getEstrada(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) return NULL;

    Estrada *e = (Estrada *)malloc(sizeof(Estrada));
    if (!e) return NULL;

    if (fscanf(arquivo, "%d", &e->T) != 1 || e->T < 3 || e->T > 1000000) {
        fclose(arquivo);
        free(e);
        return NULL;
    }

    if (fscanf(arquivo, "%d", &e->N) != 1 || e->N < 2 || e->N > 10000) {
        fclose(arquivo);
        free(e);
        return NULL;
    }

    e->C = (Cidade *)malloc(sizeof(Cidade) * e->N);
    if (!e->C) {
        fclose(arquivo);
        free(e);
        return NULL;
    }

    for (int i = 0; i < e->N; i++) {
        if (fscanf(arquivo, "%d %[^
]", &e->C[i].Posicao, e->C[i].Nome) != 2) {
            free(e->C);
            free(e);
            fclose(arquivo);
            return NULL;
        }

        if (e->C[i].Posicao <= 0 || e->C[i].Posicao >= e->T) {
            free(e->C);
            free(e);
            fclose(arquivo);
            return NULL;
        }

        for (int j = 0; j < i; j++) {
            if (e->C[i].Posicao == e->C[j].Posicao) {
                free(e->C);
                free(e);
                fclose(arquivo);
                return NULL;
            }
        }
    }

    fclose(arquivo);
    return e;
}

int compararPosicao(const void *a, const void *b) {
    Cidade *c1 = (Cidade *)a;
    Cidade *c2 = (Cidade *)b;
    return c1->Posicao - c2->Posicao;
}

double calcularMenorVizinhanca(const char *nomeArquivo) {
    Estrada *e = getEstrada(nomeArquivo);
    if (!e) return -1;

    qsort(e->C, e->N, sizeof(Cidade), compararPosicao);

    double menor = (e->C[1].Posicao - 0) / 2.0;
    for (int i = 1; i < e->N - 1; i++) {
        double vizinhanca = (e->C[i + 1].Posicao - e->C[i - 1].Posicao) / 2.0;
        if (vizinhanca < menor) {
            menor = vizinhanca;
        }
    }

    double ultima = (e->T - e->C[e->N - 2].Posicao) / 2.0;
    if (ultima < menor) {
        menor = ultima;
    }

    free(e->C);
    free(e);
    return menor;
}

char *cidadeMenorVizinhanca(const char *nomeArquivo) {
    Estrada *e = getEstrada(nomeArquivo);
    if (!e) return NULL;

    qsort(e->C, e->N, sizeof(Cidade), compararPosicao);

    double menor = (e->C[1].Posicao - 0) / 2.0;
    int indiceMenor = 0;

    for (int i = 1; i < e->N - 1; i++) {
        double vizinhanca = (e->C[i + 1].Posicao - e->C[i - 1].Posicao) / 2.0;
        if (vizinhanca < menor) {
            menor = vizinhanca;
            indiceMenor = i;
        }
    }

    double ultima = (e->T - e->C[e->N - 2].Posicao) / 2.0;
    if (ultima < menor) {
        menor = ultima;
        indiceMenor = e->N - 1;
    }

    char *resultado = (char *)malloc(strlen(e->C[indiceMenor].Nome) + 1);
    strcpy(resultado, e->C[indiceMenor].Nome);

    free(e->C);
    free(e);
    return resultado;
}
