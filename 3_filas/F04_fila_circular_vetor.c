/*
 * ==========================================================================
 *  F04 — FILAS | Fila circular com vetor
 * --------------------------------------------------------------------------
 *  DESAFIO
 *  Implemente uma fila estática CIRCULAR em um vetor de CAP posições.
 *  Convenção OBRIGATÓRIA (o teste confere os campos):
 *    - inicio : índice do primeiro elemento (o próximo a sair)
 *    - fim    : índice onde o PRÓXIMO elemento será inserido
 *    - qtd    : quantidade de elementos
 *    - após inicializar: inicio = fim = qtd = 0
 *    - os índices "dão a volta": (indice + 1) % CAP
 *
 *    void inicializarFC(FilaCircular *f)
 *    int  vaziaFC(FilaCircular *f)                   -> 1 se vazia
 *    int  cheiaFC(FilaCircular *f)                   -> 1 se cheia
 *    int  enfileirarFC(FilaCircular *f, int valor)   -> 1 ok / 0 se cheia
 *    int  desenfileirarFC(FilaCircular *f, int *valor) -> 1 ok / 0 se vazia
 *
 *  COMO TESTAR
 *      ./testar.sh F04
 * ==========================================================================
 */
#include <stdio.h>
#include <stdlib.h>

#define CAP 5

typedef struct {
    int dados[CAP];
    int inicio;
    int fim;
    int qtd;
} FilaCircular;

/* >>>>>>>>>>>>>>>>>>>>>>>>>> SUA SOLUÇÃO AQUI >>>>>>>>>>>>>>>>>>>>>>>>>> */
void inicializarFC(FilaCircular *f) {
    /* TODO */
}

int vaziaFC(FilaCircular *f) {
    /* TODO */
    return 0;
}

int cheiaFC(FilaCircular *f) {
    /* TODO */
    return 0;
}

int enfileirarFC(FilaCircular *f, int valor) {
    /* TODO */
    return 0;
}

int desenfileirarFC(FilaCircular *f, int *valor) {
    /* TODO */
    return 0;
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

int estado(FilaCircular *f, int ini, int fim, int qtd, const char *desc) {
    int ok = f->inicio == ini && f->fim == fim && f->qtd == qtd;
    if (!verificar(ok, desc))
        printf("      esperado: inicio=%d fim=%d qtd=%d | obtido: inicio=%d fim=%d qtd=%d\n",
               ini, fim, qtd, f->inicio, f->fim, f->qtd);
    return ok;
}

int main(void) {
    printf("F04 — fila circular (CAP = %d)\n", CAP);
    FilaCircular f = {{0}, 77, 77, 77};
    int v = 0, ok;

    secao("inicializarFC");
    inicializarFC(&f);
    if (!estado(&f, 0, 0, 0, "inicio = fim = qtd = 0")) return resumo();
    verificar(vaziaFC(&f) == 1 && cheiaFC(&f) == 0, "vazia e não cheia");
    verificar(desenfileirarFC(&f, &v) == 0, "desenfileirar de fila vazia retorna 0");

    secao("Enchendo: 1, 2, 3, 4, 5");
    ok = 1;
    for (int i = 1; i <= CAP; i++) ok = ok && enfileirarFC(&f, i);
    verificar(ok, "as 5 inserções retornam 1");
    estado(&f, 0, 0, 5, "inicio=0, fim=0 (deu a volta), qtd=5");
    verificar(cheiaFC(&f) == 1 && vaziaFC(&f) == 0, "cheia e não vazia");
    verificar(enfileirarFC(&f, 6) == 0, "enfileirar com fila cheia retorna 0");
    estado(&f, 0, 0, 5, "fila cheia não muda após tentativa");

    secao("Removendo 2 elementos");
    int a = -1, b = -1;
    desenfileirarFC(&f, &a);
    desenfileirarFC(&f, &b);
    verificar(a == 1 && b == 2, "saem 1 e 2 (FIFO)");
    estado(&f, 2, 0, 3, "inicio=2, fim=0, qtd=3");

    secao("Inserindo 6 e 7 (usam as posições 0 e 1 — circularidade)");
    verificar(enfileirarFC(&f, 6) == 1 && enfileirarFC(&f, 7) == 1, "as duas inserções retornam 1");
    estado(&f, 2, 2, 5, "inicio=2, fim=2, qtd=5");
    verificar(f.dados[0] == 6 && f.dados[1] == 7, "dados[0] == 6 e dados[1] == 7");
    verificar(cheiaFC(&f) == 1, "fila cheia de novo");

    secao("Esvaziando");
    int saida[CAP] = {0}, n = 0;
    while (n < CAP && desenfileirarFC(&f, &v)) saida[n++] = v;
    checarVetor(saida, n, (int[]){3, 4, 5, 6, 7}, 5, "saem 3, 4, 5, 6, 7");
    estado(&f, 2, 2, 0, "inicio=2, fim=2, qtd=0");
    verificar(vaziaFC(&f) == 1, "fila vazia");
    verificar(desenfileirarFC(&f, &v) == 0, "desenfileirar de fila vazia retorna 0");

    secao("Reutilizando");
    enfileirarFC(&f, 8);
    v = -1;
    verificar(desenfileirarFC(&f, &v) == 1 && v == 8, "enfileira e desenfileira 8");
    estado(&f, 3, 3, 0, "inicio=3, fim=3, qtd=0");

    return resumo();
}
