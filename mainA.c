#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **signInPlayers(int *numPlayers) {
    char **players = NULL;
    char buffer[50];
    *numPlayers = 0;

    while (1) {
        // Allocate memory for the next player
        players = realloc(players, (*numPlayers + 1) * sizeof(char *));
        if (fgets(buffer, sizeof(buffer), stdin) == NULL || buffer[0] == '\n') {
            break;
        }

        buffer[strcspn(buffer, "\n")] = 0;

        // Allocate memory for the player's name
        players[*numPlayers] = malloc(strlen(buffer) + 1);

        // Copy the name to the allocated memory
        strcpy(players[*numPlayers], buffer);

        (*numPlayers)++;
    }

    return players;
}

void freePlayers(char **players, const int numPlayers) {
    for (int i = 0; i < numPlayers; i++) {
        free(players[i]);
    }
    free(players);
}

void printTotalPlayers(const int numPlayers) {
    printf("%d", numPlayers);
}

void printTotalCharacters(char **players, const int numPlayers) {
    int totalCharacters = 0;
    for (int i = 0; i < numPlayers; i++) {
        totalCharacters += strlen(players[i]);
    }
    printf(" %d", totalCharacters);
}

int main() {
    int numPlayers;

    char **players = signInPlayers(&numPlayers);

    printTotalPlayers(numPlayers);
    printTotalCharacters(players, numPlayers);

    freePlayers(players, numPlayers);

    return 0;
}
