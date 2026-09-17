/*
 * ==========================================================================
 *  L02 — LISTAS | Remover a primeira ocorrência       (Simulado 2026.2 — Q04)
 * --------------------------------------------------------------------------
 *  DESAFIO
 *  Implemente uma função que remova a PRIMEIRA ocorrência de 'valor' em uma
 *  lista simplesmente encadeada. A função deve retornar o início atualizado
 *  da lista e LIBERAR (free) a memória do nó removido.
 *
 *  Trate: lista vazia, remoção no início, remoção no meio/fim e valor
 *  inexistente (lista volta sem alterações).
 *
 *  ASSINATURA
 *      NoLista *removerPrimeiraOcorrencia(NoLista *inicio, int valor);
 *
 *  OBS: o teste roda com AddressSanitizer — se você esquecer o free(),
 *  aparecerá um relatório "LeakSanitizer: detected memory leaks" no final.
 *
 *  COMO TESTAR
 *      ./testar.sh L02
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
NoLista *removerPrimeiraOcorrencia(NoLista *inicio, int valor) {
    if (!inicio) return NULL;
    if (inicio->valor == valor) {
        NoLista *proximo = inicio->prox;
        free(inicio);
        return proximo;
    }
    inicio->prox = removerPrimeiraOcorrencia(inicio->prox, valor);
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
    printf("L02 — removerPrimeiraOcorrencia\n");
    NoLista *l;

    secao("Lista vazia");
    l = removerPrimeiraOcorrencia(NULL, 10);
    verificar(l == NULL, "remover de [] retorna NULL");

    secao("Remoção no início");
    l = listaDe((int[]){10, 20, 30}, 3);
    l = removerPrimeiraOcorrencia(l, 10);
    checarLista(l, (int[]){20, 30}, 2, "[10, 20, 30] - 10 = [20, 30]");
    liberarLista(l);

    secao("Remoção no meio");
    l = listaDe((int[]){10, 20, 30}, 3);
    l = removerPrimeiraOcorrencia(l, 20);
    checarLista(l, (int[]){10, 30}, 2, "[10, 20, 30] - 20 = [10, 30]");
    liberarLista(l);

    secao("Remoção no fim");
    l = listaDe((int[]){10, 20, 30}, 3);
    l = removerPrimeiraOcorrencia(l, 30);
    checarLista(l, (int[]){10, 20}, 2, "[10, 20, 30] - 30 = [10, 20]");
    liberarLista(l);

    secao("Valor inexistente");
    l = listaDe((int[]){10, 20, 30}, 3);
    NoLista *antes = l;
    l = removerPrimeiraOcorrencia(l, 99);
    checarLista(l, (int[]){10, 20, 30}, 3, "remover 99 não altera a lista");
    verificar(l == antes, "o início continua o mesmo");
    liberarLista(l);

    secao("Somente a PRIMEIRA ocorrência");
    l = listaDe((int[]){5, 7, 5, 5}, 4);
    l = removerPrimeiraOcorrencia(l, 5);
    checarLista(l, (int[]){7, 5, 5}, 3, "[5, 7, 5, 5] - 5 = [7, 5, 5]");
    liberarLista(l);

    secao("Lista com um único elemento");
    l = listaDe((int[]){42}, 1);
    l = removerPrimeiraOcorrencia(l, 42);
    verificar(l == NULL, "[42] - 42 = [] (retorna NULL)");
    liberarLista(l);

    return resumo();
}
