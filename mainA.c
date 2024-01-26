#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void replaceNewLineByEnfOfStr(char buffer[]) {
    buffer[strcspn(buffer, "\n")] = '\0';
}

char **readPlayers(int *numPlayers) {
    char **players = NULL;
    *numPlayers = 0;
    while (1) {
        char str[50];
        players = realloc(players, (*numPlayers + 1) * sizeof(char *));
        const char *fgetsRes = fgets(str, sizeof(str), stdin);
        if (fgetsRes == NULL || str[0] == '\n') return players;
        replaceNewLineByEnfOfStr(str);
        players[*numPlayers] = malloc(strlen(str) + 1);
        strcpy(players[*numPlayers], str);
        (*numPlayers)++;
    }
}

void freePlayers(char **players, const int numPlayers) {
    for (int i = 0; i < numPlayers; i++) {
        free(players[i]);
    }
    free(players);
}


void printResult(char **players, const int numPlayers) {
    int totalCharacters = 0;
    for (int i = 0; i < numPlayers; i++) {
        totalCharacters += strlen(players[i]);
    }
    printf("%d %d", numPlayers, totalCharacters);
}

int main() {
    int numPlayers;
    char **players = readPlayers(&numPlayers);
    printResult(players, numPlayers);
    freePlayers(players, numPlayers);
    return 0;
}
