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
        char str[100];
        players = realloc(players, (*numPlayers + 1) * sizeof(char *));
        const char *fgetsRes = fgets(str, sizeof(str), stdin);
        if (fgetsRes == NULL || str[0] == '\n') return players;
        replaceNewLineByEnfOfStr(str);
        players[*numPlayers] = malloc(strlen(str) + 1);
        strcpy(players[*numPlayers], str);
        (*numPlayers)++;
    }
}

struct Score {
    char *name;
    int score;
};


struct Score *readGame(int *numPlayersInGame) {
    struct Score *game = NULL;
    *numPlayersInGame = 0;
    while (1) {
        char str[100];
        game = realloc(game, (*numPlayersInGame + 1) * sizeof(struct Score));
        const char *fgetsRes = fgets(str, sizeof(str), stdin);
        if (fgetsRes == NULL || str[0] == '\n') {
            if (*numPlayersInGame == 0)
                return NULL;
            return game;
        }
        replaceNewLineByEnfOfStr(str);
        const char *name = strtok(str, ":");
        const int points = atoi(strtok(NULL, ":"));
        game[*numPlayersInGame].name = malloc(strlen(name) + 1);
        strcpy(game[*numPlayersInGame].name, str);
        game[*numPlayersInGame].score = points;
        (*numPlayersInGame)++;
    }
}


void freePlayers(char **players, const int numPlayers) {
    for (int i = 0; i < numPlayers; i++) {
        free(players[i]);
    }
    free(players);
}


void freeGame(struct Score *game, int numPlayersInGame) {
    for (int i = 0; i < numPlayersInGame; i++) {
        free(game[i].name);
    }
    free(game);
}

int main() {
    int numPlayers;
    char **players = readPlayers(&numPlayers);
    int numPlayersInGame;
    struct Score *game;
    while ((game = readGame(&numPlayersInGame)) != NULL) {
        printf("%s %d", game->name, game->score);
    }
    freeGame(game, numPlayersInGame);
    freePlayers(players, numPlayers);
    return 0;
}
