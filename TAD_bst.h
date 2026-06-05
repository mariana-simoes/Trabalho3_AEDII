#ifndef BST_H
#define BST_H

#include <stdio.h>
#include "registro.h"

typedef struct NoBST {
    int chave;    /* id do registro */
    long posicao;  /* número do registro no arquivo */
    struct NoBST *esq;
    struct NoBST *dir;
} NoBST;

NoBST *bst_novo_no(int chave, long posicao);
NoBST *bst_inserir(NoBST *raiz, int chave, long posicao);
int bst_buscar(NoBST *raiz, int chave, FILE *arq, registro *resultado);
NoBST *bst_construir_indice(FILE *arq);
void bst_imprimir_inorder(NoBST *raiz);
int bst_altura(NoBST *raiz);
void bst_liberar(NoBST *raiz);

#endif /* BST_H */
