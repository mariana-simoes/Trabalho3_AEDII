#ifndef TAD_ARV_ANO_H
#define TAD_ARV_ANO_H

#include <stdio.h>
#include "registro.h"

/* =========================================================
 * TAD: Árvore Binária de Pesquisa indexada pelo campo 'ano'
 * (atributo não-chave, com valores repetidos).
 *
 * Como 'ano' não é único, cada nó armazena uma lista encadeada
 * de structs 'registro' com aquele valor de 'ano'. A struct
 * registro já é definida em registro.h e contém o campo 'ano',
 * por isso não há necessidade de estruturas auxiliares extras:
 * os dados completos ficam na memória durante toda a execução.
 * ========================================================= */


typedef struct noReg {
    registro      reg;   
    struct noReg *prox;
} noReg;


typedef struct arvAno {
    int            ano;  
    noReg         *lista; // registros com este ano
    struct arvAno *esq;
    struct arvAno *dir;
} arvAno;

/* Lista de resultados retornada pela busca por intervalo */
typedef struct resLista {
    registro          reg;
    struct resLista  *prox;
} resLista;

/* Operadores de comparação */
#define OP_MAIOR        0  /* >  */
#define OP_MAIOR_IGUAL  1  /* >= */
#define OP_MENOR        2  /* <  */
#define OP_MENOR_IGUAL  3  /* <= */

/* Insere um registro na BST pelo campo reg->ano.
 * Se já existir nó com aquele ano, acrescenta à lista. */
arvAno   *insereArvAno(arvAno *raiz, const registro *reg);

// Lê todo o arquivo e constrói o índice BST para 'ano' 
arvAno   *construirIndiceAno(FILE *arq);

// Libera toda a memória da BST (nós + listas de registros) 
void      podaArvAno(arvAno *raiz);

resLista *buscarRangeAno(const arvAno *raiz, int limiar, int op);

// Libera a lista de resultados 
void      liberaResLista(resLista *lista);

// Conta os itens da lista de resultados 
int       contarResLista(const resLista *lista);

// Exibe os anos presentes na BST em ordem crescente
void      exibirIndiceAno(const arvAno *raiz);

#endif /* TAD_ARV_ANO_H */
