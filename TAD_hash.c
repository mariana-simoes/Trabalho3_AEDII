#include<stdio.h>
#include<stdlib.h>
#include "TAD_hash.h"

tabelaHash* inicializaHash(int tamanho){
    tabelaHash *tabela = (tabelaHash*) malloc(sizeof(tabelaHash));
    if(tabela != NULL){
        tabela->tamanho = tamanho;
        tabela->vetor = (hashNo**) malloc(tamanho * sizeof(hashNo*));
        if(tabela->vetor == NULL){
            free(tabela);
            return NULL;
        }
        for(int i = 0; i < tabela->tamanho; i++){
            tabela->vetor[i] = NULL;
        }
        return tabela;
    }
    return NULL;
}

int funcao_Hash(int id, int tamanho){
    return id % tamanho;
}

void insereHash(tabelaHash *tabela, int id, long posicao){
    int indice = funcao_Hash(id, tabela->tamanho);
    hashNo *novo = (hashNo*) malloc(sizeof(hashNo));
    novo->id = id;
    novo->posicao = posicao;

    novo->prox = tabela->vetor[indice];
    tabela->vetor[indice] = novo;
}

tabelaHash* indiceHash(FILE *arquivo, int tamanho){
    tabelaHash *tabela = inicializaHash(tamanho);
    registro reg;
    long total = arquivo_total_registros(arquivo);
    
    for(long pos = 0; pos < total; pos++){
        if(arquivo_ler(arquivo, &reg, pos)){
            insereHash(tabela, reg.id, pos);
        }
    }

    printf("(Funcao indiceHash): %ld registros indexados na tabela Hash.\n\n",total);
    return tabela;
}

int buscarHashArquivo(tabelaHash *tabela, int id_buscado, FILE *arquivo, registro *reg, int *colisoes){
    int indice = funcao_Hash(id_buscado,tabela->tamanho);
    hashNo *atual = tabela->vetor[indice];

    *colisoes = 0;

    while(atual != NULL){
        if(atual->id == id_buscado){
            return arquivo_ler(arquivo, reg, atual->posicao);
        }
        (*colisoes)++;
        atual = atual->prox;
    }
    return 0;
}

void liberaHash(tabelaHash *tabela){
    for(int i = 0; i < tabela->tamanho; i++){
        hashNo *atual = tabela->vetor[i];
        while(atual != NULL){
            hashNo *temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    free(tabela->vetor);
    free(tabela);
}