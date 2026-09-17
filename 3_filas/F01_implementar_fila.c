/*
 * ==========================================================================
 *  F01 — FILAS | Implementar a fila encadeada (FIFO)
 * --------------------------------------------------------------------------
 *  DESAFIO
 *  Usando as structs Fila e NoFila abaixo (com ponteiros inicio E fim),
 *  implemente:
 *
 *    Fila *criarFila(void)
 *        aloca fila vazia (inicio = fim = NULL, tamanho = 0); NULL se falhar
 *    int filaVazia(Fila *f)
 *        1 se vazia (ou f == NULL)
 *    int enfileirar(Fila *f, int valor)
 *        insere no FIM em O(1); 1 ok / 0 se f == NULL ou malloc falhou
 *    int desenfileirar(Fila *f, int *valor)
 *        remove do INÍCIO, guarda em *valor, libera o nó, retorna 1;
 *        0 se vazia
 *    void liberarFila(Fila *f)
 *        libera todos os nós e a struct
 *
 *  ARMADILHA CLÁSSICA: ao remover o ÚLTIMO elemento, 'fim' também precisa
 *  virar NULL — senão o próximo enfileirar usa um nó já liberado.
 *
 *  COMO TESTAR
 *      ./testar.sh F01
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

/* >>>>>>>>>>>>>>>>>>>>>>>>>> SUA SOLUÇÃO AQUI >>>>>>>>>>>>>>>>>>>>>>>>>> */
Fila *criarFila(void) {
    /* TODO */
    return NULL;
}

int filaVazia(Fila *f) {
    /* TODO */
    return 0;
}

int enfileirar(Fila *f, int valor) {
    /* TODO */
    return 0;
}

int desenfileirar(Fila *f, int *valor) {
    /* TODO */
    return 0;
}

void liberarFila(Fila *f) {
    /* TODO */
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

int main(void) {
    printf("F01 — implementar fila\n");
    int v = 0;

    secao("criarFila");
    Fila *f = criarFila();
    if (!verificar(f != NULL, "criarFila retorna uma fila alocada")) return resumo();
    checarFila(f, NULL, 0, "fila nova vazia");
    verificar(filaVazia(f) == 1, "filaVazia retorna 1");

    secao("Operações em fila vazia / NULL");
    verificar(desenfileirar(f, &v) == 0, "desenfileirar de fila vazia retorna 0");
    verificar(enfileirar(NULL, 1) == 0, "enfileirar(NULL, 1) retorna 0");
    verificar(filaVazia(NULL) == 1, "filaVazia(NULL) retorna 1");

    secao("enfileirar 10, 20, 30");
    int r1 = enfileirar(f, 10), r2 = enfileirar(f, 20), r3 = enfileirar(f, 30);
    verificar(r1 == 1 && r2 == 1 && r3 == 1, "enfileirar retorna 1");
    checarFila(f, (int[]){10, 20, 30}, 3, "fila = [10, 20, 30]");
    verificar(filaVazia(f) == 0, "fila não está vazia");

    secao("desenfileirar segue FIFO");
    v = -1;
    verificar(desenfileirar(f, &v) == 1 && v == 10, "1º desenfileirar devolve 10");
    checarFila(f, (int[]){20, 30}, 2, "fila = [20, 30]");
    enfileirar(f, 40);
    checarFila(f, (int[]){20, 30, 40}, 3, "enfileirar 40 -> [20, 30, 40]");

    secao("Esvaziando a fila");
    int saida[3] = {0}, n = 0;
    while (n < 3 && desenfileirar(f, &v)) saida[n++] = v;
    checarVetor(saida, n, (int[]){20, 30, 40}, 3, "saem 20, 30, 40");
    checarFila(f, NULL, 0, "fila vazia (inicio e fim NULL)");
    verificar(desenfileirar(f, &v) == 0, "desenfileirar de novo retorna 0");

    secao("Reutilizando a fila depois de esvaziar");
    enfileirar(f, 99);
    checarFila(f, (int[]){99}, 1, "enfileirar 99 -> [99]");
    enfileirar(f, 100);
    checarFila(f, (int[]){99, 100}, 2, "enfileirar 100 -> [99, 100]");

    secao("liberarFila");
    liberarFila(f);
    verificar(1, "liberarFila executou (o teste de vazamento abaixo confere os free)");

    return resumo();
}
