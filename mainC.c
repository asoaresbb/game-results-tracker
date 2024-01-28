#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void replaceNewLineByEnfOfStr(char buffer[]) {
    buffer[strcspn(buffer, "\n")] = '\0';
}


struct Player {
    char *name;
    int countWins;
    int totalPoints;
};


struct Player *readPlayers(int *numPlayers) {
    struct Player *players = NULL;
    *numPlayers = 0;
    while (1) {
        char str[100];
        players = realloc(players, (*numPlayers + 1) * sizeof(struct Player));
        const char *fgetsRes = fgets(str, sizeof(str), stdin);
        if (fgetsRes == NULL || str[0] == '\n') return players;
        replaceNewLineByEnfOfStr(str);
        struct Player player = {};
        player.name = malloc(strlen(str) + 1);
        strcpy(player.name, str);
        players[*numPlayers] = player;
        (*numPlayers)++;
    }
}


struct Score {
    struct Player *player;
    int points;
};

struct Game {
    struct Score *scores;
    int playersCount;
    char *winner;
};

struct Player *findPlayer(struct Player *players, int numPlayers, const char *playerName) {
    for (int i = 0; i < numPlayers; i++) {
        if (strcmp(players[i].name, playerName) == 0) {
            return &players[i];
        }
    }
    fprintf(stderr, "player not found %s", playerName);
    exit(1);
}

struct Game *readGame(struct Player *players, const int numPlayers) {
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
        const char *playerName = strtok(str, ":");
        const int points = atoi(strtok(NULL, ":"));
        game->scores[game->playersCount].player = findPlayer(players, numPlayers, playerName);
        game->scores[game->playersCount].points = points;
        game->playersCount++;
    }
}

void freePlayers(struct Player *players, const int numPlayers) {
    for (int i = 0; i < numPlayers; i++) {
        free(players[i].name);
    }
    free(players);
}

void freeGames(struct Game *games, const int numGames) {
    for (int i = 0; i < numGames; i++) {
        const struct Game game = games[i];
        free(game.scores);
    }
    free(games);
}

struct Game *readGames(int *numGames, struct Player *players, const int numPlayers) {
    struct Game *games = NULL;
    const struct Game *game = NULL;
    while ((game = readGame(players, numPlayers)) != NULL) {
        games = realloc(games, (*numGames + 1) * sizeof(struct Game));
        games[*numGames] = *game;
        (*numGames)++;
    }
    return games;
}

void calculateMaxScores(const struct Game *games, const int numGames) {
    for (int g = 0; g < numGames; g++) {
        const struct Game game = games[g];
        int maxScore = -INT_MIN;
        for (int s = 0; s < game.playersCount; s++) {
            if (game.scores[s].points > maxScore) {
                maxScore = game.scores[s].points;
            }
        }
    }
}


void calculateTotalPoints(const struct Game *games, const int numGames) {
    for (int g = 0; g < numGames; g++) {
        const struct Game game = games[g];
        for (int p = 0; p < game.playersCount; p++) {
            const struct Score score = game.scores[p];
            struct Player *playerInGame = score.player;
            playerInGame->totalPoints += score.points;
        }
    }
}

void printPlayerStats(const struct Player *players, const int numPlayers, struct Game *game, int numGames) {
    for (int i = 0; i < numPlayers; i++) {
        const struct Player player = players[i];
        printf("%s - %d pontos\n", player.name, player.totalPoints);
    }
}

int main() {
    int numPlayers;
    struct Player *players = readPlayers(&numPlayers);
    int numGames = 0;
    struct Game *games = readGames(&numGames, players, numPlayers);
    // calculateMaxScores(games, numGames);
    calculateTotalPoints(games, numGames);
    printPlayerStats(players, numPlayers, games, numGames);
    freePlayers(players, numPlayers);
    freeGames(games, numGames);
}
