/*
 * ==========================================================================
 *  L04 — LISTAS | Inverter a lista (in-place)
 * --------------------------------------------------------------------------
 *  DESAFIO
 *  Implemente uma função que inverta uma lista simplesmente encadeada
 *  REAPROVEITANDO os nós (apenas trocando os ponteiros prox — não crie nem
 *  libere nós) e retorne o novo início.
 *
 *  Exemplo: [1, 2, 3, 4] -> [4, 3, 2, 1]
 *  Trate lista vazia e lista com um elemento.
 *
 *  Dica: use três ponteiros (anterior, atual, próximo).
 *
 *  ASSINATURA
 *      NoLista *inverterLista(NoLista *inicio);
 *
 *  COMO TESTAR
 *      ./testar.sh L04
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
NoLista *inverterLista(NoLista *inicio) {
    if (!inicio) return NULL;

    NoLista *anterior = NULL;
    NoLista *proximo = inicio->prox;

    while (proximo != NULL) {
        // invertemos a direcao do ponteiro
        inicio->prox = anterior;

        // andamos uma casinha
        anterior = inicio;
        inicio = proximo;
        proximo = proximo->prox;
    }

    inicio->prox = anterior;

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
    printf("L04 — inverterLista\n");
    NoLista *l;

    secao("Lista vazia");
    verificar(inverterLista(NULL) == NULL, "inverter [] retorna NULL");

    secao("Um elemento");
    l = listaDe((int[]){7}, 1);
    NoLista *unico = l;
    l = inverterLista(l);
    checarLista(l, (int[]){7}, 1, "inverter [7] = [7]");
    verificar(l == unico, "o nó é o mesmo");
    liberarLista(l);

    secao("Vários elementos");
    l = listaDe((int[]){1, 2, 3, 4}, 4);
    NoLista *ultimo = l->prox->prox->prox;
    NoLista *primeiro = l;
    l = inverterLista(l);
    checarLista(l, (int[]){4, 3, 2, 1}, 4, "inverter [1, 2, 3, 4] = [4, 3, 2, 1]");
    verificar(l == ultimo, "o novo início é o antigo último NÓ (nós reaproveitados)");
    verificar(primeiro->prox == NULL, "o antigo primeiro nó agora tem prox == NULL");
    liberarLista(l);

    secao("Dois elementos");
    l = listaDe((int[]){1, 2}, 2);
    l = inverterLista(l);
    checarLista(l, (int[]){2, 1}, 2, "inverter [1, 2] = [2, 1]");
    liberarLista(l);

    return resumo();
}
