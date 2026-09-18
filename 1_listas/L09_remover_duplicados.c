/*
 * ==========================================================================
 *  L09 — LISTAS | Remover elementos duplicados
 * --------------------------------------------------------------------------
 *  DESAFIO
 *  A lista NÃO está ordenada. Remova todos os valores repetidos, mantendo
 *  apenas a PRIMEIRA ocorrência de cada valor e preservando a ordem
 *  relativa. Libere os nós removidos.
 *
 *  Exemplo: [3, 1, 3, 2, 1, 3] -> [3, 1, 2]
 *
 *  O início da lista nunca muda (a 1ª ocorrência sempre fica), por isso a
 *  função não precisa retornar nada.
 *
 *  ASSINATURA
 *      void removerDuplicados(NoLista *inicio);
 *
 *  COMO TESTAR
 *      ./testar.sh L09
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
int inList(int val, NoLista *head) {
    while (head) {
            if (val == head->valor) { return 1; }
            head = head->prox;
    }
    return 0;
}

void removerDuplicados(NoLista *inicio) {
    if (!inicio) return;

    // - Vamos criar uma lista de itens unicos 
    NoLista *uniqueHead = novoNo(inicio->valor); // (primeiro item da lista tem o mesmo valor de inicio)
    NoLista *uniqueTail = uniqueHead; // (primeiro item da lista tem o mesmo valor de inicio)
    NoLista *deleteTemp;  // ponteiro auxiliar para deletar itens verificados;
    
    // - Percorremos todo o resto da lista original
    NoLista *current = inicio->prox; // nao precisamos checar o primeiro item
    while (current) {
        
        // se acharmos um valor unico, colocarmos na lista de itens unicos
        if (!inList(current->valor, uniqueHead)) {
            uniqueTail->prox = novoNo(current->valor);
            uniqueTail = uniqueTail->prox;
        }

        // deletamos o que acabamos de checar e passamos para checar o proximo
        deleteTemp = current;
        current = current->prox;
        free(deleteTemp);
    }

    // Conectamos o inicio ao restante de itens unicos da lista
    inicio->prox = uniqueHead->prox; 

    // removemos o primeiro item duplicado que sobrou.
    free(uniqueHead); 
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

void caso(const int *v, int n, const int *esp, int ne, const char *desc) {
    NoLista *l = listaDe(v, n);
    removerDuplicados(l);
    checarLista(l, esp, ne, desc);
    liberarLista(l);
}

int main(void) {
    printf("L09 — removerDuplicados\n");

    secao("Lista vazia");
    removerDuplicados(NULL);
    verificar(1, "não quebra com lista vazia");

    secao("Casos gerais");
    caso((int[]){3, 1, 3, 2, 1, 3}, 6, (int[]){3, 1, 2}, 3, "[3, 1, 3, 2, 1, 3] -> [3, 1, 2]");
    caso((int[]){5, 5, 5, 5}, 4, (int[]){5}, 1, "[5, 5, 5, 5] -> [5]");
    caso((int[]){1, 2, 3}, 3, (int[]){1, 2, 3}, 3, "[1, 2, 3] -> [1, 2, 3] (sem repetidos)");
    caso((int[]){4, 4, 8, 8, 4}, 5, (int[]){4, 8}, 2, "[4, 4, 8, 8, 4] -> [4, 8]");
    caso((int[]){9}, 1, (int[]){9}, 1, "[9] -> [9]");
    caso((int[]){1, 2, 1, 2, 1, 2, 3}, 7, (int[]){1, 2, 3}, 3, "[1, 2, 1, 2, 1, 2, 3] -> [1, 2, 3]");

    return resumo();
}
