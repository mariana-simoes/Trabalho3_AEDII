#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#include "registro.h"
#include "TAD_hash.h"
#include "estatisticas.h"

#define ARQUIVO_DADOS "dados.bin"
#define N_REGISTROS 100000
#define N_BUSCAS 30

int main(){
    FILE *arquivo = fopen(ARQUIVO_DADOS, "rb");

    if(arquivo == NULL){
        arquivo_gerar(ARQUIVO_DADOS, N_REGISTROS);
        arquivo = arquivo_abrir(ARQUIVO_DADOS, "rb");
    }
    if(arquivo == NULL){
        return 1;
    }
    /*
    utilizando o numero primo mais proximo da metade de 100.000, 
    garantindo que cada posicao da tabela hash tenha em média 2 
    elementos (próximo da metade) e que ocorra o menor numero 
    de colisões possivel (número primo, levando em conta as propriedades da função hash).
    */
    tabelaHash *tabela = indiceHash(arquivo, 50021);

    int chaves[N_BUSCAS];
    gerar_chaves(chaves,N_BUSCAS,N_REGISTROS);

    double tempos_hash[N_BUSCAS];
    int colisoes_totais = 0;
    registro resultado;

    for(int i = 0; i < N_BUSCAS; i++){
        int colisoes = 0;

        double t0 = tempoAtual();
        int achou = buscarHashArquivo(tabela, chaves[i], arquivo, &resultado, &colisoes);
        double t1 = tempoAtual();

        tempos_hash[i] = t1 - t0;
        colisoes_totais += colisoes;

        printf("Chave: %d %s | Tempo: %.9f segundos | Colisoes: %d\n",chaves[i], achou ? "encontrada" : "nao encontrada",tempos_hash[i],colisoes);

    }

    double media_hash = media(tempos_hash,N_BUSCAS);
    double desvio_hash = desvioPadrao(tempos_hash,N_BUSCAS,media_hash);

    printf("\n\n");
    printf("Tempo Medio Hash: %.9f segundos\n",media_hash);
    printf("Desvio padrao: %.9f\n",desvio_hash);
    printf("Total de colisoes: %d",colisoes_totais);

    liberaHash(tabela);
    arquivo_fechar(arquivo);

    return 0;
}