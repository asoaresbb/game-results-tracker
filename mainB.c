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
        game = realloc(game, (*numPlayersInGame + 1) * sizeof(struct Score *));
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


void freeGames(struct Score **games, const int numGames, const int numPlayersPerGame) {
    for (int i = 0; i < numGames; i++) {
        for (int ii = 0; ii < numPlayersPerGame; ii++) {
            free(games[i][ii].name);
        }
        free(games[i]);
    }
    free(games);
}

int main() {
    int numPlayers;
    char **players = readPlayers(&numPlayers);
    int numGames = 0;
    int totalPlayersPlayedGames = 0;
    int numPlayersPerGame;
    struct Score *game;
    struct Score **games = NULL;
    while ((game = readGame(&numPlayersPerGame)) != NULL) {
        games = realloc(games, (numGames + 1) * sizeof(struct Score *));
        games[numGames] = game;
        totalPlayersPlayedGames += numPlayersPerGame;
        numGames++;
    }
    printf("%d %d", numGames, totalPlayersPlayedGames);
    free(game);
    freeGames(games, numGames, numPlayersPerGame);
    freePlayers(players, numPlayers);
}
