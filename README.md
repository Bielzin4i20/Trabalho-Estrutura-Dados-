# Avaliador de Expressões Numéricas  
Projeto da disciplina Estrutura de Dados – 2º semestre de 2025  
Universidade Católica de Brasília (UCB)  
Professor: Marcelo Eustáquio

--------------------------------------------

## Descrição

Este projeto implementa um avaliador de expressões matemáticas usando linguagem C e pilhas.  
O sistema é capaz de:

- Converter expressões pós-fixas para infixas
- Avaliar expressões matemáticas em notação pós-fixa
- Suportar operadores:
  +, -, *, /, %, ^
- Suportar funções:
  raiz, sen, cos, tg, log
- Trabalhar com trigonometria em graus
- Aceitar números com vírgula ou ponto decimal
- Processar expressões de até 512 caracteres

--------------------------------------------

## Estrutura do Projeto

O projeto é composto pelos arquivos:

calculadora.c   -> Implementações (arquivo que deve ser enviado no AVA)
expressao.h     -> Cabeçalho fornecido (não alterar)
main.c          -> Arquivo de testes usado pelo aluno
README.md        -> Documento explicativo

O professor usará o próprio main.c dele para testar.  
No AVA, deve ser enviado apenas **calculadora.c**.

--------------------------------------------

## Funções Principais

### getFormaInFixa(char *Str)
- Recebe uma expressão pós-fixa
- Retorna uma string alocada contendo a versão infixa
- Retorna NULL se ocorrer erro

### getValorPosFixa(char *StrPosFixa)
- Recebe uma expressão pós-fixa
- Retorna o valor numérico em float
- Suporta operadores binários e funções unárias

--------------------------------------------

## Exemplos de expressões pós-fixas aceitas

3 4 + 5 *
7 2 * 4 + 7 * 2 + 4
8 5 2 4 + * + 8 +
45 60 + 30 cos *
0.5 45 sen 2 ^ +
2 3 + log 5 /

--------------------------------------------

## Como Compilar e Executar

### Windows (MinGW / VSCode)

gcc calculadora.c main.c -o calculadora.exe -lm
./calculadora.exe

### Linux / WSL

gcc calculadora.c main.c -o calculadora -lm
./calculadora

### Dev-C++

1. Criar projeto Console C
2. Adicionar calculadora.c, expressao.h e main.c
3. Em Parameters → Linker, adicionar: -lm
4. Compilar e executar

--------------------------------------------

## Saída Esperada (exemplo)

Teste 1:
Pos-fixa: 3 4 + 5 *
Infixa : (3+4)*5
Valor  : 35.000000

--------------------------------------------

## Observações

- O arquivo enviado no AVA deve ser apenas calculadora.c
- expressao.h não deve ser alterado
- A correção automática usará:
  gcc main.c calculadora.c -o calculadora.exe -lm
- getFormaInFixa retorna memória alocada; deve ser liberada com free()

--------------------------------------------

## Autor

Gabriel Lucas Souza Barroso
Análise e Desenvolvimento de Sistemas – UCB
