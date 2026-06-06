#ifndef REGISTRO_H
#define REGISTRO_H

#include <stdio.h>

/* =========================================================
 * Tamanho do registro:
 *   id          : 4 bytes  (int)
 *   nome        : 40 bytes (char[40])
 *   categoria   : 20 bytes (char[20])
 *   preco       : 4 bytes  (float)
 *   quantidade  : 4 bytes  (int)
 *   ano         : 4 bytes  (int) <- usado para consultas >, <
 *   _padding    : 24 bytes (char[24]) <- para completar 100 bytes
 * Total         : 100 bytes
 * ========================================================= */

#define TAM_NOME      40
#define TAM_CATEGORIA 20
#define TAM_PADDING   24
#define TAM_REGISTRO  100

typedef struct registro{
    int   id; // chave única
    char  nome[TAM_NOME];
    char  categoria[TAM_CATEGORIA];
    float preco;
    int   quantidade;
    int   ano; // atributo não chave. Pode ser útil pra quarta questão, mas vejam aí             
    char  _padding[TAM_PADDING];
} registro;


FILE *arquivo_abrir(const char *caminho, const char *modo);
void arquivo_fechar(FILE *arq);
int arquivo_escrever(FILE *arq, const registro *reg, long pos);
int arquivo_ler(FILE *arq, registro *reg, long pos);
long arquivo_total_registros(FILE *arq);
void gerar_chaves(int chaves[], int n, int max);

void arquivo_gerar(const char *caminho, int n);

void registro_imprimir(const registro *reg);

int arquivo_busca_seq_chave(FILE *arq, int chave_alvo, registro *resultado);

#endif /* REGISTRO_H */
