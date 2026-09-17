#!/usr/bin/env bash
# ============================================================================
#  testar.sh — compila e roda as questões de estudo de AED
#
#  Uso:
#    ./testar.sh L01          compila e roda a questão L01 (L, P, F ou A + número)
#    ./testar.sh todos        roda todas e mostra um placar resumido
#    ./testar.sh listar       lista as questões disponíveis
#
#  Por padrão compila com AddressSanitizer + UndefinedBehaviorSanitizer,
#  que acusam acesso inválido à memória e vazamentos (free esquecido).
#  Para desligar:   SAN=0 ./testar.sh L01
# ============================================================================
set -u
cd "$(dirname "$0")" || exit 1

CC="${CC:-gcc}"
CFLAGS="-std=c11 -Wall -g"
SANFLAGS="-fsanitize=address,undefined -fno-omit-frame-pointer"

if [ "${SAN:-1}" = "1" ]; then
    if echo 'int main(void){return 0;}' | $CC $SANFLAGS -x c - -o /tmp/.aed_san_teste 2>/dev/null; then
        CFLAGS="$CFLAGS $SANFLAGS"
    else
        echo "⚠ seu $CC não tem suporte a sanitizers (no Fedora: sudo dnf install libasan libubsan)."
        echo "  Compilando sem eles — vazamentos de memória NÃO serão detectados."
        echo
    fi
    rm -f /tmp/.aed_san_teste
fi

mkdir -p build

compilar() {   # $1 = arquivo .c  -> binário em build/
    local nome
    nome="$(basename "$1" .c)"
    $CC $CFLAGS "$1" -o "build/$nome"
}

achar() {      # $1 = id (ex.: L01) -> caminho do .c
    local id
    id="$(echo "$1" | tr '[:lower:]' '[:upper:]')"
    ls ./*/"${id}"_*.c 2>/dev/null | head -n 1
}

case "${1:-}" in
    ""|-h|--help)
        sed -n '2,14p' "$0" | sed 's/^# \{0,1\}//'
        exit 1
        ;;
    listar)
        for f in ./*/*.c; do echo "  $(basename "$f" .c)"; done
        ;;
    todos)
        ok=0; total=0
        for f in ./*/*.c; do
            nome="$(basename "$f" .c)"
            total=$((total + 1))
            if ! compilar "$f" 2>/dev/null; then
                printf "  ✗ %-40s (não compila)\n" "$nome"
                continue
            fi
            saida="$("./build/$nome" 2>/dev/null)"
            placar="$(echo "$saida" | grep -o 'RESULTADO: [0-9]*/[0-9]*' | cut -d' ' -f2)"
            if echo "$saida" | grep -q 'ACERTOU'; then
                ok=$((ok + 1))
                printf "  ✓ %-40s %s\n" "$nome" "$placar"
            else
                printf "  ✗ %-40s %s\n" "$nome" "${placar:-(travou)}"
            fi
        done
        echo "────────────────────────────────────────────────────"
        echo "  $ok/$total questões completas"
        ;;
    *)
        arq="$(achar "$1")"
        if [ -z "$arq" ]; then
            echo "Questão '$1' não encontrada. Use ./testar.sh listar"
            exit 1
        fi
        echo "▶ compilando $arq"
        compilar "$arq" || { echo "✗ erro de compilação"; exit 2; }
        echo
        "./build/$(basename "$arq" .c)"
        ;;
esac
