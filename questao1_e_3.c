#define _POSIX_C_SOURCE 199309L // Evita um erro de compilação, de acordo com o Claudekkkk n sei o que é
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "registro.h"
#include "TAD_arv.h"

#define ARQUIVO_DADOS  "dados.bin"
#define N_REGISTROS    100000   // Recomendo colocar 100 pra debugging, 100000 para teste final.
#define N_BUSCAS       30       /* quantidade de buscas para medir tempo */

static void gerar_chaves_teste(int chaves[], int n, int max)
{
    // OBS: Verificar se esse rand está bom
    srand(42);
    for (int i = 0; i < n; i++) {
        chaves[i] = 1 + rand() % max;
    }
}


static void resultadoComparacao(double media_bst, double media_seq)
{
    printf("\n========================================\n");
    printf("       RESULTADO COMPARATIVO (Q1 vs Q3)   \n");
    printf("========================================\n");
    printf("  Tempo medio BST        : %.9f s\n", media_bst);
    printf("  Tempo medio Sequencial : %.9f s\n", media_seq);
    if (media_bst > 0) {
        printf("  Sequencial / BST       : %.1fx mais lenta\n",
               media_seq / media_bst);
    }
    printf("========================================\n");
}

int main(void)
{
    struct timespec t0, t1;

    FILE *arq = fopen(ARQUIVO_DADOS, "rb");
    if (arq == NULL) {
        printf("Arquivo nao encontrado. Gerando %d registros...\n",
               N_REGISTROS);
        arquivo_gerar(ARQUIVO_DADOS, N_REGISTROS);
        arq = arquivo_abrir(ARQUIVO_DADOS, "rb");
    } else {
        long total = arquivo_total_registros(arq);
        printf("Arquivo '%s' aberto. Registros: %ld\n",
               ARQUIVO_DADOS, total);
    }

    if (arq == NULL) {
        fprintf(stderr, "Erro fatal: nao foi possivel abrir o arquivo.\n");
        return 1;
    }

    // Construção dos índices da BST
    printf("\nConstruindo indice BST...\n");

    clock_gettime(CLOCK_MONOTONIC, &t0);
    arv *raiz = construirIndice(arq);
    clock_gettime(CLOCK_MONOTONIC, &t1);

    double tempo_construcao = (t1.tv_sec  - t0.tv_sec) +
                              (t1.tv_nsec - t0.tv_nsec) * 1e-9;
    printf("[main] Indice BST construido em %.6f s\n", tempo_construcao);

   // Gera as chaves de teste
    int chaves[N_BUSCAS];
    gerar_chaves_teste(chaves, N_BUSCAS, N_REGISTROS);

    printf("\n[main] Chaves de teste:\n  ");
    for (int i = 0; i < N_BUSCAS; i++) {
        printf("%d", chaves[i]);
        if (i < N_BUSCAS - 1) printf(", ");
    }
    printf("\n");

// Para debug no terminal
#ifdef DEBUG_INORDER
    printf("\n[DEBUG] In-order BST:\n");
    bst_imprimir_inorder(raiz);
#endif

   // Questão 1
    double soma_bst = 0.0;
    {
        registro resultado;
        printf("\n--- Questao 1: Busca via BST ---\n");

        for (int i = 0; i < N_BUSCAS; i++) {
            clock_gettime(CLOCK_MONOTONIC, &t0);
            int ok = buscarNoArquivo(raiz, chaves[i], arq, &resultado);
            clock_gettime(CLOCK_MONOTONIC, &t1);

            double e = (t1.tv_sec  - t0.tv_sec) +
                       (t1.tv_nsec - t0.tv_nsec) * 1e-9;
            soma_bst += e;

            printf("  [BST] chave=%-6d %s  %.9f s\n",
                   chaves[i],
                   ok ? "encontrada   " : "NAO encontrada",
                   e);

        }
        printf("  Tempo medio BST: %.9f s\n", soma_bst / N_BUSCAS);
        printf("  Tempo total BST: %.6f s\n", soma_bst);
    }

    // Questão 3
    double soma_seq = 0.0;
    {
        registro resultado;
        printf("\n--- Questao 3: Busca Sequencial no arquivo ---\n");

        for (int i = 0; i < N_BUSCAS; i++) {
            clock_gettime(CLOCK_MONOTONIC, &t0);
            int ok = arquivo_busca_seq_chave(arq, chaves[i], &resultado);
            clock_gettime(CLOCK_MONOTONIC, &t1);

            double e = (t1.tv_sec  - t0.tv_sec) +
                       (t1.tv_nsec - t0.tv_nsec) * 1e-9;
            soma_seq += e;

            printf("  [SEQ] chave=%-6d %s  %.9f s\n",
                   chaves[i],
                   ok ? "encontrada   " : "NAO encontrada",
                   e);
        }
        printf("  Tempo medio SEQ: %.9f s\n", soma_seq / N_BUSCAS);
        printf("  Tempo total SEQ: %.6f s\n", soma_seq);
    }

    resultadoComparacao(soma_bst / N_BUSCAS, soma_seq / N_BUSCAS);

    raiz = podaArv(raiz);
    arquivo_fechar(arq);

    return 0;
}

// Para compilar:
// gcc -Wall -Wextra -O2 -std=c11 -D_POSIX_C_SOURCE=199309L -o questao1_e_3 questao1_e_3.c registro.c TAD_arv.c

// Basta apagar o bin para recompilar. POR FAVOR APAGA ELE, caso contrário vai
// distorcer os tempos de busca.