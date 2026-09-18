/*
 * ==========================================================================
 *  L10 — LISTAS | Lista DUPLAMENTE encadeada: inserir no início e remover
 * --------------------------------------------------------------------------
 *  DESAFIO
 *  Cada nó tem ponteiro para o anterior (ant) e para o próximo (prox).
 *  Implemente:
 *
 *  a) inserirInicioDupla: insere 'valor' no início e retorna o novo início.
 *     (o antigo primeiro nó deve passar a ter ant apontando para o novo)
 *
 *  b) removerDupla: remove a PRIMEIRA ocorrência de 'valor', libera o nó e
 *     retorna o início atualizado. Mantenha ant e prox consistentes:
 *       - o primeiro nó sempre tem ant == NULL
 *       - para todo nó X com X->prox != NULL: X->prox->ant == X
 *     Trate: lista vazia, remoção do início, do meio, do fim, inexistente.
 *
 *  ASSINATURAS
 *      NoDuplo *inserirInicioDupla(NoDuplo *inicio, int valor);
 *      NoDuplo *removerDupla(NoDuplo *inicio, int valor);
 *
 *  COMO TESTAR
 *      ./testar.sh L10
 * ==========================================================================
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct NoDuplo {
    int valor;
    struct NoDuplo *ant;
    struct NoDuplo *prox;
} NoDuplo;

/* Aloca um nó com 'valor' e ant = prox = NULL. Retorna NULL se falhar. */
NoDuplo *novoNoDuplo(int valor) {
    NoDuplo *no = (NoDuplo *) malloc(sizeof(NoDuplo));
    if (no == NULL) return NULL;
    no->valor = valor;
    no->ant = no->prox = NULL;
    return no;
}

/* >>>>>>>>>>>>>>>>>>>>>>>>>> SUA SOLUÇÃO AQUI >>>>>>>>>>>>>>>>>>>>>>>>>> */
NoDuplo *inserirInicioDupla(NoDuplo *inicio, int valor) {
    if (!inicio) return novoNoDuplo(valor);

    NoDuplo *novoItem = novoNoDuplo(valor);
    novoItem->prox = inicio;
    inicio->ant = novoItem;
    return novoItem; 
}

NoDuplo *removerDupla(NoDuplo *inicio, int valor) {
    // caso base 1: lista vazia ou terminou.
    if (!inicio) return NULL;
    
    // caso base 2: achamos o valor!
    if (inicio->valor == valor) {
        // rewiring the list
        // pegamos o ponteiro para o proximo (pode ser NULL)
        NoDuplo *next = inicio->prox;
        // caso o next exista,  next->ant deve apontar para o anterior de inicio (pode ser NULL)
        if (next) next->ant = inicio->ant;
        // liberamos a memoria do node que vamos deletar
        free(inicio);
        return next;
    }

    // Nao encontramos o valor, continuamos procurando recursivamente.
    inicio->prox = removerDupla(inicio->prox, valor);

    // Retornamos o inicio da lista (alterada no meio).
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

/* ---------- auxiliares de teste para lista dupla ---------- */
NoDuplo *duplaDe(const int *v, int n) {
    NoDuplo *ini = NULL, *fim = NULL;
    for (int i = 0; i < n; i++) {
        NoDuplo *no = novoNoDuplo(v[i]);
        if (ini == NULL) ini = no; else { fim->prox = no; no->ant = fim; }
        fim = no;
    }
    return ini;
}

/* confere valores (ida e volta) e consistência dos ponteiros ant/prox */
void checarDupla(NoDuplo *l, const int *esp, int ne, const char *desc) {
    static int ida[LIMITE_NOS], volta[LIMITE_NOS];
    int n = 0, m = 0, consistente = 1;
    if (l != NULL && l->ant != NULL) consistente = 0;
    NoDuplo *ultimo = NULL;
    for (NoDuplo *p = l; p != NULL && n < ne + 5; p = p->prox) {
        ida[n++] = p->valor;
        if (p->prox != NULL && p->prox->ant != p) consistente = 0;
        ultimo = p;
    }
    for (NoDuplo *p = ultimo; p != NULL && m < ne + 5; p = p->ant) volta[m++] = p->valor;
    for (int i = 0; i < m / 2; i++) { int t = volta[i]; volta[i] = volta[m - 1 - i]; volta[m - 1 - i] = t; }

    checarVetor(ida, n, esp, ne, desc);
    if (!verificar(consistente && vetoresIguais(volta, m, esp, ne), "   ponteiros ant consistentes (percurso de trás pra frente)")) {
        printf("      percorrendo pelo ant a partir do último: "); imprimirVetor(volta, m); printf("\n");
    }
}

void liberarDupla(NoDuplo *l) {
    int n = 0;
    while (l != NULL && n++ < LIMITE_NOS) { NoDuplo *p = l->prox; free(l); l = p; }
}

int main(void) {
    printf("L10 — lista duplamente encadeada\n");
    NoDuplo *l = NULL;

    secao("a) inserirInicioDupla");
    l = inserirInicioDupla(l, 30);
    checarDupla(l, (int[]){30}, 1, "inserir 30 em [] = [30]");
    l = inserirInicioDupla(l, 20);
    l = inserirInicioDupla(l, 10);
    checarDupla(l, (int[]){10, 20, 30}, 3, "inserir 20 e 10 = [10, 20, 30]");
    liberarDupla(l);

    secao("b) removerDupla — lista vazia");
    verificar(removerDupla(NULL, 1) == NULL, "remover de [] retorna NULL");

    secao("b) removerDupla — início");
    l = duplaDe((int[]){1, 2, 3, 4}, 4);
    l = removerDupla(l, 1);
    checarDupla(l, (int[]){2, 3, 4}, 3, "[1, 2, 3, 4] - 1 = [2, 3, 4]");
    liberarDupla(l);

    secao("b) removerDupla — meio");
    l = duplaDe((int[]){1, 2, 3, 4}, 4);
    l = removerDupla(l, 3);
    checarDupla(l, (int[]){1, 2, 4}, 3, "[1, 2, 3, 4] - 3 = [1, 2, 4]");
    liberarDupla(l);

    secao("b) removerDupla — fim");
    l = duplaDe((int[]){1, 2, 3, 4}, 4);
    l = removerDupla(l, 4);
    checarDupla(l, (int[]){1, 2, 3}, 3, "[1, 2, 3, 4] - 4 = [1, 2, 3]");
    liberarDupla(l);

    secao("b) removerDupla — inexistente e único elemento");
    l = duplaDe((int[]){1, 2}, 2);
    l = removerDupla(l, 9);
    checarDupla(l, (int[]){1, 2}, 2, "[1, 2] - 9 = [1, 2]");
    liberarDupla(l);
    l = duplaDe((int[]){5}, 1);
    l = removerDupla(l, 5);
    verificar(l == NULL, "[5] - 5 = [] (retorna NULL)");
    liberarDupla(l);

    return resumo();
}
