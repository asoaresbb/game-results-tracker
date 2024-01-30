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
} Players;

Players *readPlayers() {
    Players *players = malloc(sizeof(Players));
    players->numPlayers = 0;
    while (1) {
        char str[100];
        players->players = realloc(players->players, (players->numPlayers + 1) * sizeof(Player));
        const char *fgetsRes = fgets(str, sizeof(str), stdin);
        if (fgetsRes == NULL || str[0] == '\n') return players;
        replaceNewLineByEnfOfStr(str);
        const Player player = {.name = strdup(str)};
        players->players[players->numPlayers] = player;
        players->numPlayers++;
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
} Games;

Player *findPlayer(const Players *players, const char *playerName) {
    for (int i = 0; i < players->numPlayers; i++) {
        if (strcmp(players->players[i].name, playerName) == 0)
            return &players->players[i];
    }
    fprintf(stderr, "player not found %s", playerName);
    exit(1);
}

Game *readGame(const Players *players) {
    Game *game = malloc(sizeof(Game));
    game->playersCount = 0;
    while (1) {
        char str[100];
        game->scores = realloc(game->scores, (game->playersCount + 1) * sizeof(Score));
        const char *fgetsRes = fgets(str, sizeof(str), stdin);
        if (fgetsRes == NULL || str[0] == '\n') {
            if (game->playersCount == 0) return NULL;
            return game;
        }
        replaceNewLineByEnfOfStr(str);
        const char *playerName = strtok(str, ":");
        const int points = atoi(strtok(NULL, ":"));
        game->scores[game->playersCount].player = findPlayer(players, playerName);
        game->scores[game->playersCount].points = points;
        game->playersCount++;
    }
}

void freePlayers(Players *players) {
    for (int i = 0; i < players->numPlayers; i++) {
        free(players->players[i].name);
    }
    free(players->players);
    free(players);
}

void freeGames(Games *games) {
    for (int i = 0; i < games->numGames; i++) {
        free(games->games[i].scores);
    }
    free(games->games);
    free(games);
}

Games *readGames(const Players *players) {
    Games *games = malloc(sizeof(Games));
    games->numGames = 0;
    const Game *game = NULL;
    while ((game = readGame(players)) != NULL) {
        games->games = realloc(games->games, (games->numGames + 1) * sizeof(Game));
        games->games[games->numGames] = *game;
        games->numGames++;
    }
    return games;
}

int calculateMaxScore(const Game game) {
    int maxScore = -INT_MIN;
    for (int s = 0; s < game.playersCount; s++) {
        if (game.scores[s].points > maxScore)
            maxScore = game.scores[s].points;
    }
    return maxScore;
}

void calculateWinCounts(const Games *games) {
    for (int g = 0; g < games->numGames; g++) {
        const Game game = games->games[g];
        const int maxScore = calculateMaxScore(game);
        for (int s = 0; s < game.playersCount; s++) {
            const Score score = game.scores[s];
            if (score.points == maxScore)
                score.player->totalWins++;
        }
    }
}

void calculateTotalPoints(const Games *games) {
    for (int g = 0; g < games->numGames; g++) {
        const Game game = games->games[g];
        for (int p = 0; p < game.playersCount; p++) {
            const Score score = game.scores[p];
            score.player->totalPoints += score.points;
        }
    }
}

int byWinsPointsName(const void *a, const void *b) {
    const Player *playerA = a;
    const Player *playerB = b;
    return playerA->totalWins != playerB->totalWins
               ? playerB->totalWins - playerA->totalWins
               : playerA->totalPoints != playerB->totalPoints
                     ? playerB->totalPoints - playerA->totalPoints
                     : strcmp(playerA->name, playerB->name);
}

typedef int (*CompareFunction)(const void *, const void *);

void sortPlayers(const Players *playersInfo, const CompareFunction compareFunction) {
    qsort(playersInfo->players, playersInfo->numPlayers, sizeof(Player), compareFunction);
}

void printPlayerStats(const Players *players) {
    for (int i = 0; i < players->numPlayers; i++) {
        const Player player = players->players[i];
        printf("%s - %d pontos, %d vitorias.\n", player.name, player.totalPoints, player.totalWins);
    }
}

int main() {
    Players *players = readPlayers();
    Games *games = readGames(players);
    calculateWinCounts(games);
    calculateTotalPoints(games);
    sortPlayers(players, byWinsPointsName);
    printPlayerStats(players);

    freePlayers(players);
    freeGames(games);
}
