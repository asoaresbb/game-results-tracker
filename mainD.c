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
    Player *items;
    int length;
} Players;

Players *readPlayers() {
    Players *players = calloc(1, sizeof(Players));
    while (1) {
        char str[100];
        players->items = realloc(players->items, (players->length + 1) * sizeof(Player));
        const char *fgetsRes = fgets(str, sizeof(str), stdin);
        if (fgetsRes == NULL || str[0] == '\n') return players;
        replaceNewLineByEnfOfStr(str);
        const Player player = {.name = strdup(str)};
        players->items[players->length] = player;
        players->length++;
    }
}

typedef struct {
    Player *player;
    int points;
} Score;

typedef struct {
    Score *scores;
    int numPlayers;
} Game;

typedef struct {
    Game *items;
    int length;
} Games;

Player *findPlayer(const Players *players, const char *playerName) {
    for (int i = 0; i < players->length; i++) {
        if (strcmp(players->items[i].name, playerName) == 0)
            return &players->items[i];
    }
    fprintf(stderr, "player not found %s", playerName);
    exit(1);
}

Game *readGame(const Players *players) {
    Game *game = calloc(1, sizeof(Game));
    while (1) {
        char str[100];
        game->scores = realloc(game->scores, (game->numPlayers + 1) * sizeof(Score));
        const char *fgetsRes = fgets(str, sizeof(str), stdin);
        if (fgetsRes == NULL || str[0] == '\n') {
            if (game->numPlayers == 0) return NULL;
            return game;
        }
        replaceNewLineByEnfOfStr(str);
        const char *playerName = strtok(str, ":");
        const int points = atoi(strtok(NULL, ":"));
        game->scores[game->numPlayers].player = findPlayer(players, playerName);
        game->scores[game->numPlayers].points = points;
        game->numPlayers++;
    }
}

void freePlayers(Players *players) {
    for (int i = 0; i < players->length; i++) {
        free(players->items[i].name);
    }
    free(players->items);
    free(players);
}

void freeGames(Games *games) {
    for (int i = 0; i < games->length; i++) {
        free(games->items[i].scores);
    }
    free(games->items);
    free(games);
}

Games *readGames(const Players *players) {
    Games *games = calloc(1, sizeof(Games));
    const Game *game = NULL;
    while ((game = readGame(players)) != NULL) {
        games->items = realloc(games->items, (games->length + 1) * sizeof(Game));
        games->items[games->length] = *game;
        games->length++;
    }
    return games;
}

int calculateMaxScore(const Game game) {
    int maxScore = -INT_MIN;
    for (int s = 0; s < game.numPlayers; s++) {
        if (game.scores[s].points > maxScore)
            maxScore = game.scores[s].points;
    }
    return maxScore;
}

void calculateWinCounts(const Games *games) {
    for (int g = 0; g < games->length; g++) {
        const Game game = games->items[g];
        const int maxScore = calculateMaxScore(game);
        for (int s = 0; s < game.numPlayers; s++) {
            const Score score = game.scores[s];
            if (score.points == maxScore)
                score.player->totalWins++;
        }
    }
}

void calculateTotalPoints(const Games *games) {
    for (int g = 0; g < games->length; g++) {
        const Game game = games->items[g];
        for (int p = 0; p < game.numPlayers; p++) {
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

void sortPlayers(const Players *players, const CompareFunction comparePlayers) {
    qsort(players->items, players->length, sizeof(Player), comparePlayers);
}

void printPlayerStats(const Players *players) {
    for (int i = 0; i < players->length; i++) {
        const Player player = players->items[i];
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
