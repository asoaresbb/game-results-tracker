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
    int scoreCount;
};


struct Game* readGame() {
    struct Score *scores = NULL;
    int numPlayersInGame = 0;
    while (1) {
        char str[100];
        scores = realloc(scores, (numPlayersInGame + 1) * sizeof(struct Score *));
        const char *fgetsRes = fgets(str, sizeof(str), stdin);
        if (fgetsRes == NULL || str[0] == '\n') {
            if (numPlayersInGame == 0)
                return NULL;
            struct Game* game = malloc(sizeof(struct Game));
            game->scores = scores;
            game->scoreCount = numPlayersInGame;
            return game;
        }
        replaceNewLineByEnfOfStr(str);
        const char *name = strtok(str, ":");
        const int points = atoi(strtok(NULL, ":"));
        scores[numPlayersInGame].name = malloc(strlen(name) + 1);
        strcpy(scores[numPlayersInGame].name, str);
        scores[numPlayersInGame].score = points;
        numPlayersInGame++;
    }
}


void freePlayers(char **players, const int numPlayers) {
    for (int i = 0; i < numPlayers; i++) {
        free(players[i]);
    }
    free(players);
}


void freeGames(struct Game **games, const int numGames) {
    for (int g = 0; g < numGames; g++) {
        struct Game* game = games[g];
        for (int s = 0; s < games[g]->scoreCount; s++) {
            // free(game->scores[s].name);
            // free(game[s].scores);
        }
        free(game);
    }
    free(games);
}

int main() {
    int numPlayers;
    char **players = readPlayers(&numPlayers);
    int numGames = 0;
    int totalPlayersPlayedGames = 0;
    struct Game *game;
    struct Game **games = NULL;
    while ((game = readGame()) != NULL) {
        games = realloc(games, (numGames + 1) * sizeof(struct Score *));
        games[numGames] = game;
        totalPlayersPlayedGames += game->scoreCount;
        numGames++;
    }
    printf("%d %d", numGames, totalPlayersPlayedGames);
    free(game);
    freeGames(games, numGames);
    freePlayers(players, numPlayers);
}
