# GameResultsTracker

A C program that records the results of games with any number of players, from two-person chess to bridge. Points are free-form; a player wins a game if they have 2 or more points and no other player has more. The output is in Portuguese.

## Input

Everything is read from `stdin`:

1. The list of players, one name per line, ending with a blank line.
2. The games, one line per player in the game (`Name: points`), with a blank line between games.

```
Pierre Zimmermann
Piotr Gawrys

Pierre Zimmermann: 10
Piotr Gawrys: 6

Pierre Zimmermann: 1
Piotr Gawrys: 1
```

Points can be any integer, zero included. More than one player can win the same game (a tie for the highest score), and a game can have no winner (when the highest score is below 2).

## The four parts

The assignment was split into four parts, each with its own program:

| Program | What it does |
| --- | --- |
| `mainA.c` | Reads the list of players and prints how many there are and the total number of characters in their names |
| `mainB.c` | Also reads the games and prints how many games there were and the total number of participations |
| `mainC.c` | Calculates each player's points and wins, in input order |
| `mainD.c` | Same as C, with players sorted by wins, then points, then name |

## Running

Requires `gcc` (or `cmake`).

```sh
sh runTests                         # compiles and runs the tests for every part
gcc mainD.c -o exerciseD && ./exerciseD < games.txt
```

The tests (`testA` to `testD2`) feed each program an input and compare its output with the expected one. They compile with AddressSanitizer, so memory errors make them fail.

## Context

Course project for Programming Lab (Laboratório de Programação), Universidade Aberta, January 2024.

Built through pair programming with a mentor.
