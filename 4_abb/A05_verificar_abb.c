/*
 * ==========================================================================
 *  A05 — ÁRVORES | Verificar se é uma ABB válida    (Prova AV1 2026.1 — Q03)
 * --------------------------------------------------------------------------
 *  DESAFIO
 *  Crie uma função RECURSIVA para verificar se uma árvore binária é uma
 *  ABB (Árvore Binária de Busca) válida. Retorne 1 se for, 0 se não for.
 *
 *  Definição usada: para TODO nó X,
 *    - todos os valores da subárvore ESQUERDA são  < X->valor
 *    - todos os valores da subárvore DIREITA  são  > X->valor
 *  (valores repetidos => NÃO é ABB). Árvore vazia é ABB.
 *
 *  ARMADILHA: comparar cada nó só com seus filhos diretos NÃO basta!
 *        50
 *       /  \
 *     30    70        <- 60 é maior que 30 (ok localmente), mas está à
 *    /  \                ESQUERDA do 50 -> não é ABB.
 *   20   60
 *
 *  Dica: use uma função auxiliar que recebe o intervalo permitido
 *  (mínimo, máximo) para cada subárvore. Cuidado com os limites: o teste
 *  final usa INT_MIN e INT_MAX como valores válidos (use long long para
 *  os limites, ou ponteiros para os nós-limite).
 *
 *  ASSINATURA
 *      int ehABB(NoArvore *raiz);
 *
 *  COMO TESTAR
 *      ./testar.sh A05
 * ==========================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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
int ehABB(NoArvore *raiz) {
    /* TODO: implemente aqui (crie funções auxiliares se quiser) */
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
/* ---------- inserção/remoção de REFERÊNCIA usadas só pelos testes ---------- */
NoArvore *inserirRef(NoArvore *r, int valor) {
    if (r == NULL) return N(valor, NULL, NULL);
    if (valor < r->valor) r->esq = inserirRef(r->esq, valor);
    else if (valor > r->valor) r->dir = inserirRef(r->dir, valor);
    return r;
}

NoArvore *abbDe(const int *v, int n) {
    NoArvore *r = NULL;
    for (int i = 0; i < n; i++) r = inserirRef(r, v[i]);
    return r;
}

/* remoção com o "algoritmo da direita": nó com 2 filhos é substituído
 * pelo MENOR valor da subárvore DIREITA (sucessor em-ordem) */
NoArvore *removerRef(NoArvore *r, int valor) {
    if (r == NULL) return NULL;
    if (valor < r->valor) r->esq = removerRef(r->esq, valor);
    else if (valor > r->valor) r->dir = removerRef(r->dir, valor);
    else if (r->esq == NULL || r->dir == NULL) {
        NoArvore *filho = r->esq != NULL ? r->esq : r->dir;
        free(r);
        return filho;
    } else {
        NoArvore *s = r->dir;
        while (s->esq != NULL) s = s->esq;
        r->valor = s->valor;
        r->dir = removerRef(r->dir, s->valor);
    }
    return r;
}

void caso(NoArvore *r, int esperado, const char *desc) {
    int obtido = ehABB(r);
    char d[200];
    snprintf(d, sizeof d, "%s -> %d", desc, esperado);
    if (!verificar(obtido == esperado, d)) printf("      obtido: %d\n", obtido);
    liberarArvore(r);
}

int main(void) {
    printf("A05 — ehABB\n");

    secao("Casos base");
    caso(NULL, 1, "árvore vazia");
    caso(N(5, NULL, NULL), 1, "um único nó");
    caso(N(5, N(3, NULL, NULL), N(8, NULL, NULL)), 1, "5(3, 8)");
    caso(N(5, N(8, NULL, NULL), NULL), 0, "8 à esquerda do 5");
    caso(N(5, NULL, N(2, NULL, NULL)), 0, "2 à direita do 5");

    secao("ABBs válidas maiores");
    caso(arvoreCompleta7(), 1, "árvore completa 4(2(1,3), 6(5,7))");
    caso(arvoreSimulado2(), 1, "árvore do Simulado 2");
    int prova[] = {70, 40, 90, 20, 60, 80, 100, 10, 30, 50, 65, 85, 95, 110, 55, 52, 53, 54, 51, 57};
    caso(abbDe(prova, TAM(prova)), 1, "árvore da Prova AV1 2026.1 (20 nós)");
    caso(arvoreCorrente(), 1, "corrente 1->2->3->4");

    secao("Armadilhas (violação que não é entre pai e filho)");
    caso(N(50, N(30, N(20, NULL, NULL), N(60, NULL, NULL)), N(70, NULL, NULL)), 0,
         "60 na subárvore esquerda do 50");
    caso(N(50, N(30, NULL, NULL), N(70, N(40, NULL, NULL), N(80, NULL, NULL))), 0,
         "40 na subárvore direita do 50");
    caso(N(10, N(5, NULL, N(7, NULL, N(12, NULL, NULL))), NULL), 0,
         "12 bem fundo à esquerda do 10");
    caso(N(12, N(8, N(5, NULL, NULL), N(10, NULL, NULL)),
               N(20, N(15, N(11, NULL, NULL), NULL), N(25, NULL, NULL))), 0,
         "11 na subárvore direita do 12 (à esq. do 15)");

    secao("Valores repetidos");
    caso(N(5, N(5, NULL, NULL), NULL), 0, "5 à esquerda do 5");
    caso(N(5, NULL, N(5, NULL, NULL)), 0, "5 à direita do 5");

    secao("Bônus: limites de int");
    caso(N(0, N(INT_MIN, NULL, NULL), N(INT_MAX, NULL, NULL)), 1, "0(INT_MIN, INT_MAX) é ABB");
    caso(N(INT_MIN, NULL, N(INT_MAX, NULL, NULL)), 1, "INT_MIN(-, INT_MAX) é ABB");

    return resumo();
}
