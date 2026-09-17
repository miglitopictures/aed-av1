/*
 * ==========================================================================
 *  P07 — PILHAS | Ordenar uma pilha usando UMA pilha auxiliar
 * --------------------------------------------------------------------------
 *  DESAFIO
 *  Ordene a pilha de forma que o MAIOR valor fique no TOPO
 *  (ou seja, lida da base para o topo, fica em ordem crescente).
 *
 *  Exemplo (base -> topo): [3, 1, 4, 1, 5]  ->  [1, 1, 3, 4, 5]
 *
 *  Restrições:
 *   - use apenas UMA pilha auxiliar + variáveis inteiras simples
 *     (nada de vetores);
 *   - use só as operações da pilha (empilhar, desempilhar, consultarTopo,
 *     pilhaVazia, criarPilha, liberarPilha);
 *   - o resultado fica na própria pilha p; libere a auxiliar.
 *
 *  Dica: repita enquanto p não estiver vazia: desempilhe um valor 'tmp'
 *  de p; enquanto o topo da auxiliar for MENOR que tmp, devolva esse topo
 *  para p; então empilhe tmp na auxiliar. Assim a auxiliar fica com o
 *  MENOR no topo. No final, o que falta para o MAIOR ficar no topo de p?
 *
 *  ASSINATURA
 *      void ordenarPilha(Pilha *p);
 *
 *  COMO TESTAR
 *      ./testar.sh P07
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
void ordenarPilha(Pilha *p) {
    /* TODO: implemente aqui */
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
/* ---------- auxiliares de teste para pilhas ----------
 * Nos testes, as pilhas são escritas da BASE para o TOPO:
 *     [1, 2, 3]  ->  1 está na base, 3 está no topo
 */
Pilha *pilhaDe(const int *v, int n) {
    Pilha *p = (Pilha *) malloc(sizeof(Pilha));
    p->topo = NULL;
    p->tamanho = 0;
    for (int i = 0; i < n; i++) {
        NoPilha *no = (NoPilha *) malloc(sizeof(NoPilha));
        no->valor = v[i];
        no->prox = p->topo;
        p->topo = no;
        p->tamanho++;
    }
    return p;
}

/* lê a pilha (sem alterar) em ordem base -> topo; retorna a quantidade */
int pilhaParaVetor(Pilha *p, int *v) {
    int n = 0;
    for (NoPilha *no = p->topo; no != NULL && n < LIMITE_NOS; no = no->prox) v[n++] = no->valor;
    for (int i = 0; i < n / 2; i++) { int t = v[i]; v[i] = v[n - 1 - i]; v[n - 1 - i] = t; }
    return n;
}

void checarPilha(Pilha *p, const int *esperado, int ne, const char *descricao) {
    static int buf[LIMITE_NOS];
    if (p == NULL) {
        verificar(0, descricao);
        printf("      a pilha é NULL\n");
        return;
    }
    int n = pilhaParaVetor(p, buf);
    char desc2[300];
    snprintf(desc2, sizeof desc2, "%s  (base -> topo)", descricao);
    checarVetor(buf, n, esperado, ne, desc2);
    if (!verificar(p->tamanho == ne, "   campo tamanho correto"))
        printf("      esperado: %d, obtido: %d\n", ne, p->tamanho);
}

void liberarPilhaTeste(Pilha *p) {
    if (p == NULL) return;
    NoPilha *no = p->topo;
    int n = 0;
    while (no != NULL && n++ < LIMITE_NOS) { NoPilha *t = no->prox; free(no); no = t; }
    free(p);
}

/* 1 se alguma das pilhas compartilha nós com a outra */
int pilhasCompartilham(Pilha *a, Pilha *b) {
    for (NoPilha *x = a->topo; x != NULL; x = x->prox)
        for (NoPilha *y = b->topo; y != NULL; y = y->prox)
            if (x == y) return 1;
    return 0;
}

void caso(const int *v, int n, const int *esp, const char *desc) {
    Pilha *p = pilhaDe(v, n);
    ordenarPilha(p);
    checarPilha(p, esp, n, desc);
    liberarPilhaTeste(p);
}

int main(void) {
    printf("P07 — ordenarPilha\n");

    secao("Casos triviais");
    caso(NULL, 0, NULL, "pilha vazia continua vazia");
    caso((int[]){4}, 1, (int[]){4}, "[4] -> [4]");

    secao("Casos gerais");
    caso((int[]){3, 1, 4, 1, 5}, 5, (int[]){1, 1, 3, 4, 5}, "[3, 1, 4, 1, 5] -> [1, 1, 3, 4, 5]");
    caso((int[]){34, 3, 31, 98, 92, 23}, 6, (int[]){3, 23, 31, 34, 92, 98}, "[34, 3, 31, 98, 92, 23] -> [3, 23, 31, 34, 92, 98]");
    caso((int[]){9, 7, 5, 3, 1}, 5, (int[]){1, 3, 5, 7, 9}, "[9, 7, 5, 3, 1] -> [1, 3, 5, 7, 9]");
    caso((int[]){1, 2, 3}, 3, (int[]){1, 2, 3}, "[1, 2, 3] já ordenada");
    caso((int[]){-2, 10, 0, -7}, 4, (int[]){-7, -2, 0, 10}, "com negativos [-2, 10, 0, -7] -> [-7, -2, 0, 10]");

    return resumo();
}
