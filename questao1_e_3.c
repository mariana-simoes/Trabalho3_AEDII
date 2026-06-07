#define _POSIX_C_SOURCE 199309L // Evita um erro de compilação, de acordo com o Claudekkkk n sei o que é
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "registro.h"
#include "TAD_arv.h"
#include "estatisticas.h"

#define ARQUIVO_DADOS  "dados.bin"
#define N_REGISTROS    100000   // Recomendo colocar 100 pra debugging, 100000 para teste final.
#define N_BUSCAS       30       /* quantidade de buscas para medir tempo */


// alterado para o uso das estatísticas.
static void resultadoComparacao(double tempos_bst[], double tempos_seq[])
{
    double med_bst = media(tempos_bst, N_BUSCAS);
    double med_seq = media(tempos_seq, N_BUSCAS);
    double dp_bst  = desvioPadrao(tempos_bst, N_BUSCAS, med_bst);
    double dp_seq  = desvioPadrao(tempos_seq, N_BUSCAS, med_seq);
 
    printf("\n========================================\n");
    printf("   RESULTADO COMPARATIVO (Q1 vs Q3)\n");
    printf("========================================\n");
    printf("  BST — media: %.9f s  desvio: %.9f s\n", med_bst, dp_bst);
    printf("  SEQ — media: %.9f s  desvio: %.9f s\n", med_seq, dp_seq);
    if (med_bst > 0)
        printf("  Sequencial / BST: %.1fx mais lenta\n", med_seq / med_bst);
    printf("========================================\n");
}

int main()

{

    FILE *arq = fopen(ARQUIVO_DADOS, "rb");
    if (arq == NULL) {
        printf("Arquivo nao encontrado. Gerando %d registros...\n", N_REGISTROS);
        arquivo_gerar(ARQUIVO_DADOS, N_REGISTROS);
        arq = arquivo_abrir(ARQUIVO_DADOS, "rb");
    } else {
        printf("Arquivo '%s' aberto. Registros: %ld\n", ARQUIVO_DADOS, arquivo_total_registros(arq));
    }

    if (arq == NULL) {
        fprintf(stderr, "Erro fatal: nao foi possivel abrir o arquivo.\n");
        return 1;
    }

    // Construção dos índices da BST
    printf("\nConstruindo indice BST...\n");
    double t0 = tempoAtual();
    arv *raiz = construirIndice(arq);
    printf("Indice construido em %.6f s\n", tempoAtual() - t0);

   // Gera as chaves de teste
    int chaves[N_BUSCAS];
    gerar_chaves(chaves, N_BUSCAS, N_REGISTROS);

    printf("\n Chaves de teste:\n  ");
    for (int i = 0; i < N_BUSCAS; i++) {
        printf("%d", chaves[i]);
        if (i < N_BUSCAS - 1) printf(", ");
    }
    printf("\n");

// Para debug no terminal
#ifdef DEBUG_INORDER
    printf("\n[DEBUG] Em-ordem BST:\n");
    central(raiz);
    printf("\n");
#endif

   // Questão 1
    double tempos_bst[N_BUSCAS];
    {
        registro resultado;
        printf("\n--- Questao 1: Busca via BST ---\n");

        for (int i = 0; i < N_BUSCAS; i++) {
        double ini = tempoAtual();
        // Diz se encontrou ou não o registro (bool)
        int ok = buscarNoArquivo(raiz, chaves[i], arq, &resultado);
        tempos_bst[i] = tempoAtual() - ini;

            printf("[BST] chave=%-6d %s  %.9f s\n", chaves[i], ok ? "encontrada   " : "NAO encontrada", tempos_bst[i]);

        }
        printf("  Tempo medio BST: %.9f s\n", media(tempos_bst, N_BUSCAS));
        printf("  Tempo total BST: %.6f s\n", soma(tempos_bst, N_BUSCAS));
    }

    // Questão 3
    double tempos_seq[N_BUSCAS];
    {
        registro resultado;
        printf("\n--- Questao 3: Busca Sequencial no arquivo ---\n");

        for (int i = 0; i < N_BUSCAS; i++) {
            double ini = tempoAtual();
            int ok = arquivo_busca_seq_chave(arq, chaves[i], &resultado);
            tempos_seq[i] = tempoAtual() - ini;

            printf("[SEQ] chave=%-6d %s  %.9f s\n", chaves[i], ok ? "encontrada   " : "NAO encontrada", tempos_seq[i]);
        }
        printf("Tempo medio SEQ: %.9f s\n", media(tempos_seq, N_BUSCAS));
        printf("Tempo total SEQ: %.6f s\n", soma(tempos_seq, N_BUSCAS));
    }

    resultadoComparacao(tempos_bst, tempos_seq);

    raiz = podaArv(raiz);
    arquivo_fechar(arq);

    return 0;
}

// Para compilar:
// gcc -Wall -Wextra -O2 -std=c11 -D_POSIX_C_SOURCE=199309L -o questao1_e_3 questao1_e_3.c registro.c TAD_arv.c estatisticas.c -lm

// Debug (mostra in-order da BST):
// gcc -Wall -Wextra -O2 -std=c11 -D_POSIX_C_SOURCE=199309L -DDEBUG_INORDER -o questao1_e_3 questao1_e_3.c registro.c TAD_arv.c estatisticas.c -lm
// e depois ./questao1_e_3

// apaguem o dados.bin antes de compilar novamente.