/*
 * ==========================================================================
 *  P02 — PILHAS | Parênteses, colchetes e chaves balanceados
 * --------------------------------------------------------------------------
 *  DESAFIO
 *  Usando a pilha de caracteres (PilhaChar, já pronta abaixo), implemente
 *  uma função que verifique se os delimitadores ( ) [ ] { } de uma
 *  expressão estão balanceados e corretamente aninhados.
 *
 *  - Outros caracteres (letras, números, operadores, espaços) são ignorados.
 *  - Retorne 1 se balanceada, 0 caso contrário.
 *  - String vazia é balanceada.
 *  - Libere a pilha antes de retornar (em TODOS os caminhos de saída!).
 *
 *  Exemplos:  "{[()]}" -> 1     "([)]" -> 0     "((" -> 0     "())" -> 0
 *
 *  ASSINATURA
 *      int balanceada(const char *expr);
 *
 *  COMO TESTAR
 *      ./testar.sh P02
 * ==========================================================================
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct NoChar {
    char c;
    struct NoChar *prox;
} NoChar;

typedef struct {
    NoChar *topo;
} PilhaChar;

/* ---------- operações JÁ PRONTAS ----------
 *   PilhaChar *criarPilhaChar(void)
 *   int  pilhaCharVazia(PilhaChar *p)              -> 1 se vazia
 *   int  empilharChar(PilhaChar *p, char c)        -> 1 ok / 0 falha
 *   int  desempilharChar(PilhaChar *p, char *c)    -> 1 ok / 0 se vazia
 *   void liberarPilhaChar(PilhaChar *p)
 */
PilhaChar *criarPilhaChar(void) {
    PilhaChar *p = (PilhaChar *) malloc(sizeof(PilhaChar));
    if (p != NULL) p->topo = NULL;
    return p;
}

int pilhaCharVazia(PilhaChar *p) {
    return p == NULL || p->topo == NULL;
}

int empilharChar(PilhaChar *p, char c) {
    if (p == NULL) return 0;
    NoChar *no = (NoChar *) malloc(sizeof(NoChar));
    if (no == NULL) return 0;
    no->c = c;
    no->prox = p->topo;
    p->topo = no;
    return 1;
}

int desempilharChar(PilhaChar *p, char *c) {
    if (pilhaCharVazia(p)) return 0;
    NoChar *no = p->topo;
    if (c != NULL) *c = no->c;
    p->topo = no->prox;
    free(no);
    return 1;
}

void liberarPilhaChar(PilhaChar *p) {
    if (p == NULL) return;
    while (desempilharChar(p, NULL)) {}
    free(p);
}

/* >>>>>>>>>>>>>>>>>>>>>>>>>> SUA SOLUÇÃO AQUI >>>>>>>>>>>>>>>>>>>>>>>>>> */
int balanceada(const char *expr) {
    /* TODO: implemente aqui */
    return -1;
}
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

void caso(const char *expr, int esperado) {
    char desc[200];
    snprintf(desc, sizeof desc, "\"%s\" -> %d", expr, esperado);
    int r = balanceada(expr);
    if (!verificar(r == esperado, desc)) printf("      obtido: %d\n", r);
}

int main(void) {
    printf("P02 — balanceada\n");

    secao("Balanceadas");
    caso("", 1);
    caso("()", 1);
    caso("(a + b) * c", 1);
    caso("{[()]}", 1);
    caso("a[b{c}d]e(f)", 1);
    caso("{[(2 + 3) * (4 - 1)] / [5]}", 1);

    secao("Não balanceadas");
    caso("([)]", 0);
    caso("((", 0);
    caso("())", 0);
    caso(")(", 0);
    caso("{", 0);
    caso("]", 0);
    caso("(a + b]", 0);
    caso("{[}]", 0);

    return resumo();
}
