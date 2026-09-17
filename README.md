# Praticando AED
> Listas, Pilhas, Filas e Arvores Binárias de Busca.

40 exercícios em C com **testes automáticos**. Gerados pelo Claude Opus 5 a partir dos simulados e da prova AV1 de **Algoritmos e Estruturas de Dados** (3º período de ADS — CESAR School).

**Sem soluções neste repositório**, de propósito. As minhas ficam em um fork.

## Como usar

```bash
chmod +x testar.sh     # só na primeira vez
./testar.sh L01        # compila e roda uma questão
./testar.sh todos      # placar geral
./testar.sh listar     # lista as questões
```

Cada `.c` é autocontido: enunciado no topo, structs prontas, um bloco
`SUA SOLUÇÃO AQUI` (a única parte que você edita) e um `main` que testa e mostra ✓ / ✗
com o esperado x o obtido.

Compila com AddressSanitizer: `free` esquecido vira ✗ no teste de vazamento, no fim.
Se o seu gcc não tiver suporte, use `SAN=0 ./testar.sh L01`.

Nas questões de papel (P05, P10, F10, A08–A10) você preenche vetores com as respostas —
nas de ABB, escreva a **pré-ordem** da árvore que desenhou. Para ver o gabarito das que
errou, troque `MOSTRAR_GABARITO` para `1`.

**Convenções** (também no topo de cada arquivo): altura conta arestas (vazia = −1, só a
raiz = 0); ABB não aceita repetidos; nó com dois filhos é substituído pelo menor valor
da subárvore direita; quem remove, libera.

## Questões

| | Listas | Pilhas | Filas | ABB |
|---|---|---|---|---|
| 01 | inserir no fim | implementar pilha | implementar fila | inserir e buscar |
| 02 | remover 1ª ocorrência | parênteses balanceados | intercalar filas | copiar árvore |
| 03 | concatenar listas | copiar pilha | inverter fila com pilha | nós com um filho |
| 04 | inverter lista | inverter pilha | fila circular em vetor | nós internos |
| 05 | remover todas ocorrências | V ou F (teoria) | remover valor da fila | verificar se é ABB |
| 06 | inserção ordenada | expressão pós-fixa | separar pares/ímpares | altura e folhas |
| 07 | intercalar ordenadas | ordenar pilha | concatenar filas O(1) | remoção em ABB |
| 08 | copiar lista | decimal → binário | filas iguais | papel: ABB (simulado 2026.2) |
| 09 | remover duplicados | pilha em vetor | mover para a frente | papel: ABB (simulado 2) |
| 10 | lista duplamente encadeada | papel: rastreio | papel: rastreio | papel: ABB (prova + inédita) |

Caíram nas avaliações: L02, L03, F02, P05, A02, A03, A04, A05, A08, A09, A10.
O resto são variações e clássicos do tema.

## Como os testes foram validados

Cada questão tem uma solução de referência que passa em 100% dos testes (com
`-Wall -Werror` + ASan/UBSan); o esqueleto vazio falha sem quebrar; soluções
propositalmente erradas (esquecer `free`, não atualizar o `fim`, checar ABB só entre pai
e filho, alterar as filas originais) foram usadas para confirmar que os testes pegam
esses erros; os gabaritos das questões de papel foram feitos à mão e conferidos com o
código.

## Contribuir

Novas questões no mesmo formato e correções de enunciado são bem-vindas.
**PRs com soluções não serão aceitos aqui.**

---

*Material não oficial, feito com apoio de llms por um aluno (eu), sem vínculo com a coordenação ou o corpo docente
da CESAR School. Enunciados adaptados dos simulados e provas, apenas para estudo.*
