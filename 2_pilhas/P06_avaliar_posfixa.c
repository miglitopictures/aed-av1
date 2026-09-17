/*
 * ==========================================================================
 *  P06 — PILHAS | Avaliar expressão pós-fixa (notação polonesa reversa)
 * --------------------------------------------------------------------------
 *  DESAFIO
 *  Usando a pilha de inteiros (já pronta), avalie uma expressão pós-fixa
 *  e retorne o resultado.
 *
 *  Regras da entrada (sempre VÁLIDA):
 *   - operandos são dígitos únicos '0'..'9'
 *   - operadores: +  -  *  /   (divisão inteira)
 *   - espaços devem ser ignorados
 *
 *  Algoritmo: dígito -> empilha; operador -> desempilha DOIS valores,
 *  calcula e empilha o resultado. ATENÇÃO À ORDEM: o primeiro desempilhado
 *  é o operando da DIREITA.  "52-" = 5 - 2 = 3  (e não 2 - 5).
 *
 *  Exemplo: "23*5+"  ->  (2*3)+5 = 11
 *  Libere a pilha antes de retornar.
 *
 *  ASSINATURA
 *      int avaliarPosfixa(const char *expr);
 *
 *  COMO TESTAR
 *      ./testar.sh P06
 * ==========================================================================
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct NoPilha {
    int valor;
    struct NoPilha *prox;      /* nó logo abaixo */
} NoPilha;

typedef struct {
    NoPilha *topo;             /* NULL quando vazia */
    int tamanho;
} Pilha;
/* ---------- operações de pilha JÁ PRONTAS (use à vontade) ----------
 *   Pilha *criarPilha(void)                      -> pilha vazia (NULL se falhar)
 *   int    pilhaVazia(Pilha *p)                  -> 1 se vazia
 *   int    empilhar(Pilha *p, int valor)         -> 1 ok / 0 falha
 *   int    desempilhar(Pilha *p, int *valor)     -> 1 ok / 0 se vazia
 *   int    consultarTopo(Pilha *p, int *valor)   -> 1 ok / 0 se vazia
 *   void   liberarPilha(Pilha *p)                -> libera nós e a struct
 */
Pilha *criarPilha(void) {
    Pilha *p = (Pilha *) malloc(sizeof(Pilha));
    if (p == NULL) return NULL;
    p->topo = NULL;
    p->tamanho = 0;
    return p;
}

int pilhaVazia(Pilha *p) {
    return p == NULL || p->topo == NULL;
}

int empilhar(Pilha *p, int valor) {
    if (p == NULL) return 0;
    NoPilha *no = (NoPilha *) malloc(sizeof(NoPilha));
    if (no == NULL) return 0;
    no->valor = valor;
    no->prox = p->topo;
    p->topo = no;
    p->tamanho++;
    return 1;
}

int desempilhar(Pilha *p, int *valor) {
    if (pilhaVazia(p)) return 0;
    NoPilha *no = p->topo;
    if (valor != NULL) *valor = no->valor;
    p->topo = no->prox;
    p->tamanho--;
    free(no);
    return 1;
}

int consultarTopo(Pilha *p, int *valor) {
    if (pilhaVazia(p)) return 0;
    if (valor != NULL) *valor = p->topo->valor;
    return 1;
}

void liberarPilha(Pilha *p) {
    if (p == NULL) return;
    while (desempilhar(p, NULL)) {}
    free(p);
}

/* >>>>>>>>>>>>>>>>>>>>>>>>>> SUA SOLUÇÃO AQUI >>>>>>>>>>>>>>>>>>>>>>>>>> */
int avaliarPosfixa(const char *expr) {
    /* TODO: implemente aqui */
    return -999;
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

void caso(const char *expr, int esperado, const char *infixa) {
    char desc[200];
    snprintf(desc, sizeof desc, "\"%s\"  (%s) = %d", expr, infixa, esperado);
    int r = avaliarPosfixa(expr);
    if (!verificar(r == esperado, desc)) printf("      obtido: %d\n", r);
}

int main(void) {
    printf("P06 — avaliarPosfixa\n");

    secao("Operações simples");
    caso("5", 5, "5");
    caso("23+", 5, "2 + 3");
    caso("34*", 12, "3 * 4");

    secao("Ordem dos operandos");
    caso("52-", 3, "5 - 2");
    caso("25-", -3, "2 - 5");
    caso("93/", 3, "9 / 3");
    caso("82/3-", 1, "8 / 2 - 3");

    secao("Expressões maiores");
    caso("23*5+", 11, "2 * 3 + 5");
    caso("2 3 4 * +", 14, "2 + 3 * 4");
    caso("34+2*7-", 7, "(3 + 4) * 2 - 7");
    caso("98-76-*", 1, "(9 - 8) * (7 - 6)");
    caso("123+*45-/", -5, "(1 * (2 + 3)) / (4 - 5)");

    return resumo();
}
