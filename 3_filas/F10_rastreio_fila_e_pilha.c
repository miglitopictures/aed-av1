/*
 * ==========================================================================
 *  F10 — FILAS + PILHAS | Rastreio de operações (questão de papel)
 * --------------------------------------------------------------------------
 *  DESAFIO
 *  PARTE 1 — Uma FILA começa vazia e executa, nesta ordem:
 *
 *     1) enfileirar(4)      6) enfileirar(2)
 *     2) enfileirar(7)      7) desenfileirar()
 *     3) enfileirar(1)      8) desenfileirar()
 *     4) desenfileirar()    9) enfileirar(5)
 *     5) enfileirar(9)     10) desenfileirar()
 *
 *  a) Qual a sequência de valores removidos (na ordem em que saem)?
 *  b) Qual o conteúdo final da fila (do INÍCIO para o FIM)?
 *
 *  PARTE 2 — Em seguida, desenfileira-se TODOS os elementos restantes da
 *  fila, empilhando cada um numa PILHA vazia. Depois, desempilha-se tudo.
 *
 *  c) Qual a sequência de valores desempilhados?
 *
 *  PARTE 3 — Uma pilha vazia recebe empilhar(1), empilhar(2), empilhar(3),
 *  empilhar(4). Depois, repete-se 2 vezes: "desempilhar um valor e
 *  enfileirá-lo em uma fila inicialmente vazia". Por fim, desenfileira-se
 *  um valor da fila e empilha-se ele de volta.
 *
 *  d) Conteúdo final da pilha (BASE -> TOPO)?
 *  e) Conteúdo final da fila (INÍCIO -> FIM)?
 *
 *  COMO TESTAR
 *      ./testar.sh F10
 *  Para ver o gabarito das que errou, mude MOSTRAR_GABARITO para 1.
 * ==========================================================================
 */
#include <stdio.h>
#include <stdlib.h>

#define MOSTRAR_GABARITO 0

/* >>>>>>>>>>>>>>>>>>>>>>>>>> SUA SOLUÇÃO AQUI >>>>>>>>>>>>>>>>>>>>>>>>>> */
int a_removidos[]   = { 0 };
int b_filaFinal[]   = { 0 };   /* início -> fim */
int c_desempilhados[] = { 0 };
int d_pilhaFinal[]  = { 0 };   /* base -> topo */
int e_filaFinal[]   = { 0 };   /* início -> fim */
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

/* simulação usada para calcular o gabarito */
int fila[64], fi = 0, ff = 0;          /* fila: [fi, ff) */
int pilha[64], topo = -1;

void conferir(const int *sua, int ns, const int *certo, int nc, const char *desc) {
    if (!verificar(vetoresIguais(sua, ns, certo, nc), desc)) {
        printf("      sua resposta: "); imprimirVetor(sua, ns); printf("\n");
        if (MOSTRAR_GABARITO) { printf("      gabarito:     "); imprimirVetor(certo, nc); printf("\n"); }
    }
}

int main(void) {
    printf("F10 — rastreio de fila e pilha\n");
    const int OPS[] = {4, 7, 1, 0, 9, 2, 0, 0, 5, 0};   /* 0 = desenfileirar */
    int rem[16], nr = 0, des[16], nd = 0;

    for (int i = 0; i < TAM(OPS); i++) {
        if (OPS[i] != 0) fila[ff++] = OPS[i];
        else rem[nr++] = fila[fi++];
    }
    int filaFinal[16], nf = 0;
    for (int i = fi; i < ff; i++) filaFinal[nf++] = fila[i];

    while (fi < ff) pilha[++topo] = fila[fi++];
    while (topo >= 0) des[nd++] = pilha[topo--];

    /* parte 3 */
    fi = ff = 0; topo = -1;
    for (int v = 1; v <= 4; v++) pilha[++topo] = v;
    for (int k = 0; k < 2; k++) fila[ff++] = pilha[topo--];
    pilha[++topo] = fila[fi++];

    secao("Parte 1");
    conferir(a_removidos, TAM(a_removidos), rem, nr, "a) sequência de removidos");
    conferir(b_filaFinal, TAM(b_filaFinal), filaFinal, nf, "b) fila final (início -> fim)");
    secao("Parte 2");
    conferir(c_desempilhados, TAM(c_desempilhados), des, nd, "c) sequência desempilhada");
    secao("Parte 3");
    conferir(d_pilhaFinal, TAM(d_pilhaFinal), pilha, topo + 1, "d) pilha final (base -> topo)");
    conferir(e_filaFinal, TAM(e_filaFinal), fila + fi, ff - fi, "e) fila final (início -> fim)");

    return resumo();
}
