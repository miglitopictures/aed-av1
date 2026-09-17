/*
 * ==========================================================================
 *  P05 — TEÓRICA | Verdadeiro ou Falso           (Simulado 2 — Q03, ampliada)
 * --------------------------------------------------------------------------
 *  DESAFIO
 *  Julgue cada afirmação como Verdadeira (V) ou Falsa (F). Preencha a
 *  string 'respostas' abaixo com 12 letras, na ordem (ex.: "VFVV...").
 *  Para as FALSAS, tente justificar mentalmente o porquê — na prova o
 *  raciocínio conta.
 *
 *   1) Em uma pilha, o primeiro elemento inserido é sempre o primeiro a ser
 *      removido (FIFO).
 *   2) Em uma fila, novos elementos são inseridos no fim e removidos do
 *      início (FIFO).
 *   3) Em uma lista simplesmente encadeada (com ponteiro só para o início),
 *      remover o primeiro elemento é mais rápido do que remover o último.
 *   4) Tanto filas quanto pilhas podem ser implementadas usando listas
 *      simplesmente encadeadas.
 *   5) Em uma fila encadeada com ponteiros para início e fim, enfileirar e
 *      desenfileirar podem ser feitos em tempo constante O(1).
 *   6) O percurso em-ordem (in-order) de uma árvore binária de busca
 *      visita os valores em ordem crescente.
 *   7) Em uma árvore binária de busca, o menor valor está sempre em uma
 *      folha.
 *   8) Ao remover de uma ABB um nó com dois filhos usando o "algoritmo da
 *      direita", o substituto é o MAIOR valor da subárvore esquerda.
 *   9) Uma pilha implementada com vetor de tamanho fixo pode sofrer
 *      overflow (estouro) ao empilhar.
 *  10) A busca em uma ABB é sempre O(log n), independentemente da ordem em
 *      que os valores foram inseridos.
 *  11) Em uma lista duplamente encadeada, cada nó guarda um ponteiro para o
 *      anterior e outro para o próximo.
 *  12) Para verificar se uma árvore binária é ABB, basta checar, em cada
 *      nó, se filho esquerdo < nó < filho direito.
 *
 *  COMO TESTAR
 *      ./testar.sh P05
 *  Para ver o gabarito das que errou, mude MOSTRAR_GABARITO para 1.
 * ==========================================================================
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MOSTRAR_GABARITO 0

/* >>>>>>>>>>>>>>>>>>>>>>>>>> SUA SOLUÇÃO AQUI >>>>>>>>>>>>>>>>>>>>>>>>>> */
const char *respostas = "????????????";   /* 12 letras: V ou F */
/* <<<<<<<<<<<<<<<<<<<<<<<<<< FIM DA SUA SOLUÇÃO <<<<<<<<<<<<<<<<<<<<<<<<<< */

/* ------------------------------------------------------------------
 * Mini framework de testes (não precisa mexer daqui para baixo)
 * ------------------------------------------------------------------ */
#define TAM(v) ((int)(sizeof(v) / sizeof((v)[0])))
#define LIMITE_NOS 10000

int testes_total = 0, testes_ok = 0;

int verificar(int condicao, const char *descricao) {
    testes_total++;
    if (condicao) {
        testes_ok++;
        printf("  ✓ %s\n", descricao);
    } else {
        printf("  ✗ %s\n", descricao);
    }
    return condicao;
}

void secao(const char *titulo) {
    printf("\n▶ %s\n", titulo);
}

void imprimirVetor(const int *v, int n) {
    printf("[");
    for (int i = 0; i < n; i++) printf(i ? ", %d" : "%d", v[i]);
    printf("]");
}

int vetoresIguais(const int *a, int na, const int *b, int nb) {
    if (na != nb) return 0;
    for (int i = 0; i < na; i++)
        if (a[i] != b[i]) return 0;
    return 1;
}

/* compara 'obtido' com 'esperado'; se falhar, mostra os dois */
void checarVetor(const int *obtido, int no, const int *esperado, int ne, const char *descricao) {
    if (!verificar(vetoresIguais(obtido, no, esperado, ne), descricao)) {
        printf("      esperado: "); imprimirVetor(esperado, ne);
        printf("\n      obtido:   "); imprimirVetor(obtido, no);
        printf("\n");
    }
}

#if defined(__SANITIZE_ADDRESS__)          /* compilado com -fsanitize=address */
#include <sanitizer/lsan_interface.h>
const char *__lsan_default_options(void) { return "leak_check_at_exit=0"; }
#endif

int resumo(void) {
#if defined(__SANITIZE_ADDRESS__)
    secao("Vazamento de memória (AddressSanitizer)");
    fflush(stdout);
    int vazou = __lsan_do_recoverable_leak_check();
    fflush(stderr);
    verificar(!vazou, vazou ? "vazamento detectado: veja o relatório acima (faltou algum free?)"
                            : "nenhum vazamento de memória");
#endif
    printf("\n────────────────────────────────────────\n");
    if (testes_ok == testes_total)
        printf("RESULTADO: %d/%d testes passaram ✓ ACERTOU!\n", testes_ok, testes_total);
    else
        printf("RESULTADO: %d/%d testes passaram ✗ revise sua solução.\n", testes_ok, testes_total);
    fflush(stdout);
    return testes_ok == testes_total ? 0 : 1;
}

/* gabarito "embaralhado" para você não ler sem querer */
const unsigned char G[12] = {0x6d, 0x64, 0x6f, 0x16, 0x11, 0x18, 0x13, 0x1a, 0x35, 0x2c, 0x27, 0x3e};

int main(void) {
    printf("P05 — Verdadeiro ou Falso\n");
    secao("Respostas");
    if (strlen(respostas) != 12) {
        verificar(0, "a string respostas deve ter exatamente 12 letras");
        return resumo();
    }
    for (int i = 0; i < 12; i++) {
        char certo = (char) (G[i] ^ ((0x2B + 7 * i) & 0xFF));
        char sua = (char) toupper((unsigned char) respostas[i]);
        char desc[80];
        snprintf(desc, sizeof desc, "afirmação %2d (sua resposta: %c)", i + 1, sua);
        if (!verificar(sua == certo, desc) && MOSTRAR_GABARITO)
            printf("      gabarito: %c\n", certo);
    }
    return resumo();
}
