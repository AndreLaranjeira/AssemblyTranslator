# Assembly translator

## Descrição

Programa para traduzir um arquivo na linguagem assembly inventada da disciplina para assembly intel 32 bits. Trabalho 02 da disciplina Software básico 2018/2 da Universidade de Brasília.

## Integrantes

Nome | Matrícula | Sistema operacional
---  | --- | ---
André Laranjeira | 16/0023777 | Manjaro linux 64-bit
Victor André Gris Costa | 16/0019311 | WSL Ubuntu 14.04 no Windows 10 64-bit

## Compilação e execução do código

Para compilar o código basta executar o comando `make` no diretório raiz do projeto. Para executar e traduzir um programa apenas execute `./tradutor nome_sem_asm`. Nós preparamos alguns arquivos de teste na pasta `./test`. Você pode traduzir, compilar e ligar (obs.: ld -m elf_i386...) todos os arquivos de test usando o comando `make test`. Os *.s e *.out serão gerados na mesma pasta do arquivos *.asm.
