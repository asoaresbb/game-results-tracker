#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** signInPlayers(const int numPlayers) {
    char** players = malloc(numPlayers * sizeof(char*));

    for (int i = 0; i < numPlayers; i++) {
        players[i] = (char*)malloc(50 * sizeof(char));
        fgets(players[i], 50, stdin);
        players[i][strcspn(players[i], "\n")] = 0; // Remover a nova linha
    }
    return players;
}

void freePlayers(char** players, const int numPlayers) {
    for (int i = 0; i < numPlayers; i++) {
        free(players[i]);
    }
    free(players);
}

void printTotalPlayers(const int numPlayers) {
    int totalPlayers = 0;
    for (int i = 0; i < numPlayers; i++) {
        totalPlayers++;
    }
    printf("%d", totalPlayers);
}

void printTotalCharacters(char** players, const int numPlayers) {
    int totalCharacters = 0;

    for (int i = 0; i < numPlayers; i++) {
        totalCharacters += strlen(players[i]);
    }
    printf(" %d", totalCharacters);
}

int main() {
    const int numPlayers = 4;

    char** players = signInPlayers(numPlayers);

    printTotalPlayers(numPlayers);
    printTotalCharacters(players ,numPlayers);

    freePlayers(players, numPlayers);

    return 0;
}
