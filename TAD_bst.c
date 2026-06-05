#include "TAD_bst.h"

#include <stdio.h>
#include <stdlib.h>

NoBST *bst_novo_no(int chave, long posicao) {
    NoBST *no = (NoBST *)malloc(sizeof(NoBST));
    if (no == NULL) {
        perror("bst_novo_no: malloc");
        return NULL;
    }
    no->chave   = chave;
    no->posicao = posicao;
    no->esq     = NULL;
    no->dir     = NULL;
    return no;
}


NoBST *bst_inserir(NoBST *raiz, int chave, long posicao)
{
    /* caso base: posição vazia, cria o nó aqui */
    if (raiz == NULL) {
        return bst_novo_no(chave, posicao);
    }

    if (chave < raiz->chave) {
        raiz->esq = bst_inserir(raiz->esq, chave, posicao);
    } else if (chave > raiz->chave) {
        raiz->dir = bst_inserir(raiz->dir, chave, posicao);
    }
    /* chave == raiz->chave: duplicata, não faz nada */

    return raiz;
}

int bst_buscar(NoBST *raiz, int chave, FILE *arq, registro *resultado)
{

    if (raiz == NULL) {
        return 0;
    }

    if (chave == raiz->chave) {
        /* achou o nó; agora busca o registro no arquivo */
        return arquivo_ler(arq, resultado, raiz->posicao);
    }

    if (chave < raiz->chave) {
        return bst_buscar(raiz->esq, chave, arq, resultado);
    } else {
        return bst_buscar(raiz->dir, chave, arq, resultado);
    }
}

NoBST *bst_construir_indice(FILE *arq)
{
    NoBST   *raiz = NULL;
    registro reg;
    long     total = arquivo_total_registros(arq);

    for (long pos = 0; pos < total; pos++) {
        if (arquivo_ler(arq, &reg, pos)) {
            raiz = bst_inserir(raiz, reg.id, pos);
        }
    }

    printf("[bst_construir_indice] %ld registros indexados. "
           "Altura da arvore: %d\n", total, bst_altura(raiz));
    return raiz;
}


void bst_imprimir_inorder(NoBST *raiz)
{
    if (raiz == NULL) return;
    bst_imprimir_inorder(raiz->esq);
    printf("  chave=%d  posicao=%ld\n", raiz->chave, raiz->posicao);
    bst_imprimir_inorder(raiz->dir);
}

int bst_altura(NoBST *raiz)
{
    if (raiz == NULL) return -1;

    int h_esq = bst_altura(raiz->esq);
    int h_dir = bst_altura(raiz->dir);

    return 1 + (h_esq > h_dir ? h_esq : h_dir);
}

void bst_liberar(NoBST *raiz)
{
    if (raiz == NULL) return;
    bst_liberar(raiz->esq);
    bst_liberar(raiz->dir);
    free(raiz);
}
