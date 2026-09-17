/*
 * ==========================================================================
 *  F03 — FILAS + PILHAS | Inverter uma fila usando uma pilha
 * --------------------------------------------------------------------------
 *  DESAFIO
 *  Inverta a ordem dos elementos de uma fila usando UMA pilha auxiliar.
 *  O resultado fica na própria fila recebida.
 *
 *  Exemplo (início -> fim): [1, 2, 3, 4]  ->  [4, 3, 2, 1]
 *
 *  - Use apenas as operações prontas de fila e pilha
 *    (não mexa em ->inicio, ->fim, ->topo, ->prox diretamente).
 *  - f == NULL ou vazia: não faça nada.
 *  - Libere a pilha auxiliar.
 *
 *  ASSINATURA
 *      void inverterFila(Fila *f);
 *
 *  COMO TESTAR
 *      ./testar.sh F03
 * ==========================================================================
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct NoFila {
    int valor;
    struct NoFila *prox;       /* próximo a ser atendido depois deste */
} NoFila;

typedef struct {
    NoFila *inicio;            /* de onde se REMOVE (NULL se vazia) */
    NoFila *fim;               /* onde se INSERE   (NULL se vazia) */
    int tamanho;
} Fila;
/* ---------- operações de fila JÁ PRONTAS (use à vontade) ----------
 *   Fila *criarFila(void)                        -> fila vazia (NULL se falhar)
 *   int   filaVazia(Fila *f)                     -> 1 se vazia
 *   int   enfileirar(Fila *f, int valor)         -> 1 ok / 0 falha   (insere no fim)
 *   int   desenfileirar(Fila *f, int *valor)     -> 1 ok / 0 se vazia (remove do início)
 *   void  liberarFila(Fila *f)                   -> libera nós e a struct
 */
Fila *criarFila(void) {
    Fila *f = (Fila *) malloc(sizeof(Fila));
    if (f == NULL) return NULL;
    f->inicio = f->fim = NULL;
    f->tamanho = 0;
    return f;
}

int filaVazia(Fila *f) {
    return f == NULL || f->inicio == NULL;
}

int enfileirar(Fila *f, int valor) {
    if (f == NULL) return 0;
    NoFila *no = (NoFila *) malloc(sizeof(NoFila));
    if (no == NULL) return 0;
    no->valor = valor;
    no->prox = NULL;
    if (f->fim == NULL) f->inicio = no;
    else f->fim->prox = no;
    f->fim = no;
    f->tamanho++;
    return 1;
}

int desenfileirar(Fila *f, int *valor) {
    if (filaVazia(f)) return 0;
    NoFila *no = f->inicio;
    if (valor != NULL) *valor = no->valor;
    f->inicio = no->prox;
    if (f->inicio == NULL) f->fim = NULL;
    f->tamanho--;
    free(no);
    return 1;
}

void liberarFila(Fila *f) {
    if (f == NULL) return;
    while (desenfileirar(f, NULL)) {}
    free(f);
}
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
void inverterFila(Fila *f) {
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
/* ---------- auxiliares de teste para filas ----------
 * Nos testes, as filas são escritas do INÍCIO para o FIM:
 *     [1, 2, 3]  ->  1 é o próximo a sair, 3 foi o último a entrar
 */
Fila *filaDe(const int *v, int n) {
    Fila *f = (Fila *) malloc(sizeof(Fila));
    f->inicio = f->fim = NULL;
    f->tamanho = 0;
    for (int i = 0; i < n; i++) {
        NoFila *no = (NoFila *) malloc(sizeof(NoFila));
        no->valor = v[i];
        no->prox = NULL;
        if (f->fim == NULL) f->inicio = no; else f->fim->prox = no;
        f->fim = no;
        f->tamanho++;
    }
    return f;
}

/* confere valores, tamanho e o ponteiro fim */
void checarFila(Fila *f, const int *esperado, int ne, const char *descricao) {
    static int buf[LIMITE_NOS];
    if (f == NULL) {
        verificar(0, descricao);
        printf("      a fila é NULL\n");
        return;
    }
    int n = 0;
    NoFila *ultimo = NULL;
    for (NoFila *no = f->inicio; no != NULL && n < LIMITE_NOS; no = no->prox) {
        buf[n++] = no->valor;
        ultimo = no;
    }
    char desc2[300];
    snprintf(desc2, sizeof desc2, "%s  (início -> fim)", descricao);
    checarVetor(buf, n, esperado, ne, desc2);
    if (!verificar(f->tamanho == ne, "   campo tamanho correto"))
        printf("      esperado: %d, obtido: %d\n", ne, f->tamanho);
    if (!verificar(f->fim == ultimo, "   ponteiro fim aponta para o último nó (NULL se vazia)"))
        printf("      fim está %s\n", f->fim == NULL ? "NULL" : "apontando para outro nó");
}

void liberarFilaTeste(Fila *f) {
    if (f == NULL) return;
    NoFila *no = f->inicio;
    int n = 0;
    while (no != NULL && n++ < LIMITE_NOS) { NoFila *t = no->prox; free(no); no = t; }
    free(f);
}

/* 1 se as filas compartilham algum nó */
int filasCompartilham(Fila *a, Fila *b) {
    for (NoFila *x = a->inicio; x != NULL; x = x->prox)
        for (NoFila *y = b->inicio; y != NULL; y = y->prox)
            if (x == y) return 1;
    return 0;
}

void caso(const int *v, int n, const int *esp, const char *desc) {
    Fila *f = filaDe(v, n);
    inverterFila(f);
    checarFila(f, esp, n, desc);
    liberarFilaTeste(f);
}

int main(void) {
    printf("F03 — inverterFila\n");

    secao("NULL e vazia");
    inverterFila(NULL);
    verificar(1, "inverterFila(NULL) não quebra");
    caso(NULL, 0, NULL, "fila vazia continua vazia");

    secao("Casos");
    caso((int[]){1, 2, 3, 4}, 4, (int[]){4, 3, 2, 1}, "[1, 2, 3, 4] -> [4, 3, 2, 1]");
    caso((int[]){7}, 1, (int[]){7}, "[7] -> [7]");
    caso((int[]){5, 9}, 2, (int[]){9, 5}, "[5, 9] -> [9, 5]");
    caso((int[]){3, 3, 1}, 3, (int[]){1, 3, 3}, "[3, 3, 1] -> [1, 3, 3]");

    secao("A fila continua funcionando depois de invertida");
    Fila *f = filaDe((int[]){1, 2, 3}, 3);
    inverterFila(f);
    enfileirar(f, 10);
    checarFila(f, (int[]){3, 2, 1, 10}, 4, "inverter [1, 2, 3] e enfileirar 10 -> [3, 2, 1, 10]");
    liberarFilaTeste(f);

    return resumo();
}
