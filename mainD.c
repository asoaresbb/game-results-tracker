#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void replaceNewLineByEnfOfStr(char buffer[]) {
    buffer[strcspn(buffer, "\n")] = '\0';
}


struct Player {
    char *name;
    int totalWins;
    int totalPoints;
};

struct PlayersInfo {
    struct Player *players;
    int numPlayers;
};


struct PlayersInfo readPlayers() {
    struct PlayersInfo playersInfo = {NULL,0};
    while (1) {
        char str[100];
        playersInfo.players = realloc(playersInfo.players, (playersInfo.numPlayers + 1) * sizeof(struct Player));
        const char *fgetsRes = fgets(str, sizeof(str), stdin);
        if (fgetsRes == NULL || str[0] == '\n') return playersInfo;
        replaceNewLineByEnfOfStr(str);
        struct Player player = {};
        player.name = malloc(strlen(str) + 1);
        strcpy(player.name, str);
        playersInfo.players[playersInfo.numPlayers] = player;
        playersInfo.numPlayers++;
    }
}


struct Score {
    struct Player *player;
    int points;
};

struct Game {
    struct Score *scores;
    int playersCount;
};

struct Player *findPlayer(struct PlayersInfo playersInfo, const char *playerName) {
    for (int i = 0; i < playersInfo.numPlayers; i++) {
        if (strcmp(playersInfo.players[i].name, playerName) == 0) {
            return &playersInfo.players[i];
        }
    }
    fprintf(stderr, "player not found %s", playerName);
    exit(1);
}

struct Game *readGame(struct PlayersInfo playersInfo) {
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
        game->scores[game->playersCount].player = findPlayer(playersInfo, playerName);
        game->scores[game->playersCount].points = points;
        game->playersCount++;
    }
}

void freePlayers(struct PlayersInfo playersInfo) {
    for (int i = 0; i < playersInfo.numPlayers; i++) {
        free(playersInfo.players[i].name);
    }
    free(playersInfo.players);
}

void freeGames(struct Game *games, const int numGames) {
    for (int i = 0; i < numGames; i++) {
        const struct Game game = games[i];
        free(game.scores);
    }
    free(games);
}

struct Game *readGames(int *numGames, struct PlayersInfo playersInfo) {
    struct Game *games = NULL;
    const struct Game *game = NULL;
    while ((game = readGame(playersInfo)) != NULL) {
        games = realloc(games, (*numGames + 1) * sizeof(struct Game));
        games[*numGames] = *game;
        (*numGames)++;
    }
    return games;
}

int calculateMaxScore(const struct Game game) {
    int maxScore = -INT_MIN;
    for (int s = 0; s < game.playersCount; s++) {
        if (game.scores[s].points > maxScore) {
            maxScore = game.scores[s].points;
        }
    }
    return maxScore;
}

void calculateWinCounts(const struct Game *games, const int numGames) {
    for (int g = 0; g < numGames; g++) {
        const struct Game game = games[g];
        const int maxScore = calculateMaxScore(game);
        for (int s = 0; s < game.playersCount; s++) {
            const struct Score score = game.scores[s];
            if (score.points == maxScore) {
                score.player->totalWins++;
            }
        }
    }
}


void calculateTotalPoints(const struct Game *games, const int numGames) {
    for (int g = 0; g < numGames; g++) {
        const struct Game game = games[g];
        for (int p = 0; p < game.playersCount; p++) {
            const struct Score score = game.scores[p];
            score.player->totalPoints += score.points;
        }
    }
}

int comparePlayers(const void *a, const void *b) {
    const struct Player *playerA = a;
    const struct Player *playerB = b;
    if (playerA->totalWins != playerB->totalWins) {
        return playerB->totalWins - playerA->totalWins;
    }
    if (playerA->totalPoints != playerB->totalPoints) {
        return playerB->totalPoints - playerA->totalPoints;
    }
    return strcmp(playerA->name, playerB->name);
}

void sortPlayers(struct PlayersInfo playersInfo) {
    qsort(playersInfo.players, playersInfo.numPlayers, sizeof(struct Player), comparePlayers);
}

void printPlayerStats(struct PlayersInfo playersInfo) {
    for (int i = 0; i < playersInfo.numPlayers; i++) {
        const struct Player player = playersInfo.players[i];
        printf("%s - %d pontos, %d vitorias.\n", player.name, player.totalPoints, player.totalWins);
    }
}

int main() {
    const struct PlayersInfo playersInfo = readPlayers();
    int numGames = 0;
    struct Game *games = readGames(&numGames, playersInfo);
    calculateWinCounts(games, numGames);
    calculateTotalPoints(games, numGames);
    sortPlayers(playersInfo);
    printPlayerStats(playersInfo);
    freePlayers(playersInfo);
    freeGames(games, numGames);
}
