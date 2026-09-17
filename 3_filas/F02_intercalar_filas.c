/*
 * ==========================================================================
 *  F02 — FILAS | Intercalar duas filas              (Simulado 2026.2 — Q01)
 * --------------------------------------------------------------------------
 *  DESAFIO
 *  Implemente uma função que receba duas filas de inteiros e crie uma
 *  TERCEIRA fila intercalando seus elementos. Comece pela primeira fila e,
 *  quando uma das filas terminar, continue inserindo os elementos
 *  restantes da outra.
 *
 *  AS FILAS ORIGINAIS NÃO PODEM SER ALTERADAS: ao final da função elas
 *  devem ter o mesmo conteúdo, na mesma ordem. (Se você desenfileirar
 *  delas, precisa recolocar tudo!)
 *
 *  Exemplo: F1 = [10, 30, 50] e F2 = [20, 40] -> [10, 20, 30, 40, 50]
 *
 *  - Duas filas vazias -> retorna uma fila NOVA e vazia.
 *  - F1 ou F2 == NULL  -> retorne NULL.
 *
 *  ASSINATURA
 *      Fila *intercalarFilas(Fila *F1, Fila *F2);
 *
 *  COMO TESTAR
 *      ./testar.sh F02
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

/* >>>>>>>>>>>>>>>>>>>>>>>>>> SUA SOLUÇÃO AQUI >>>>>>>>>>>>>>>>>>>>>>>>>> */
Fila *intercalarFilas(Fila *F1, Fila *F2) {
    /* TODO: implemente aqui */
    return NULL;
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

void caso(const int *a, int na, const int *b, int nb, const int *esp, int ne, const char *desc) {
    secao(desc);
    Fila *F1 = filaDe(a, na), *F2 = filaDe(b, nb);
    Fila *r = intercalarFilas(F1, F2);
    checarFila(r, esp, ne, "resultado");
    checarFila(F1, a, na, "F1 não foi alterada");
    checarFila(F2, b, nb, "F2 não foi alterada");
    if (r != NULL) {
        int indep = r != F1 && r != F2 && !filasCompartilham(r, F1) && !filasCompartilham(r, F2);
        verificar(indep, "resultado é uma fila nova (sem nós compartilhados)");
        if (indep) liberarFilaTeste(r);
    }
    liberarFilaTeste(F1);
    liberarFilaTeste(F2);
}

int main(void) {
    printf("F02 — intercalarFilas\n");

    caso((int[]){10, 30, 50}, 3, (int[]){20, 40}, 2,
         (int[]){10, 20, 30, 40, 50}, 5, "Enunciado: [10, 30, 50] e [20, 40]");
    caso((int[]){1}, 1, (int[]){2, 3, 4}, 3,
         (int[]){1, 2, 3, 4}, 4, "F2 maior: [1] e [2, 3, 4]");
    caso((int[]){1, 3}, 2, (int[]){2, 4}, 2,
         (int[]){1, 2, 3, 4}, 4, "Mesmo tamanho: [1, 3] e [2, 4]");
    caso(NULL, 0, (int[]){5, 6}, 2,
         (int[]){5, 6}, 2, "F1 vazia: [] e [5, 6]");
    caso((int[]){7, 8}, 2, NULL, 0,
         (int[]){7, 8}, 2, "F2 vazia: [7, 8] e []");
    caso(NULL, 0, NULL, 0, NULL, 0, "Ambas vazias");

    secao("Parâmetro NULL");
    Fila *f = filaDe((int[]){1}, 1);
    verificar(intercalarFilas(NULL, f) == NULL, "intercalarFilas(NULL, F2) retorna NULL");
    verificar(intercalarFilas(f, NULL) == NULL, "intercalarFilas(F1, NULL) retorna NULL");
    liberarFilaTeste(f);

    return resumo();
}
