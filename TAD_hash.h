#include<stdio.h>
#include "registro.h"

typedef struct hashNo{
    int id;
    long posicao;
    struct hashNo *prox;
}hashNo;

typedef struct tabelaHash{
    hashNo **vetor;
    int tamanho;
}tabelaHash;

tabelaHash* inicializaHash(int tamanho);
void insereHash(tabelaHash *tabela, int id, long posicao);
tabelaHash* indiceHash(FILE *arquivo, int tamanho);
int buscarHashArquivo(tabelaHash *tabela, int id_buscado, FILE *arq, registro *reg, int *colisoes);
void liberaHash(tabelaHash *tabela);