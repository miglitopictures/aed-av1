/*
 * ==========================================================================
 *  P10 — PILHAS | Rastreio de operações (questão de papel)
 * --------------------------------------------------------------------------
 *  DESAFIO
 *  Uma pilha começa vazia e executa, nesta ordem:
 *
 *     1) empilhar(5)       8) empilhar(9)
 *     2) empilhar(8)       9) empilhar(1)
 *     3) desempilhar()    10) empilhar(4)
 *     4) empilhar(3)      11) desempilhar()
 *     5) empilhar(7)      12) empilhar(6)
 *     6) desempilhar()    13) desempilhar()
 *     7) desempilhar()    14) desempilhar()
 *
 *  a) Qual a sequência de valores removidos (na ordem em que saem)?
 *  b) Qual o conteúdo final da pilha, da BASE para o TOPO?
 *
 *  Faça no papel primeiro (desenhe a pilha a cada passo) e só depois
 *  preencha os vetores abaixo.
 *
 *  COMO TESTAR
 *      ./testar.sh P10
 *  Para ver o gabarito das que errou, mude MOSTRAR_GABARITO para 1.
 * ==========================================================================
 */
#include <stdio.h>
#include <stdlib.h>

#define MOSTRAR_GABARITO 0

/* >>>>>>>>>>>>>>>>>>>>>>>>>> SUA SOLUÇÃO AQUI >>>>>>>>>>>>>>>>>>>>>>>>>> */
int removidos[]  = { 0 };   /* a) ex.: { 8, ... } */
int pilhaFinal[] = { 0 };   /* b) base -> topo    */
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

/* simulação usada para calcular o gabarito (0 = desempilhar) */
const int OPS[] = {5, 8, 0, 3, 7, 0, 0, 9, 1, 4, 0, 6, 0, 0};

void conferir(const int *sua, int ns, const int *certo, int nc, const char *desc) {
    if (!verificar(vetoresIguais(sua, ns, certo, nc), desc)) {
        printf("      sua resposta: "); imprimirVetor(sua, ns); printf("\n");
        if (MOSTRAR_GABARITO) { printf("      gabarito:     "); imprimirVetor(certo, nc); printf("\n"); }
    }
}

int main(void) {
    printf("P10 — rastreio de pilha\n");
    int pilha[32], topo = -1, rem[32], nrem = 0;
    for (int i = 0; i < TAM(OPS); i++) {
        if (OPS[i] != 0) pilha[++topo] = OPS[i];
        else rem[nrem++] = pilha[topo--];
    }

    secao("Respostas");
    conferir(removidos, TAM(removidos), rem, nrem, "a) sequência de removidos");
    conferir(pilhaFinal, TAM(pilhaFinal), pilha, topo + 1, "b) conteúdo final (base -> topo)");

    return resumo();
}
