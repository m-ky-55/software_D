#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "konemain_koyo.h"

int holeRate;   // ← これを追加！！

int main() {
    char command[32];

    if (scanf("%s", command) != 1) return 0;

    if (strcmp(command, "init") == 0) {
        scanf("%d %d %d %d", &YOKO, &TATE, &ren, &holeRate);
        initBoard(holeRate);
        printf("OK\n");
    }
    else if (strcmp(command, "drop") == 0) {
        int col;
        char player;
        scanf("%d %c", &col, &player);
        dropPiece(col, player);
        printf("OK\n");
    }
    else if (strcmp(command, "board") == 0) {
        for (int r = 0; r < TATE; r++) {
            for (int c = 0; c < YOKO; c++) {
                printf("%c", board[r][c]);
            }
            printf("\n");
        }
    }
    else if (strcmp(command, "check") == 0) {
        char winner = checkWin(ren);
        if (winner)
            printf("%c\n", winner);
        else
            printf("0\n");
    }

    return 0;
}
