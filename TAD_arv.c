#include <stdio.h>
#include <stdlib.h>
#include "TAD_arv.h"

arv *inicializa(void)
{
    return NULL;
}

// modificação: tem a posição em vez da string.
arv *insereArv(arv *raiz, int id, long posicao)
{
    if (raiz == NULL) {
        arv *novo = (arv *) malloc(sizeof(arv));
        if (novo == NULL) {
            perror("insereArv: malloc");
            return NULL;
        }
        novo->id      = id;
        novo->posicao = posicao;   /* NOVO: guarda posição, não string */
        novo->esq     = NULL;
        novo->dir     = NULL;
        return novo;
    }

    if (id < raiz->id)
        raiz->esq = insereArv(raiz->esq, id, posicao);
    else if (id > raiz->id)
        raiz->dir = insereArv(raiz->dir, id, posicao);

    return raiz;
}

int alturaArv(arv *raiz)
{
    if (raiz == NULL) return -1;
    int altEsq = alturaArv(raiz->esq);
    int altDir = alturaArv(raiz->dir);
    return 1 + (altEsq > altDir ? altEsq : altDir);
}

arv *podaArv(arv *raiz)
{
    if (raiz != NULL) {
        raiz->esq = podaArv(raiz->esq);
        raiz->dir = podaArv(raiz->dir);
        free(raiz);
    }
    return NULL;
}

arv *buscaArv(arv *raiz, int id)
{
    if (raiz == NULL || raiz->id == id)
        return raiz;

    if (id < raiz->id)
        return buscaArv(raiz->esq, id);

    return buscaArv(raiz->dir, id);
}

// imprime id e posicao em vez de id e dado. Mesma lógica do trabalho anterior.
void preFixado(arv *raiz)
{
    if (raiz) {
        printf("[id=%d pos=%ld] ", raiz->id, raiz->posicao);
        preFixado(raiz->esq);
        preFixado(raiz->dir);
    }
}

void central(arv *raiz)
{
    if (raiz) {
        central(raiz->esq);
        printf("[id=%d pos=%ld] ", raiz->id, raiz->posicao);
        central(raiz->dir);
    }
}

void posFix(arv *raiz)
{
    if (raiz) {
        posFix(raiz->esq);
        posFix(raiz->dir);
        printf("[id=%d pos=%ld] ", raiz->id, raiz->posicao);
    }
}

// Função nova
arv *construirIndice(FILE *arq)
{
    arv     *raiz = inicializa();
    Registro reg;
    long     total = arquivo_total_registros(arq);

    for (long pos = 0; pos < total; pos++) {
        if (arquivo_ler(arq, &reg, pos)) {
            raiz = insereArv(raiz, reg.id, pos);
        }
    }

    printf("[construirIndice] %ld registros indexados. "
           "Altura da arvore: %d\n", total, alturaArv(raiz));
    return raiz;
}

// Função nova
int buscarNoArquivo(arv *raiz, int id, FILE *arq, Registro *reg)
{
    arv *no = buscaArv(raiz, id);
    if (no == NULL)
        return 0;

    return arquivo_ler(arq, reg, no->posicao);
}
