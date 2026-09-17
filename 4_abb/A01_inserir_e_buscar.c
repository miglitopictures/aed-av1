/*
 * ==========================================================================
 *  A01 — ABB | Inserir e buscar (recursivo)                     (aquecimento)
 * --------------------------------------------------------------------------
 *  DESAFIO
 *  a) inserirABB: insere 'valor' na árvore binária de busca e retorna a
 *     raiz (que muda quando a árvore estava vazia). Menores à esquerda,
 *     maiores à direita. Valores REPETIDOS são ignorados.
 *     Use novoNoArvore(valor) para alocar. Se falhar, não altere a árvore.
 *
 *  b) buscarABB: retorna o ponteiro para o nó que contém 'valor', ou NULL
 *     se não existir. Aproveite a propriedade da ABB (não visite a árvore
 *     inteira!).
 *
 *  Faça as duas de forma RECURSIVA.
 *
 *  ASSINATURAS
 *      NoArvore *inserirABB(NoArvore *raiz, int valor);
 *      NoArvore *buscarABB(NoArvore *raiz, int valor);
 *
 *  COMO TESTAR
 *      ./testar.sh A01
 * ==========================================================================
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct NoArvore {
    int valor;
    struct NoArvore *esq;
    struct NoArvore *dir;
} NoArvore;

/* contador para simular falha de malloc nos testes (não mexa) */
int falharNaAlocacao = -1;   /* -1 = nunca falha; k = a k-ésima chamada falha */
int alocacoes = 0;

/* Aloca um nó folha com 'valor'. Retorna NULL se o malloc falhar. */
NoArvore *novoNoArvore(int valor) {
    alocacoes++;
    if (alocacoes == falharNaAlocacao) return NULL;
    NoArvore *no = (NoArvore *) malloc(sizeof(NoArvore));
    if (no == NULL) return NULL;
    no->valor = valor;
    no->esq = no->dir = NULL;
    return no;
}

/* >>>>>>>>>>>>>>>>>>>>>>>>>> SUA SOLUÇÃO AQUI >>>>>>>>>>>>>>>>>>>>>>>>>> */
NoArvore *inserirABB(NoArvore *raiz, int valor) {
    /* TODO: implemente aqui */
    return raiz;
}

NoArvore *buscarABB(NoArvore *raiz, int valor) {
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
/* ---------- auxiliares de teste para árvores ---------- */

/* monta um nó "na mão": N(10, N(5, NULL, NULL), NULL) */
NoArvore *N(int valor, NoArvore *esq, NoArvore *dir) {
    NoArvore *no = (NoArvore *) malloc(sizeof(NoArvore));
    no->valor = valor;
    no->esq = esq;
    no->dir = dir;
    return no;
}

void preOrdemVet(NoArvore *r, int *v, int *n) {
    if (r == NULL || *n >= LIMITE_NOS) return;
    v[(*n)++] = r->valor;
    preOrdemVet(r->esq, v, n);
    preOrdemVet(r->dir, v, n);
}

void emOrdemVet(NoArvore *r, int *v, int *n) {
    if (r == NULL || *n >= LIMITE_NOS) return;
    emOrdemVet(r->esq, v, n);
    v[(*n)++] = r->valor;
    emOrdemVet(r->dir, v, n);
}

void posOrdemVet(NoArvore *r, int *v, int *n) {
    if (r == NULL || *n >= LIMITE_NOS) return;
    posOrdemVet(r->esq, v, n);
    posOrdemVet(r->dir, v, n);
    v[(*n)++] = r->valor;
}

/* A pré-ordem de uma ABB (valores distintos) determina sua FORMA
 * exatamente — por isso os testes comparam a pré-ordem. */
void checarPreOrdem(NoArvore *r, const int *esperado, int ne, const char *descricao) {
    static int buf[LIMITE_NOS];
    int n = 0;
    preOrdemVet(r, buf, &n);
    char desc2[300];
    snprintf(desc2, sizeof desc2, "%s  (pré-ordem)", descricao);
    checarVetor(buf, n, esperado, ne, desc2);
}

void liberarArvore(NoArvore *r) {
    if (r == NULL) return;
    liberarArvore(r->esq);
    liberarArvore(r->dir);
    free(r);
}

/* guarda os endereços dos nós em 'v' (para testar compartilhamento) */
void enderecos(NoArvore *r, NoArvore **v, int *n) {
    if (r == NULL || *n >= LIMITE_NOS) return;
    v[(*n)++] = r;
    enderecos(r->esq, v, n);
    enderecos(r->dir, v, n);
}

int arvoresCompartilham(NoArvore *a, NoArvore *b) {
    static NoArvore *va[LIMITE_NOS], *vb[LIMITE_NOS];
    int na = 0, nb = 0;
    enderecos(a, va, &na);
    enderecos(b, vb, &nb);
    for (int i = 0; i < na; i++)
        for (int j = 0; j < nb; j++)
            if (va[i] == vb[j]) return 1;
    return 0;
}

/* árvores de exemplo usadas em vários testes (NÃO são todas ABB) */
NoArvore *arvoreSimulado2(void) {      /* Simulado 2, Q04 */
    /*        12
     *      /    \
     *     8      20
     *    / \    /  \
     *   5  10  15   25
     *            \
     *             17          */
    return N(12, N(8, N(5, NULL, NULL), N(10, NULL, NULL)),
                 N(20, N(15, NULL, N(17, NULL, NULL)), N(25, NULL, NULL)));
}

NoArvore *arvoreCompleta7(void) {
    /*        4
     *      /   \
     *     2     6
     *    / \   / \
     *   1   3 5   7            */
    return N(4, N(2, N(1, NULL, NULL), N(3, NULL, NULL)),
                N(6, N(5, NULL, NULL), N(7, NULL, NULL)));
}

NoArvore *arvoreTorta(void) {
    /*        10
     *       /  \
     *      5    20
     *     /       \
     *    3         30
     *   /         /  \
     *  1         25   35       */
    return N(10, N(5, N(3, N(1, NULL, NULL), NULL), NULL),
                 N(20, NULL, N(30, N(25, NULL, NULL), N(35, NULL, NULL))));
}

NoArvore *arvoreCorrente(void) {
    /* 1 -> 2 -> 3 -> 4 (só filhos à direita) */
    return N(1, NULL, N(2, NULL, N(3, NULL, N(4, NULL, NULL))));
}

int main(void) {
    printf("A01 — inserirABB / buscarABB\n");
    NoArvore *r = NULL;

    secao("Inserir em árvore vazia");
    r = inserirABB(r, 50);
    verificar(r != NULL && r->valor == 50 && r->esq == NULL && r->dir == NULL, "raiz = 50, sem filhos");

    secao("Inserir 30, 70, 20, 40, 60, 80");
    int v[] = {30, 70, 20, 40, 60, 80};
    for (int i = 0; i < TAM(v); i++) r = inserirABB(r, v[i]);
    /*        50
     *      /    \
     *    30      70
     *   /  \    /  \
     *  20  40  60  80        */
    checarPreOrdem(r, (int[]){50, 30, 20, 40, 70, 60, 80}, 7, "forma correta");
    verificar(r != NULL && r->valor == 50, "a raiz continua sendo 50");

    secao("Inserir 35, 65 e 10");
    r = inserirABB(r, 35);
    r = inserirABB(r, 65);
    r = inserirABB(r, 10);
    checarPreOrdem(r, (int[]){50, 30, 20, 10, 40, 35, 70, 60, 65, 80}, 10, "35 à esq. do 40, 65 à dir. do 60, 10 à esq. do 20");

    secao("Valor repetido é ignorado");
    r = inserirABB(r, 40);
    checarPreOrdem(r, (int[]){50, 30, 20, 10, 40, 35, 70, 60, 65, 80}, 10, "inserir 40 de novo não muda nada");

    secao("buscarABB");
    NoArvore *x = buscarABB(r, 65);
    verificar(x != NULL && x->valor == 65, "buscar 65 encontra o nó");
    x = buscarABB(r, 50);
    verificar(x == r, "buscar 50 retorna a própria raiz");
    x = buscarABB(r, 10);
    verificar(x != NULL && x->valor == 10 && x->esq == NULL && x->dir == NULL, "buscar 10 (folha) encontra o nó");
    verificar(buscarABB(r, 66) == NULL, "buscar 66 retorna NULL");
    verificar(buscarABB(r, 5) == NULL, "buscar 5 retorna NULL");
    verificar(buscarABB(NULL, 5) == NULL, "buscar em árvore vazia retorna NULL");

    liberarArvore(r);
    return resumo();
}
