/*
 * ==========================================================================
 *  P09 — PILHAS | Pilha estática (implementada com vetor)
 * --------------------------------------------------------------------------
 *  DESAFIO
 *  Implemente uma pilha usando um vetor de tamanho fixo MAX.
 *  Convenção OBRIGATÓRIA (o teste confere):
 *    - 'topo' guarda o ÍNDICE do elemento do topo;
 *    - pilha vazia  -> topo == -1
 *    - pilha cheia  -> topo == MAX - 1
 *
 *    void inicializarPV(PilhaVetor *p)              -> deixa a pilha vazia
 *    int  vaziaPV(PilhaVetor *p)                    -> 1 se vazia
 *    int  cheiaPV(PilhaVetor *p)                    -> 1 se cheia
 *    int  empilharPV(PilhaVetor *p, int valor)      -> 1 ok / 0 se cheia (overflow)
 *    int  desempilharPV(PilhaVetor *p, int *valor)  -> 1 ok / 0 se vazia (underflow)
 *
 *  COMO TESTAR
 *      ./testar.sh P09
 * ==========================================================================
 */
#include <stdio.h>
#include <stdlib.h>

#define MAX 5

typedef struct {
    int dados[MAX];
    int topo;
} PilhaVetor;

/* >>>>>>>>>>>>>>>>>>>>>>>>>> SUA SOLUÇÃO AQUI >>>>>>>>>>>>>>>>>>>>>>>>>> */
void inicializarPV(PilhaVetor *p) {
    /* TODO */
}

int vaziaPV(PilhaVetor *p) {
    /* TODO */
    return 0;
}

int cheiaPV(PilhaVetor *p) {
    /* TODO */
    return 0;
}

int empilharPV(PilhaVetor *p, int valor) {
    /* TODO */
    return 0;
}

int desempilharPV(PilhaVetor *p, int *valor) {
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

int main(void) {
    printf("P09 — pilha com vetor (MAX = %d)\n", MAX);
    PilhaVetor p = {{0}, 12345};
    int v = 0, ok;

    secao("inicializarPV");
    inicializarPV(&p);
    if (!verificar(p.topo == -1, "após inicializar, topo == -1")) {
        printf("      topo = %d — os próximos testes dependem disso\n", p.topo);
        return resumo();
    }
    verificar(vaziaPV(&p) == 1, "pilha está vazia");
    verificar(cheiaPV(&p) == 0, "pilha não está cheia");
    verificar(desempilharPV(&p, &v) == 0, "desempilhar de pilha vazia retorna 0 (underflow)");
    verificar(p.topo == -1, "underflow não altera o topo");

    secao("Enchendo a pilha: 10, 20, 30, 40, 50");
    ok = 1;
    for (int i = 1; i <= MAX; i++) ok = ok && empilharPV(&p, i * 10);
    verificar(ok, "as 5 inserções retornam 1");
    verificar(p.topo == MAX - 1, "topo == MAX - 1");
    verificar(p.dados[0] == 10 && p.dados[4] == 50, "dados[0] == 10 e dados[4] == 50");
    verificar(cheiaPV(&p) == 1 && vaziaPV(&p) == 0, "pilha cheia e não vazia");

    secao("Overflow");
    verificar(empilharPV(&p, 60) == 0, "empilhar 60 com pilha cheia retorna 0");
    verificar(p.topo == MAX - 1 && p.dados[4] == 50, "overflow não altera a pilha");

    secao("Desempilhando (LIFO)");
    v = -1;
    verificar(desempilharPV(&p, &v) == 1 && v == 50, "desempilha 50");
    v = -1;
    verificar(desempilharPV(&p, &v) == 1 && v == 40, "desempilha 40");
    verificar(p.topo == 2, "topo == 2");
    verificar(empilharPV(&p, 99) == 1 && p.topo == 3 && p.dados[3] == 99, "empilhar 99 vai para dados[3]");
    int esperado[] = {99, 30, 20, 10}, obtido[4] = {0}, n = 0;
    while (n < 4 && desempilharPV(&p, &v)) obtido[n++] = v;
    checarVetor(obtido, n, esperado, 4, "desempilha 99, 30, 20, 10");
    verificar(vaziaPV(&p) == 1 && p.topo == -1, "pilha vazia no final (topo == -1)");

    return resumo();
}
