# Usando no Windows

O código dos exercícios é C padrão (só `stdio.h`, `stdlib.h`, `string.h`, `limits.h`) e
compila em qualquer gcc. O que não é nativo do Windows é o `testar.sh` (script bash) e
os sanitizers. Escolha uma das opções abaixo.

## Opção 1 — WSL (recomendado)

Funciona exatamente como no Linux, **com** o teste de vazamento de memória.

No PowerShell como administrador:

```powershell
wsl --install            # instala o Ubuntu; reinicia o PC se pedir
```

Depois, dentro do Ubuntu:

```bash
sudo apt update && sudo apt install build-essential
cd ~                     # clone dentro do Linux, não em /mnt/c (fica bem mais rápido)
git clone <url-do-repo>
cd estudo_aed_av1
chmod +x testar.sh
./testar.sh L01
```

Para editar no VS Code do Windows e compilar no Linux, instale a extensão **WSL** e abra
a pasta com `code .` de dentro do Ubuntu.

## Opção 2 — MSYS2 / MinGW-w64 + Git Bash

O `testar.sh` roda, mas o gcc do MinGW normalmente **não** tem AddressSanitizer. O
script detecta isso, avisa e compila sem ele — os testes continuam valendo, você só
perde o ✗ automático quando esquece um `free`.

1. Instale o [MSYS2](https://www.msys2.org/) e, no terminal dele:
   `pacman -S mingw-w64-ucrt-x86_64-gcc`
2. Adicione `C:\msys64\ucrt64\bin` ao PATH (ou use o próprio terminal "MSYS2 UCRT64").
3. No Git Bash ou no terminal do MSYS2, na pasta do repositório:

```bash
SAN=0 ./testar.sh L01
SAN=0 ./testar.sh todos
```

## Opção 3 — Só o gcc, sem o script

Serve para qualquer gcc no Windows, direto no PowerShell ou no cmd:

```
gcc -std=c11 -Wall -g 1_listas\L01_inserir_no_fim.c -o l01.exe
l01.exe
```

É exatamente o que o script faz — ele só automatiza achar o arquivo pelo ID e rodar
tudo de uma vez.

## Opção 4 — Compilador online

Para resolver uma questão rápido, sem instalar nada: cole o arquivo inteiro em um
compilador online (por exemplo o [Programiz](https://www.programiz.com/c-programming/online-compiler/)
ou o [OnlineGDB](https://www.onlinegdb.com/online_c_compiler)) e rode. Cada arquivo é
autocontido, então funciona. O OnlineGDB aceita `-fsanitize=address` nas opções de
compilação, o que recupera o teste de vazamento.

## Problemas comuns

**Acentos e ✓ / ✗ saem como lixo no cmd.exe**
O console não está em UTF-8. Rode `chcp 65001` antes, ou use o Windows Terminal, o
PowerShell atual ou o Git Bash — nesses já sai certo.

**`./testar.sh: /bin/bash^M: bad interpreter` ou `$'\r': command not found`**
O Git converteu as quebras de linha para CRLF ao clonar. O repositório tem um
`.gitattributes` que evita isso; se você clonou antes dele existir, conserte com:

```bash
git config core.autocrlf false
git rm --cached -r . && git reset --hard
```

**`gcc: command not found` / "não é reconhecido como comando"**
O compilador não está no PATH. No WSL, instale o `build-essential`; no MinGW, confira o
passo 2 da opção 2.

**`bash: ./testar.sh: Permission denied`**
Falta a permissão de execução: `chmod +x testar.sh` (ou rode `bash testar.sh L01`).
