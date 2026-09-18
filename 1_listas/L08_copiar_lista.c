/*
 * ==========================================================================
 *  L08 — LISTAS | Cópia completa (deep copy)
 *            (versão "lista" da Q02 do Simulado 2026.2 — copiar árvore)
 * --------------------------------------------------------------------------
 *  DESAFIO
 *  Implemente uma função que produza uma cópia completa de uma lista
 *  simplesmente encadeada: mesmos valores, mesma ordem, mas NENHUM nó
 *  compartilhado com a original.
 *
 *  - Lista vazia -> retorna NULL.
 *  - Falha de alocação no meio da cópia -> libere tudo o que já foi
 *    copiado e retorne NULL (o teste simula essa falha!).
 *  - IMPORTANTE: aloque os nós SOMENTE com novoNo(valor), senão o teste
 *    de falha de alocação não consegue simular o erro.
 *
 *  ASSINATURA
 *      NoLista *copiarLista(NoLista *inicio);
 *
 *  COMO TESTAR
 *      ./testar.sh L08
 * ==========================================================================
 */
#include <stdio.h>
#include <stdlib.h>

/* contador para simular falha de malloc (não mexa) */
int falharNaAlocacao = -1;   /* -1 = nunca falha; k = a k-ésima chamada (1, 2, ...) falha */
int alocacoes = 0;

typedef struct NoLista {
    int valor;
    struct NoLista *prox;
} NoLista;

/* Aloca um nó com 'valor' e prox = NULL. Retorna NULL se o malloc falhar. */
NoLista *novoNo(int valor) {
    alocacoes++;
    if (falharNaAlocacao == alocacoes) return NULL;
    NoLista *no = (NoLista *) malloc(sizeof(NoLista));
    if (no == NULL) return NULL;
    no->valor = valor;
    no->prox = NULL;
    return no;
}

/* >>>>>>>>>>>>>>>>>>>>>>>>>> SUA SOLUÇÃO AQUI >>>>>>>>>>>>>>>>>>>>>>>>>> */
NoLista *copiarLista(NoLista *inicio) {
    if (!inicio) return NULL;

    NoLista *newHead = NULL;
    NoLista *newLast = NULL;
    
    newHead = novoNo(inicio->valor);
    if (!newHead) return NULL;
    newHead->prox = newLast;

    inicio = inicio->prox;

    while (inicio) {

        NoLista *new = novoNo(inicio->valor);

        if (!new) {
            while (newHead) {
                NoLista *toDelete = newHead;
                newHead = newHead->prox;
                free(toDelete);
            }
            return NULL;
        }

        newLast = new;
        newLast->prox = new;

        inicio = inicio->prox;
    }
    
    return newHead;
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
    printf("L08 — copiarLista\n");
    int v[] = {3, 1, 4, 1, 5};
    NoLista *orig = listaDe(v, 5);

    secao("Lista vazia");
    verificar(copiarLista(NULL) == NULL, "copiar [] retorna NULL");

    secao("Cópia de [3, 1, 4, 1, 5]");
    NoLista *copia = copiarLista(orig);
    checarLista(copia, v, 5, "a cópia tem os mesmos valores na mesma ordem");
    checarLista(orig, v, 5, "a original não foi alterada");
    int compartilha = compartilhamNos(copia, orig);
    verificar(copia != NULL && !compartilha, "nenhum nó é compartilhado");

    secao("Independência");
    if (copia != NULL && !compartilha) {
        copia->valor = 99;
        verificar(orig->valor == 3, "alterar a cópia não altera a original");
        liberarLista(copia);
    } else {
        verificar(0, "alterar a cópia não altera a original");
    }

    secao("Falha de alocação no 3º nó");
    alocacoes = 0;
    falharNaAlocacao = 3;
    NoLista *r = copiarLista(orig);
    falharNaAlocacao = -1;
    verificar(alocacoes >= 3, "a cópia usou novoNo() para alocar");
    verificar(r == NULL, "retorna NULL quando o malloc falha");
    printf("    (se aparecer 'LeakSanitizer' no final, você não liberou os nós já copiados)\n");
    if (r != NULL && !compartilhamNos(r, orig)) liberarLista(r);

    liberarLista(orig);
    return resumo();
}
