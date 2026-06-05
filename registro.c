#include "registro.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

FILE *arquivo_abrir(const char *caminho, const char *modo)
{
    FILE *arq = fopen(caminho, modo);
    if (arq == NULL) {
        perror("arquivo_abrir");
    }
    return arq;
}

void arquivo_fechar(FILE *arq)
{
    if (arq != NULL) {
        fclose(arq);
    }
}


 // pos é o número do registro (base 0), não o byte offset.
 // O byte offset é calculado aqui: pos * TAM_REGISTRO.
int arquivo_escrever(FILE *arq, const registro *reg, long pos)
{
    long offset = pos * (long)TAM_REGISTRO;
    if (fseek(arq, offset, SEEK_SET) != 0) {
        perror("arquivo_escrever: fseek");
        return 0;
    }
    if (fwrite(reg, TAM_REGISTRO, 1, arq) != 1) {
        perror("arquivo_escrever: fwrite");
        return 0;
    }
    return 1;
}

int arquivo_ler(FILE *arq, registro *reg, long pos)
{
    long offset = pos * (long)TAM_REGISTRO;
    if (fseek(arq, offset, SEEK_SET) != 0) {
        perror("arquivo_ler: fseek");
        return 0;
    }
    if (fread(reg, TAM_REGISTRO, 1, arq) != 1) {
        return 0;
    }
    return 1;
}

long arquivo_total_registros(FILE *arq)
{
    if (fseek(arq, 0, SEEK_END) != 0) {
        perror("arquivo_total_registros: fseek");
        return -1;
    }
    long tamanho_bytes = ftell(arq);
    return tamanho_bytes / (long)TAM_REGISTRO;
}

static const char *NOMES[] = {
    "André", "Bembom", "Benjamim", "Mariana", "Luiggy",
    "Vithor", "Manfred", "Maria", "Joao", "Iago"
};
static const char *CATEGORIAS[] = {
    "Eletronicos", "Alimentos", "Vestuario",
    "Moveis", "Livros"
};
#define N_NOMES      10
#define N_CATEGORIAS 5

void arquivo_gerar(const char *caminho, int n)
{
    srand((unsigned int)time(NULL));

    FILE *arq = fopen(caminho, "wb");
    if (arq == NULL) {
        perror("arquivo_gerar: fopen");
        return;
    }

    // aqui, os IDs já são gerados embaralhados.
    int *ids = (int *)malloc(n * sizeof(int));
    if (ids == NULL) {
        perror("arquivo_gerar: malloc");
        fclose(arq);
        return;
    }
    for (int i = 0; i < n; i++) ids[i] = i + 1;
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = ids[i]; ids[i] = ids[j]; ids[j] = tmp;
    }

    registro reg; // buffer temporário do reg (confesso que n entendi direito)
    for (int i = 0; i < n; i++) {
        memset(&reg, 0, TAM_REGISTRO); /* zera bytes, incluindo _padding */

        reg.id         = ids[i];
        reg.preco      = 10.0f + (float)(rand() % 99000) / 100.0f;
        reg.quantidade = rand() % 1000 + 1;
        reg.ano        = 2000 + rand() % 25; /* 2000 a 2024 */

        snprintf(reg.nome, TAM_NOME, "%s_%04d",
                 NOMES[rand() % N_NOMES], reg.id);

        strncpy(reg.categoria,
                CATEGORIAS[rand() % N_CATEGORIAS],
                TAM_CATEGORIA - 1);

        if (fwrite(&reg, TAM_REGISTRO, 1, arq) != 1) {
            perror("arquivo_gerar: fwrite");
            break;
        }
    }

    free(ids);
    fclose(arq);
    printf("[arquivo_gerar] %d registros escritos em '%s'\n", n, caminho);
}



void registro_imprimir(const registro *reg)
{
    printf("  ID        : %d\n",   reg->id);
    printf("  Nome      : %s\n",   reg->nome);
    printf("  Categoria : %s\n",   reg->categoria);
    printf("  Preco     : %.2f\n", reg->preco);
    printf("  Quantidade: %d\n",   reg->quantidade);
    printf("  Ano       : %d\n",   reg->ano);
}


int arquivo_busca_seq_chave(FILE *arq, int chave_alvo, registro *resultado)
{
    registro reg;

    // Move o ponteiro de leitura para o início do file
    rewind(arq);

    // fread() lê TAM_REGISTRO bytes a cada chamada.
    while (fread(&reg, TAM_REGISTRO, 1, arq) == 1) {
        if (reg.id == chave_alvo) {
            *resultado = reg;
            return 1;
        }
    }
    return 0;
}
