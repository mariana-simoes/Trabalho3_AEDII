#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>

#include "registro.h"
#include "TAD_arv_ano.h"
#include "estatisticas.h"

#define ARQUIVO_DADOS "dados.bin"
#define N_REGISTROS   100000
#define N_BUSCAS      30

/* ================================================================
 * Questão 4 — Busca do tipo 'ano > X' usando a BST indexada pelo
 * atributo não-chave 'ano' (TAD_arv_ano).
 *
 * O índice é construído lendo o arquivo uma única vez: cada nó da
 * BST guarda um valor distinto de 'ano' e uma lista de structs
 * 'registro' com aquele ano (ver TAD_arv_ano.h / TAD_arv_ano.c).
 * Durante a busca nenhum acesso ao arquivo é feito — os dados já
 * estão na memória.
 *
 * Os mesmos 30 limiares são usados na Questão 5 (sequencial).
 * ================================================================ */

/*
 * Limiares: mesmos 30 valores usados na Questão 5.
 * O campo 'ano' varia de 2000 a 2024 no arquivo; usar 2000..2029
 * cobre desde consultas com muitos resultados até zero resultados.
 */
static const int LIMIARES[N_BUSCAS] = {
    2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009,
    2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019,
    2020, 2021, 2022, 2023, 2024, 2025, 2026, 2027, 2028, 2029
};

int main(void)
{
    FILE *arq = fopen(ARQUIVO_DADOS, "rb");
    if (arq == NULL) {
        printf("Arquivo nao encontrado. Gerando %d registros...\n", N_REGISTROS);
        arquivo_gerar(ARQUIVO_DADOS, N_REGISTROS);
        arq = arquivo_abrir(ARQUIVO_DADOS, "rb");
    } else {
        printf("Arquivo '%s' aberto. Registros: %ld\n",
               ARQUIVO_DADOS, arquivo_total_registros(arq));
    }
    if (arq == NULL) {
        fprintf(stderr, "Erro fatal: nao foi possivel abrir o arquivo.\n");
        return 1;
    }

    /* --- Construção do índice BST --- */
    printf("\nConstruindo indice BST para 'ano'...\n");
    double t0 = tempoAtual();
    arvAno *raiz = construirIndiceAno(arq);
    printf("Indice construido em %.6f s\n", tempoAtual() - t0);

#ifdef DEBUG_INORDER
    printf("\n[DEBUG] Indice em-ordem:\n");
    exibirIndiceAno(raiz);
    printf("\n");
#endif

    /* --- 30 buscas 'ano op limiar' via BST --- */
    double tempos[4][N_BUSCAS];
    const int operadores[4] = { OP_MAIOR, OP_MAIOR_IGUAL, OP_MENOR, OP_MENOR_IGUAL };
    const char *sinais[4] = { ">", ">=", "<", "<=" };

    printf("\n--- Questao 4: Busca 'ano op X' via BST ---\n");
    for (int op = 0; op < 4; op++) {
        printf("\nOperador '%s':\n", sinais[op]);
        for (int i = 0; i < N_BUSCAS; i++) {
            /* contarRangeAno: mesma poda da BST, sem malloc.
             * Mede apenas o tempo de travessia — base justa de
             * comparação com a busca sequencial da Questão 5,
             * que também só conta sem alocar memória.          */
                 double ini = tempoAtual();
                 resLista *res = buscarRangeAno(raiz, LIMIARES[i], operadores[op]);
                 int cnt = contarResLista(res);
                 tempos[op][i] = tempoAtual() - ini;
                 liberaResLista(res);

                 printf("[BST] ano %s %-4d : %6d registros  %.9f s\n",
                     sinais[op], LIMIARES[i], cnt, tempos[op][i]);
        }

        /* --- Estatísticas --- */
        double med = media(tempos[op], N_BUSCAS);
        double dp  = desvioPadrao(tempos[op], N_BUSCAS, med);

        printf("\n========================================\n");
        printf("   RESULTADO — Questao 4 (BST por 'ano' %s)\n", sinais[op]);
        printf("========================================\n");
        printf("  Media       : %.9f s\n", med);
        printf("  Desvio pad. : %.9f s\n", dp);
        printf("  Tempo total : %.6f s\n", soma(tempos[op], N_BUSCAS));
        printf("========================================\n");
    }

    podaArvAno(raiz);
    arquivo_fechar(arq);
    return 0;
}

/* Compilar:
   gcc -Wall -Wextra -O2 -std=c11 -D_POSIX_C_SOURCE=199309L
       -o questao4 questao4.c registro.c TAD_arv_ano.c estatisticas.c -lm

   Debug (mostra o indice em-ordem):
   gcc -Wall -Wextra -O2 -std=c11 -D_POSIX_C_SOURCE=199309L -DDEBUG_INORDER
       -o questao4 questao4.c registro.c TAD_arv_ano.c estatisticas.c -lm
*/
