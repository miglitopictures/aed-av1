/*
 * ==========================================================================
 *  L03 — LISTAS | Concatenar duas listas                (Prova AV1 2026.1 — Q04)
 * --------------------------------------------------------------------------
 *  DESAFIO
 *  Implemente uma função que concatene duas listas simplesmente encadeadas,
 *  ligando o FINAL da primeira ao INÍCIO da segunda (sem criar nós novos),
 *  e retorne o início da lista resultante.
 *
 *  "Faça todos os tratamentos necessários, não apenas o caminho feliz!"
 *    - L1 vazia            -> retorna L2
 *    - L2 vazia            -> retorna L1 (sem alterações)
 *    - ambas vazias        -> retorna NULL
 *    - L1 == L2 (mesma lista) -> NÃO concatene (isso criaria um ciclo);
 *                             retorne L1 sem alterações
 *
 *  ASSINATURA  (na prova o tipo se chamava No; aqui é NoLista)
 *      NoLista *concatenarListas(NoLista *L1, NoLista *L2);
 *
 *  COMO TESTAR
 *      ./testar.sh L03
 * ==========================================================================
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct NoLista {
    int valor;
    struct NoLista *prox;
} NoLista;

/* Aloca um nó com 'valor' e prox = NULL. Retorna NULL se o malloc falhar. */
NoLista *novoNo(int valor) {
    NoLista *no = (NoLista *) malloc(sizeof(NoLista));
    if (no == NULL) return NULL;
    no->valor = valor;
    no->prox = NULL;
    return no;
}

/* >>>>>>>>>>>>>>>>>>>>>>>>>> SUA SOLUÇÃO AQUI >>>>>>>>>>>>>>>>>>>>>>>>>> */
NoLista *concatenarListas(NoLista *L1, NoLista *L2) {
    /* TODO: implemente aqui */
    return L1;
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
/* ---------- auxiliares de teste para listas ---------- */
NoLista *listaDe(const int *v, int n) {
    NoLista *inicio = NULL, *fim = NULL;
    for (int i = 0; i < n; i++) {
        NoLista *no = novoNo(v[i]);
        if (inicio == NULL) inicio = no; else fim->prox = no;
        fim = no;
    }
    return inicio;
}

int temCiclo(NoLista *l) {            /* algoritmo de Floyd */
    NoLista *lento = l, *rapido = l;
    while (rapido != NULL && rapido->prox != NULL) {
        lento = lento->prox;
        rapido = rapido->prox->prox;
        if (lento == rapido) return 1;
    }
    return 0;
}

/* copia até LIMITE_NOS valores da lista para 'v'; retorna a quantidade */
int listaParaVetor(NoLista *l, int *v) {
    int n = 0;
    while (l != NULL && n < LIMITE_NOS) { v[n++] = l->valor; l = l->prox; }
    return n;
}

void checarLista(NoLista *l, const int *esperado, int ne, const char *descricao) {
    static int buf[LIMITE_NOS];
    if (temCiclo(l)) {
        verificar(0, descricao);
        printf("      a lista resultante tem um CICLO (algum prox aponta para trás)\n");
        return;
    }
    int n = listaParaVetor(l, buf);
    checarVetor(buf, n, esperado, ne, descricao);
}

void liberarLista(NoLista *l) {
    if (temCiclo(l)) return;          /* evita liberar duas vezes */
    while (l != NULL) {
        NoLista *prox = l->prox;
        free(l);
        l = prox;
    }
}

/* 1 se algum nó de 'a' também pertence a 'b' (mesmo endereço) */
int compartilhamNos(NoLista *a, NoLista *b) {
    if (temCiclo(a) || temCiclo(b)) return 1;
    for (NoLista *p = a; p != NULL; p = p->prox)
        for (NoLista *q = b; q != NULL; q = q->prox)
            if (p == q) return 1;
    return 0;
}

int main(void) {
    printf("L03 — concatenarListas\n");
    NoLista *a, *b, *r;

    secao("Caso normal");
    a = listaDe((int[]){1, 2, 3}, 3);
    b = listaDe((int[]){4, 5}, 2);
    NoLista *primeiroDeB = b;
    r = concatenarListas(a, b);
    checarLista(r, (int[]){1, 2, 3, 4, 5}, 5, "[1, 2, 3] + [4, 5] = [1, 2, 3, 4, 5]");
    verificar(r == a, "o início do resultado é o início de L1");
    verificar(a->prox != NULL && a->prox->prox != NULL && a->prox->prox->prox == primeiroDeB,
              "o último nó de L1 aponta para o PRIMEIRO nó de L2 (sem criar nós novos)");
    liberarLista(r);

    secao("L1 vazia");
    b = listaDe((int[]){7, 8}, 2);
    r = concatenarListas(NULL, b);
    verificar(r == b, "[] + [7, 8] retorna o próprio L2");
    checarLista(r, (int[]){7, 8}, 2, "conteúdo [7, 8]");
    liberarLista(r);

    secao("L2 vazia");
    a = listaDe((int[]){7, 8}, 2);
    r = concatenarListas(a, NULL);
    verificar(r == a, "[7, 8] + [] retorna o próprio L1");
    checarLista(r, (int[]){7, 8}, 2, "conteúdo [7, 8]");
    liberarLista(r);

    secao("Ambas vazias");
    r = concatenarListas(NULL, NULL);
    verificar(r == NULL, "[] + [] retorna NULL");

    secao("L1 e L2 são a MESMA lista");
    a = listaDe((int[]){1, 2}, 2);
    r = concatenarListas(a, a);
    verificar(!temCiclo(r), "não cria ciclo");
    checarLista(r, (int[]){1, 2}, 2, "lista continua [1, 2]");
    liberarLista(r);

    secao("Listas de um elemento");
    a = listaDe((int[]){9}, 1);
    b = listaDe((int[]){10}, 1);
    r = concatenarListas(a, b);
    checarLista(r, (int[]){9, 10}, 2, "[9] + [10] = [9, 10]");
    liberarLista(r);

    return resumo();
}
