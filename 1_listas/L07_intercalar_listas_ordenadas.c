/*
 * ==========================================================================
 *  L07 — LISTAS | Intercalar (merge) duas listas ordenadas
 * --------------------------------------------------------------------------
 *  DESAFIO
 *  L1 e L2 estão em ordem CRESCENTE. Crie e retorne uma TERCEIRA lista,
 *  também crescente, com todos os elementos das duas.
 *
 *  - As listas originais NÃO podem ser alteradas (nem valores, nem prox).
 *  - A nova lista deve ter nós NOVOS (nenhum nó compartilhado com L1/L2).
 *  - Se uma das listas acabar, copie o restante da outra.
 *  - Ambas vazias -> retorna NULL.
 *
 *  Exemplo: L1 = [1, 4, 7], L2 = [2, 3, 8, 9] -> [1, 2, 3, 4, 7, 8, 9]
 *
 *  ASSINATURA
 *      NoLista *intercalarOrdenadas(NoLista *L1, NoLista *L2);
 *
 *  COMO TESTAR
 *      ./testar.sh L07
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

// 1  4  7
// 2  3  8  9

// 1
// 4  7
// 2  3  8  9

// 1  2
// 4  7
// 3  8  9


/* >>>>>>>>>>>>>>>>>>>>>>>>>> SUA SOLUÇÃO AQUI >>>>>>>>>>>>>>>>>>>>>>>>>> */
NoLista *intercalarOrdenadas(NoLista *L1, NoLista *L2) {
    // caso base: nenhuma das listas existe.
    if (!L1 && !L2) return NULL;
    
    NoLista *newNode = NULL;
    
    // caso condicional: as duas existem, precisamos comparar e colocar o menor valor
    if (L1 && L2) {
        if (L1->valor < L2->valor) {
            newNode = novoNo(L1->valor);
            newNode->prox = intercalarOrdenadas(L1->prox, L2);
        } else {
            newNode = novoNo(L2->valor);
            newNode->prox = intercalarOrdenadas(L1, L2->prox);
        }
        return newNode;
    }
    
    // caso final: uma das listas terminou, compiamos a que sobrou ate as duas acabarem ne caso base
    NoLista *resto = L1 ? L1 : L2;
    newNode = novoNo(resto->valor);
    newNode->prox = intercalarOrdenadas(resto->prox, NULL);
    return newNode;
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

void caso(const int *a, int na, const int *b, int nb, const int *esp, int ne, const char *desc) {
    NoLista *L1 = listaDe(a, na), *L2 = listaDe(b, nb);
    NoLista *r = intercalarOrdenadas(L1, L2);
    checarLista(r, esp, ne, desc);
    checarLista(L1, a, na, "   L1 continua igual");
    checarLista(L2, b, nb, "   L2 continua igual");
    int compartilha = compartilhamNos(r, L1) || compartilhamNos(r, L2);
    verificar(!compartilha, "   resultado não compartilha nós com L1/L2");
    if (!compartilha) liberarLista(r);
    liberarLista(L1);
    liberarLista(L2);
}

int main(void) {
    printf("L07 — intercalarOrdenadas\n");

    secao("Caso do enunciado");
    caso((int[]){1, 4, 7}, 3, (int[]){2, 3, 8, 9}, 4,
         (int[]){1, 2, 3, 4, 7, 8, 9}, 7, "[1, 4, 7] + [2, 3, 8, 9]");

    secao("Valores repetidos entre as listas");
    caso((int[]){1, 3, 5}, 3, (int[]){1, 3, 5}, 3,
         (int[]){1, 1, 3, 3, 5, 5}, 6, "[1, 3, 5] + [1, 3, 5]");

    secao("Uma lista vazia");
    caso(NULL, 0, (int[]){2, 4}, 2, (int[]){2, 4}, 2, "[] + [2, 4]");
    caso((int[]){6}, 1, NULL, 0, (int[]){6}, 1, "[6] + []");

    secao("Ambas vazias");
    NoLista *r = intercalarOrdenadas(NULL, NULL);
    verificar(r == NULL, "[] + [] = NULL");

    secao("Todos de L2 menores que os de L1");
    caso((int[]){10, 20}, 2, (int[]){1, 2, 3}, 3,
         (int[]){1, 2, 3, 10, 20}, 5, "[10, 20] + [1, 2, 3]");

    return resumo();
}
