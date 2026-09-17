/*
 * ==========================================================================
 *  P08 — PILHAS | Converter decimal para binário usando pilha
 * --------------------------------------------------------------------------
 *  DESAFIO
 *  Converta um inteiro n >= 0 para sua representação binária, escrevendo
 *  o resultado como string em 'saida' (o buffer tem espaço suficiente).
 *
 *  Algoritmo: enquanto n > 0, empilhe n % 2 e faça n = n / 2.
 *  Depois desempilhe tudo escrevendo '0' ou '1' em saida.
 *  Não esqueça do '\0' no final e do caso especial n == 0 -> "0".
 *  Libere a pilha.
 *
 *  Exemplos: 0 -> "0"   2 -> "10"   10 -> "1010"   255 -> "11111111"
 *
 *  ASSINATURA
 *      void decimalParaBinario(int n, char *saida);
 *
 *  COMO TESTAR
 *      ./testar.sh P08
 * ==========================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void decimalParaBinario(int n, char *saida) {
    /* TODO: implemente aqui */
    saida[0] = '\0';
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

void caso(int n, const char *esperado) {
    char saida[64];
    memset(saida, 'X', sizeof saida);
    saida[63] = '\0';
    decimalParaBinario(n, saida);
    char desc[120];
    snprintf(desc, sizeof desc, "%d -> \"%s\"", n, esperado);
    if (!verificar(strcmp(saida, esperado) == 0, desc)) printf("      obtido: \"%s\"\n", saida);
}

int main(void) {
    printf("P08 — decimalParaBinario\n");

    secao("Casos especiais");
    caso(0, "0");
    caso(1, "1");

    secao("Casos gerais");
    caso(2, "10");
    caso(5, "101");
    caso(10, "1010");
    caso(13, "1101");
    caso(255, "11111111");
    caso(256, "100000000");
    caso(1000, "1111101000");

    return resumo();
}
