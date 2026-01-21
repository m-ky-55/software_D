#include "konemain_koyo.h"

//----------------------------------------------
// メイン
//----------------------------------------------
int main() {
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);

    char path[256];
    snprintf(path, sizeof(path), "saves/%04d%02d%02d%02d%02d%02d",
             tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour,
             tm->tm_min, tm->tm_sec);

    createSaveFolder();
    createSaveGameFolder(path);

    int menu;
    printf("1: 新規ゲーム\n");
    printf("2: ロードして再開\n> ");
    scanf("%d", &menu);
    if (menu != 1 && menu != 2) {
        printf("無効な入力です。はじめからやり直してください。\n");
        return 0;
    }

    char turn = 'O';

    if (menu == 2) {
        char fname[256];
        char input[256];
        printf(
            "saves以下の相対パスを入力してください。（例：20251332259900/"
            "20251332259905.txt）\n> ");
        if (scanf("%255s", input) != 1) input[0] = '\0';

        /* 既に "saves/" が付いていればそのまま、なければ付けて fname に格納 */
        if (strncmp(input, "saves/", 6) == 0) {
            strncpy(fname, input, sizeof(fname));
            fname[sizeof(fname) - 1] = '\0';
        } else {
            snprintf(fname, sizeof(fname), "saves/%s", input);
        }

        if (!loadGame(fname, &turn)) {
            printf("ロードに失敗したので新規ゲームを開始します\n");
            menu = 1;  // 新規ゲームに切り替え
        } else {
            printf("ロード完了\n");
            /* consume leftover newline from scanf */
            {
                int _ch;
                while ((_ch = getchar()) != '\n' && _ch != EOF);
            }
        }
    }

    if (menu == 1 || menu != 2) {
        /*printf("盤面サイズを選択してください:\n");
        printf("1: 7x6\n2: 10x8\n3: 6x4\n> ");

        int choice;
        while (1) {
            scanf("%d", &choice);
            if (choice == 1) {
                YOKO = 7;
                TATE = 6;
                break;
            }
            if (choice == 2) {
                YOKO = 10;
                TATE = 8;
                break;
            }
            if (choice == 3) {
                YOKO = 6;
                TATE = 4;
                break;
            }
            printf("無効な入力です。はじめからやり直してください。\n");
            return 0;
        }*/

        while (1) {
            printf("盤面の横サイズ (1～%d) を入力してください: ", MAX_YOKO);
            scanf("%d", &YOKO);

            printf("盤面の縦サイズ (1～%d) を入力してください: ", MAX_TATE);
            scanf("%d", &TATE);

            if (YOKO < 1 || YOKO > MAX_YOKO || TATE < 1 || TATE > MAX_TATE) {
                printf("範囲外です。はじめからやり直してください。\n");
                return 0;
            }
            break;
        }

        while (1) {
            printf("1以上の揃える数を入力してください:\n>");
            scanf("%d", &ren);
            if (ren <= 0) {
                printf("無効な入力です。はじめからやり直してください。\n");
                return 0;
            }
            break;
        }

        int holeRate;

        // 落とし穴の割合設定
        while (1) {
            printf("落とし穴(#)の割合を入力してください (0～50%%): ");
            scanf("%d", &holeRate);

            if (holeRate < 0 || holeRate > 50) {
                printf("範囲外です。もう一度入力してください。\n");
                return 0;
            }
            break;
        }


        /* consume leftover newline (or other chars) from scanf so the next
           fgets() does not read an empty line immediately */
        {
            int _ch;
            while ((_ch = getchar()) != '\n' && _ch != EOF);
        }

        initBoard(holeRate);
        printf("%d\n",holeRate);
        // printf("%d\n",holeCount);
        turn = 'O';
    }

    int yoko;

    while (1) {
        displayBoard();
        printf("Player %c の手番です。列を入力: ", turn);

        char input[64];
        if (!fgets(input, sizeof(input), stdin)) {
            /* 入力エラーやEOF */
            printf("入力エラーです。もう一度入力してください。\n");
            clearerr(stdin);
            continue;
        }

        /* remove trailing newline */
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') input[len - 1] = '\0';

        if (input[0] == '\0') {
            printf("INVALUE:数字を入力してください。\n");
            continue;
        }

        char* endptr = NULL;
        long val = strtol(input, &endptr, 10);
        if (endptr == input || *endptr != '\0') {
            printf("NULLPOINTER:数字を入力してください。\n");
            continue;
        }

        yoko = (int)val;
        if (val < 0 || yoko > YOKO) {
            printf("範囲外です。\n");
            continue;
        }

        if (!dropPiece(yoko, turn)) {
            printf("その列には置けません。\n");
            continue;
        }
        // ★ 毎ターン自動セーブ
        // 保存時は「そのターンで置いたプレイヤー」を保存しているので、
        // ロード時は次の手番にする（反転する）



        char savedTurn = turn;
        if (savedTurn == 'O')
            savedTurn = 'X';
        else if (savedTurn == 'X')
            savedTurn = 'O';
        saveGame(savedTurn, path, tm);

        char winner = checkWin(ren);

        if (winner) {
            displayBoard();
            printf("Player %c の勝利です！\n", winner);
            break;
        }

        if (isFull()) {
            displayBoard();
            printf("引き分けです。\n");
            break;
        }

        turn = (turn == 'O') ? 'X' : 'O';
    }

    return 0;
}
