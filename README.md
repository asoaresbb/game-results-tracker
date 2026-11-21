# GameResultsTracker

Programa em C que regista os resultados de jogos com qualquer número de jogadores, do xadrez a duas pessoas ao bridge. Os pontos são livres; ganha quem tiver 2 ou mais pontos e a pontuação mais alta do jogo.

## As quatro alíneas

O enunciado (`Enunciado.pdf`) está dividido em alíneas, e cada uma tem o seu programa. Todos leem do `stdin`.

| Programa | O que faz |
| --- | --- |
| `mainA.c` | Lê a lista de jogadores e indica quantos são e o total de caracteres dos nomes |
| `mainB.c` | Lê também os jogos e indica quantos jogos houve e o total de participações |
| `mainC.c` | Calcula os pontos e as vitórias de cada jogador |
| `mainD.c` | Igual à C, com os jogadores ordenados por vitórias, pontos e nome |

## Como correr

Requer `gcc` (ou `cmake`).

```sh
sh runTests                         # compila e corre os testes de todas as alíneas
gcc mainD.c -o exerciseD && ./exerciseD < jogos.txt
```

Os testes (`testA` a `testD`) comparam a saída de cada programa com a saída esperada do enunciado.

## Contexto

Projeto da unidade curricular de Laboratório de Programação, Universidade Aberta, janeiro de 2024.

Desenvolvido em programação a pares com um mentor.
