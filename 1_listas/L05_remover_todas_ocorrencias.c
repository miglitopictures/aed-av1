/*
 * ==========================================================================
 *  L05 — LISTAS | Remover TODAS as ocorrências
 * --------------------------------------------------------------------------
 *  DESAFIO
 *  Variação da L02: remova TODAS as ocorrências de 'valor' de uma lista
 *  simplesmente encadeada, liberando cada nó removido, e retorne o início
 *  atualizado.
 *
 *  Cuidado com: ocorrências seguidas, ocorrências no início (várias!),
 *  lista em que todos os elementos são iguais a 'valor', lista vazia.
 *
 *  ASSINATURA
 *      NoLista *removerTodasOcorrencias(NoLista *inicio, int valor);
 *
 *  COMO TESTAR
 *      ./testar.sh L05
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
NoLista *removerTodasOcorrencias(NoLista *inicio, int valor) {
    /* TODO: implemente aqui */
    return inicio;
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
    printf("L05 — removerTodasOcorrencias\n");
    NoLista *l;

    secao("Lista vazia");
    verificar(removerTodasOcorrencias(NULL, 1) == NULL, "remover de [] retorna NULL");

    secao("Ocorrências no início, meio (seguidas) e fim");
    l = listaDe((int[]){7, 7, 1, 7, 7, 2, 7}, 7);
    l = removerTodasOcorrencias(l, 7);
    checarLista(l, (int[]){1, 2}, 2, "[7, 7, 1, 7, 7, 2, 7] sem 7 = [1, 2]");
    liberarLista(l);

    secao("Todos os elementos iguais");
    l = listaDe((int[]){3, 3, 3}, 3);
    l = removerTodasOcorrencias(l, 3);
    verificar(l == NULL, "[3, 3, 3] sem 3 = [] (retorna NULL)");
    liberarLista(l);

    secao("Nenhuma ocorrência");
    l = listaDe((int[]){1, 2, 3}, 3);
    l = removerTodasOcorrencias(l, 9);
    checarLista(l, (int[]){1, 2, 3}, 3, "[1, 2, 3] sem 9 = [1, 2, 3]");
    liberarLista(l);

    secao("Uma única ocorrência no meio");
    l = listaDe((int[]){4, 5, 6}, 3);
    l = removerTodasOcorrencias(l, 5);
    checarLista(l, (int[]){4, 6}, 2, "[4, 5, 6] sem 5 = [4, 6]");
    liberarLista(l);

    return resumo();
}
