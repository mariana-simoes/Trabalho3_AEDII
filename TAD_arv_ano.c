#include <stdio.h>
#include <stdlib.h>
#include "TAD_arv_ano.h"

/* =========================================================
 * insereArvAno
 * Insere o registro na BST usando reg->ano como chave.
 * Se já existir um nó com aquele ano, o registro é
 * acrescentado à frente da lista encadeada desse nó.
 * ========================================================= */
arvAno *insereArvAno(arvAno *raiz, const registro *reg)
{
    if (raiz == NULL) {
        arvAno *novo = (arvAno *) malloc(sizeof(arvAno));
        if (novo == NULL) { perror("insereArvAno: malloc"); return NULL; }

        noReg *nr = (noReg *) malloc(sizeof(noReg));
        if (nr == NULL) { free(novo); perror("insereArvAno: noReg malloc"); return NULL; }

        nr->reg   = *reg; 
        nr->prox  = NULL;

        novo->ano   = reg->ano;
        novo->lista = nr;
        novo->esq   = NULL;
        novo->dir   = NULL;
        return novo;
    }

    if (reg->ano < raiz->ano)
        raiz->esq = insereArvAno(raiz->esq, reg);
    else if (reg->ano > raiz->ano)
        raiz->dir = insereArvAno(raiz->dir, reg);
    else {
        /* mesmo ano: insere na frente da lista */
        noReg *nr = (noReg *) malloc(sizeof(noReg));
        if (nr == NULL) { perror("insereArvAno: noReg dup malloc"); return raiz; }
        nr->reg     = *reg;
        nr->prox    = raiz->lista;
        raiz->lista = nr;
    }
    return raiz;
}

arvAno *construirIndiceAno(FILE *arq)
{
    arvAno  *raiz  = NULL;
    registro reg;
    long     total = arquivo_total_registros(arq);

    for (long pos = 0; pos < total; pos++) {
        if (arquivo_ler(arq, &reg, pos))
            raiz = insereArvAno(raiz, &reg);
    }

    printf("[construirIndiceAno] %ld registros indexados pelo campo 'ano'.\n", total);
    return raiz;
}

void podaArvAno(arvAno *raiz)
{
    if (raiz == NULL) return;
    podaArvAno(raiz->esq);
    podaArvAno(raiz->dir);

    noReg *p = raiz->lista;
    while (p) { noReg *tmp = p; p = p->prox; free(tmp); }

    free(raiz);
}

void exibirIndiceAno(const arvAno *raiz)
{
    if (raiz == NULL) return;
    exibirIndiceAno(raiz->esq);
    int cnt = 0;
    for (const noReg *p = raiz->lista; p; p = p->prox) cnt++;
    printf("  [ano=%d : %d registro(s)]\n", raiz->ano, cnt);
    exibirIndiceAno(raiz->dir);
}

void liberaResLista(resLista *lista)
{
    while (lista) { resLista *tmp = lista; lista = lista->prox; free(tmp); }
}

int contarResLista(const resLista *lista)
{
    int n = 0;
    while (lista) { n++; lista = lista->prox; }
    return n;
}

/* =========================================================
 * buscarRangeAno
 * Percurso com poda na BST, sem acessar o arquivo:
 * os registros já estão nos nós da árvore.
 *
 * Propriedade da BST (duplicatas na mesma lista):
 *   esq: todos os anos  <  raiz->ano
 *   dir: todos os anos  >  raiz->ano
 *
 * Poda aplicada:
 *   OP_MAIOR / OP_MAIOR_IGUAL — se raiz->ano não satisfaz,
 *     a subárvore esquerda inteira também não satisfaz → poda.
 *   OP_MENOR / OP_MENOR_IGUAL — se raiz->ano não satisfaz,
 *     a subárvore direita inteira também não satisfaz → poda.
 * ========================================================= */
static int satisfaz(int ano, int limiar, int op)
{
    switch (op) {
        case OP_MAIOR:       return ano >  limiar;
        case OP_MAIOR_IGUAL: return ano >= limiar;
        case OP_MENOR:       return ano <  limiar;
        case OP_MENOR_IGUAL: return ano <= limiar;
        default:             return 0;
    }
}

static resLista *traverseRange(const arvAno *raiz, int limiar, int op,
                               resLista *acc)
{
    if (raiz == NULL) return acc;

    if (op == OP_MAIOR || op == OP_MAIOR_IGUAL) {
        if (satisfaz(raiz->ano, limiar, op)) {
            /* este nó satisfaz: copia todos os seus registros */
            for (const noReg *p = raiz->lista; p; p = p->prox) {
                resLista *novo = (resLista *) malloc(sizeof(resLista));
                if (novo == NULL) break;
                novo->reg  = p->reg;
                novo->prox = acc;
                acc        = novo;
            }
            /* subárvore esquerda pode ter anos entre limiar+1 e raiz->ano-1 */
            acc = traverseRange(raiz->esq, limiar, op, acc);
        }
        /* subárvore direita sempre pode ter matches */
        acc = traverseRange(raiz->dir, limiar, op, acc);

    } else { /* OP_MENOR ou OP_MENOR_IGUAL */
        if (satisfaz(raiz->ano, limiar, op)) {
            for (const noReg *p = raiz->lista; p; p = p->prox) {
                resLista *novo = (resLista *) malloc(sizeof(resLista));
                if (novo == NULL) break;
                novo->reg  = p->reg;
                novo->prox = acc;
                acc        = novo;
            }
            /* subárvore direita pode ter anos entre raiz->ano+1 e limiar-1 */
            acc = traverseRange(raiz->dir, limiar, op, acc);
        }
        /* subárvore esquerda sempre pode ter matches */
        acc = traverseRange(raiz->esq, limiar, op, acc);
    }

    return acc;
}

resLista *buscarRangeAno(const arvAno *raiz, int limiar, int op)
{
    return traverseRange(raiz, limiar, op, NULL);
}
