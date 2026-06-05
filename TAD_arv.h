#ifndef ARVORE_H
#define ARVORE_H

#include <stdio.h>
#include "registro.h"

typedef struct arv {
    int        id;       
    long       posicao;  
    struct arv *esq;
    struct arv *dir;
} arv;

arv *inicializa(void);
arv *insereArv(arv *raiz, int id, long posicao);
int alturaArv(arv *raiz);
arv *podaArv(arv *raiz);
arv *buscaArv(arv *raiz, int id);
void preFixado(arv *raiz);
void central(arv *raiz);
void posFix(arv *raiz);
arv *construirIndice(FILE *arq);
int buscarNoArquivo(arv *raiz, int id, FILE *arq, Registro *reg);

#endif /* ARVORE_H */
