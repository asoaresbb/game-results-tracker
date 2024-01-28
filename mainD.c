#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void replaceNewLineByEnfOfStr(char buffer[]) {
    buffer[strcspn(buffer, "\n")] = '\0';
}


typedef struct {
    char *name;
    int totalWins;
    int totalPoints;
} Player;

typedef struct {
    Player *players;
    int numPlayers;
} PlayersInfo;


PlayersInfo *readPlayers() {
    PlayersInfo *playersInfo = malloc(sizeof(PlayersInfo));
    while (1) {
        char str[100];
        playersInfo->players = realloc(playersInfo->players, (playersInfo->numPlayers + 1) * sizeof(Player));
        const char *fgetsRes = fgets(str, sizeof(str), stdin);
        if (fgetsRes == NULL || str[0] == '\n') return playersInfo;
        replaceNewLineByEnfOfStr(str);
        Player player = {NULL, 0, 0};
        player.name = malloc(strlen(str) + 1);
        strcpy(player.name, str);
        playersInfo->players[playersInfo->numPlayers] = player;
        playersInfo->numPlayers++;
    }
}


typedef struct {
    Player *player;
    int points;
} Score;

typedef struct {
    Score *scores;
    int playersCount;
} Game;

typedef struct {
    Game *games;
    int numGames;
} GamesInfo;

Player *findPlayer(const PlayersInfo *playersInfo, const char *playerName) {
    for (int i = 0; i < playersInfo->numPlayers; i++) {
        if (strcmp(playersInfo->players[i].name, playerName) == 0) {
            return &playersInfo->players[i];
        }
    }
    fprintf(stderr, "player not found %s", playerName);
    exit(1);
}

Game *readGame(const PlayersInfo *playersInfo) {
    Game *game = malloc(sizeof(Game));
    while (1) {
        char str[100];
        game->scores = realloc(game->scores, (game->playersCount + 1) * sizeof(Score));
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

void freePlayers(PlayersInfo *playersInfo) {
    for (int i = 0; i < playersInfo->numPlayers; i++) {
        free(playersInfo->players[i].name);
    }
    free(playersInfo->players);
    free(playersInfo);
}

void freeGames(GamesInfo *gamesInfo) {
    for (int i = 0; i < gamesInfo->numGames; i++) {
        const Game game = gamesInfo->games[i];
        free(game.scores);
    }
    free(gamesInfo->games);
    free(gamesInfo);
}

GamesInfo *readGames(const PlayersInfo *playersInfo) {
    GamesInfo *gamesInfo = malloc(sizeof(GamesInfo));
    const Game *game = NULL;
    while ((game = readGame(playersInfo)) != NULL) {
        gamesInfo->games = realloc(gamesInfo->games, (gamesInfo->numGames + 1) * sizeof(Game));
        gamesInfo->games[gamesInfo->numGames] = *game;
        gamesInfo->numGames++;
    }
    return gamesInfo;
}

int calculateMaxScore(const Game game) {
    int maxScore = -INT_MIN;
    for (int s = 0; s < game.playersCount; s++) {
        if (game.scores[s].points > maxScore) {
            maxScore = game.scores[s].points;
        }
    }
    return maxScore;
}

void calculateWinCounts(const GamesInfo *gamesInfo) {
    for (int g = 0; g < gamesInfo->numGames; g++) {
        const Game game = gamesInfo->games[g];
        const int maxScore = calculateMaxScore(game);
        for (int s = 0; s < game.playersCount; s++) {
            const Score score = game.scores[s];
            if (score.points == maxScore) {
                score.player->totalWins++;
            }
        }
    }
}


void calculateTotalPoints(const GamesInfo *gamesInfo) {
    for (int g = 0; g < gamesInfo->numGames; g++) {
        const Game game = gamesInfo->games[g];
        for (int p = 0; p < game.playersCount; p++) {
            const Score score = game.scores[p];
            score.player->totalPoints += score.points;
        }
    }
}

int comparePlayers(const void *a, const void *b) {
    const Player *playerA = a;
    const Player *playerB = b;
    if (playerA->totalWins != playerB->totalWins) {
        return playerB->totalWins - playerA->totalWins;
    }
    if (playerA->totalPoints != playerB->totalPoints) {
        return playerB->totalPoints - playerA->totalPoints;
    }
    return strcmp(playerA->name, playerB->name);
}

void sortPlayers(const PlayersInfo *playersInfo) {
    qsort(playersInfo->players, playersInfo->numPlayers, sizeof(Player), comparePlayers);
}

void printPlayerStats(const PlayersInfo *playersInfo) {
    for (int i = 0; i < playersInfo->numPlayers; i++) {
        const Player player = playersInfo->players[i];
        printf("%s - %d pontos, %d vitorias.\n", player.name, player.totalPoints, player.totalWins);
    }
}

int main() {
    PlayersInfo *playersInfo = readPlayers();
    GamesInfo *games = readGames(playersInfo);
    calculateWinCounts(games);
    calculateTotalPoints(games);
    sortPlayers(playersInfo);
    printPlayerStats(playersInfo);
    freePlayers(playersInfo);
    freeGames(games);
}
