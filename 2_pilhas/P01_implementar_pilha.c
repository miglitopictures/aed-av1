/*
 * ==========================================================================
 *  P01 — PILHAS | Implementar a pilha encadeada (LIFO)
 * --------------------------------------------------------------------------
 *  DESAFIO
 *  Usando as structs Pilha e NoPilha abaixo, implemente as operações:
 *
 *    Pilha *criarPilha(void)
 *        aloca uma pilha vazia (topo = NULL, tamanho = 0); NULL se falhar
 *    int pilhaVazia(Pilha *p)
 *        1 se a pilha está vazia (ou p == NULL), 0 caso contrário
 *    int empilhar(Pilha *p, int valor)            (push)
 *        insere no topo; retorna 1 se deu certo, 0 se p == NULL ou malloc falhou
 *    int desempilhar(Pilha *p, int *valor)        (pop)
 *        remove o topo, guarda o valor em *valor, LIBERA o nó e retorna 1;
 *        retorna 0 se a pilha estiver vazia
 *    int consultarTopo(Pilha *p, int *valor)      (peek)
 *        guarda o valor do topo em *valor SEM remover; 0 se vazia
 *    void liberarPilha(Pilha *p)
 *        libera todos os nós e a própria struct
 *
 *  Mantenha o campo 'tamanho' sempre correto.
 *  Todas devem ser O(1), exceto liberarPilha.
 *
 *  COMO TESTAR
 *      ./testar.sh P01
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

/* >>>>>>>>>>>>>>>>>>>>>>>>>> SUA SOLUÇÃO AQUI >>>>>>>>>>>>>>>>>>>>>>>>>> */
Pilha *criarPilha(void) {
    /* TODO */
    return NULL;
}

int pilhaVazia(Pilha *p) {
    /* TODO */
    return 0;
}

int empilhar(Pilha *p, int valor) {
    /* TODO */
    return 0;
}

int desempilhar(Pilha *p, int *valor) {
    /* TODO */
    return 0;
}

int consultarTopo(Pilha *p, int *valor) {
    /* TODO */
    return 0;
}

void liberarPilha(Pilha *p) {
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

int main(void) {
    printf("P01 — implementar pilha\n");
    int v = 0;

    secao("criarPilha");
    Pilha *p = criarPilha();
    if (!verificar(p != NULL, "criarPilha retorna uma pilha alocada")) return resumo();
    verificar(p->topo == NULL && p->tamanho == 0, "pilha nova: topo == NULL e tamanho == 0");
    verificar(pilhaVazia(p) == 1, "pilha nova está vazia");

    secao("Operações em pilha vazia");
    v = -1;
    verificar(desempilhar(p, &v) == 0, "desempilhar de pilha vazia retorna 0");
    verificar(consultarTopo(p, &v) == 0, "consultarTopo em pilha vazia retorna 0");
    verificar(empilhar(NULL, 1) == 0, "empilhar(NULL, 1) retorna 0");
    verificar(pilhaVazia(NULL) == 1, "pilhaVazia(NULL) retorna 1");

    secao("empilhar 10, 20, 30");
    int r1 = empilhar(p, 10), r2 = empilhar(p, 20), r3 = empilhar(p, 30);
    verificar(r1 == 1 && r2 == 1 && r3 == 1, "empilhar retorna 1");
    checarPilha(p, (int[]){10, 20, 30}, 3, "pilha = [10, 20, 30]");
    verificar(pilhaVazia(p) == 0, "pilha não está vazia");

    secao("consultarTopo não remove");
    v = -1;
    verificar(consultarTopo(p, &v) == 1 && v == 30, "consultarTopo devolve 30");
    checarPilha(p, (int[]){10, 20, 30}, 3, "pilha continua [10, 20, 30]");

    secao("desempilhar segue LIFO");
    v = -1;
    verificar(desempilhar(p, &v) == 1 && v == 30, "1º desempilhar devolve 30");
    v = -1;
    verificar(desempilhar(p, &v) == 1 && v == 20, "2º desempilhar devolve 20");
    checarPilha(p, (int[]){10}, 1, "pilha = [10]");

    secao("Intercalando operações");
    empilhar(p, 40);
    checarPilha(p, (int[]){10, 40}, 2, "empilhar 40 -> [10, 40]");
    v = -1;
    verificar(desempilhar(p, &v) == 1 && v == 40, "desempilhar devolve 40");
    v = -1;
    verificar(desempilhar(p, &v) == 1 && v == 10, "desempilhar devolve 10");
    verificar(pilhaVazia(p) == 1 && p->topo == NULL && p->tamanho == 0, "pilha vazia de novo (topo NULL, tamanho 0)");
    verificar(desempilhar(p, &v) == 0, "desempilhar de novo retorna 0");

    secao("liberarPilha");
    empilhar(p, 1);
    empilhar(p, 2);
    liberarPilha(p);
    verificar(1, "liberarPilha executou (o teste de vazamento abaixo confere os free)");

    return resumo();
}
