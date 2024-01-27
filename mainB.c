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

struct Game {
    struct Score *scores;
    int playersCount;
};

struct Game *readGame() {
    struct Game *game = malloc(sizeof(struct Game));
    while (1) {
        char str[100];
        game->scores = realloc(game->scores, (game->playersCount + 1) * sizeof(struct Score));
        const char *fgetsRes = fgets(str, sizeof(str), stdin);
        if (fgetsRes == NULL || str[0] == '\n') {
            if (game->playersCount == 0)
                return NULL;
            return game;
        }
        replaceNewLineByEnfOfStr(str);
        const char *name = strtok(str, ":");
        const int points = atoi(strtok(NULL, ":"));
        game->scores[game->playersCount].name = malloc(strlen(name) + 1);
        strcpy(game->scores[game->playersCount].name, name);
        game->scores[game->playersCount].score = points;
        game->playersCount++;
    }
}

void freePlayers(char **players, const int numPlayers) {
    for (int i = 0; i < numPlayers; i++) {
        free(players[i]);
    }
    free(players);
}

void freeGames(struct Game *games, const int numGames) {
    for (int i = 0; i < numGames; i++) {
        for (int j = 0; j < games[i].playersCount; j++) {
            free(games[i].scores[j].name);
        }
        free(games[i].scores);
    }
    free(games);
}

struct Game *readGames(int *numGames) {
    struct Game *games = NULL;
    const struct Game *game = NULL;
    while ((game = readGame()) != NULL) {
        games = realloc(games, (*numGames + 1) * sizeof(struct Game));
        games[*numGames] = *game;
        (*numGames)++;
    }
    return games;
}

void printResult(const struct Game *games, const int numGames) {
    int totalPlayersPlayedGames = 0;
    for (int i = 0; i < numGames; i++) {
        totalPlayersPlayedGames += games[i].playersCount;
    }
    printf("%d %d", numGames, totalPlayersPlayedGames);
}

int main() {
    int numPlayers;
    char **players = readPlayers(&numPlayers);
    int numGames = 0;
    struct Game *games = readGames(&numGames);
    printResult(games, numGames);
    freePlayers(players, numPlayers);
    freeGames(games, numGames);
}
