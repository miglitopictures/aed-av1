/*
 * ==========================================================================
 *  A08 — ABB | Questão de papel                    (Simulado 2026.2 — Q05)
 * --------------------------------------------------------------------------
 *  DESAFIO  (faça TUDO no papel antes de preencher!)
 *  Considere uma árvore binária de busca inicialmente vazia.
 *
 *  a) Construa a árvore inserindo, nesta ordem:
 *       60, 35, 85, 20, 45, 75, 95, 10, 30, 40, 50, 70, 80, 90, 100,
 *       42, 41, 43, 47, 55
 *  b) Informe o percurso em PRÉ-ORDEM.
 *  c) Informe o percurso em PÓS-ORDEM.
 *  d) Remova o nó 35 e redesenhe a árvore. Quando o nó tiver dois filhos,
 *     use sempre o MENOR valor da subárvore DIREITA.
 *  e) A partir da árvore do item d, remova o nó 45 e redesenhe a árvore
 *     usando a mesma regra.
 *
 *  COMO CONFERIR OS DESENHOS
 *  Um desenho de ABB não cabe num vetor, mas a PRÉ-ORDEM de uma ABB
 *  determina a sua forma. Então, para os itens a, d e e, escreva a
 *  pré-ordem da árvore que você DESENHOU (leia do seu desenho!).
 *  O item a) é conferido pelo item b).
 *
 *  COMO TESTAR
 *      ./testar.sh A08
 *  Para ver o gabarito das que errou, mude MOSTRAR_GABARITO para 1.
 * ==========================================================================
 */
#include <stdio.h>
#include <stdlib.h>

#define MOSTRAR_GABARITO 0

/* >>>>>>>>>>>>>>>>>>>>>>>>>> SUA SOLUÇÃO AQUI >>>>>>>>>>>>>>>>>>>>>>>>>> */
int b_preOrdem[]     = { 0 };   /* árvore do item a */
int c_posOrdem[]     = { 0 };   /* árvore do item a */
int d_preOrdem[]     = { 0 };   /* árvore após remover 35 */
int e_preOrdem[]     = { 0 };   /* árvore após remover 35 e 45 */
/* <<<<<<<<<<<<<<<<<<<<<<<<<< FIM DA SUA SOLUÇÃO <<<<<<<<<<<<<<<<<<<<<<<<<< */

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
void conferir(const int *sua, int ns, const int *certo, int nc, const char *desc) {
    if (!verificar(vetoresIguais(sua, ns, certo, nc), desc)) {
        printf("      sua resposta: "); imprimirVetor(sua, ns); printf("\n");
        int k = 0;
        while (k < ns && k < nc && sua[k] == certo[k]) k++;
        if (ns != nc) printf("      quantidade de números: sua = %d, esperada = %d\n", ns, nc);
        if (k < ns || k < nc) printf("      primeira divergência na posição %d (contando do 1)\n", k + 1);
        if (MOSTRAR_GABARITO) { printf("      gabarito:     "); imprimirVetor(certo, nc); printf("\n"); }
    }
}

/* confere a pré-ordem e a pós-ordem da árvore atual */
void conferirPre(NoArvore *r, const int *sua, int ns, const char *desc) {
    int v[LIMITE_NOS], n = 0;
    preOrdemVet(r, v, &n);
    conferir(sua, ns, v, n, desc);
}

void conferirPos(NoArvore *r, const int *sua, int ns, const char *desc) {
    int v[LIMITE_NOS], n = 0;
    posOrdemVet(r, v, &n);
    conferir(sua, ns, v, n, desc);
}

void conferirEm(NoArvore *r, const int *sua, int ns, const char *desc) {
    int v[LIMITE_NOS], n = 0;
    emOrdemVet(r, v, &n);
    conferir(sua, ns, v, n, desc);
}

int main(void) {
    printf("A08 — ABB no papel (Simulado 2026.2)\n");
    int ins[] = {60, 35, 85, 20, 45, 75, 95, 10, 30, 40, 50, 70, 80, 90, 100, 42, 41, 43, 47, 55};
    NoArvore *r = abbDe(ins, TAM(ins));

    secao("Construção e percursos");
    conferirPre(r, b_preOrdem, TAM(b_preOrdem), "b) pré-ordem (confere também o desenho do item a)");
    conferirPos(r, c_posOrdem, TAM(c_posOrdem), "c) pós-ordem");

    secao("Remoções");
    r = removerRef(r, 35);
    conferirPre(r, d_preOrdem, TAM(d_preOrdem), "d) após remover 35");
    r = removerRef(r, 45);
    conferirPre(r, e_preOrdem, TAM(e_preOrdem), "e) após remover 45");

    liberarArvore(r);
    return resumo();
}
