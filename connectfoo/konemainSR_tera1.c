#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#define MAX_ROWS 10
#define MAX_COLS 10

int ROWS, COLS;
char board[MAX_ROWS][MAX_COLS];  // '.' 空, 'O' Player1, 'X' Player2

//----------------------------------------------
// savesフォルダ作成
//----------------------------------------------
void createSaveFolder() {
#ifdef _WIN32
    mkdir("saves");
#else
    mkdir("saves", 0777);
#endif
}

//----------------------------------------------
// saveファイル入れるフォルダ作成
//----------------------------------------------
void createSaveGameFolder(const char* path) {
#ifdef _WIN32
    mkdir(path);
#else
    mkdir(path, 0777);
#endif
}

//----------------------------------------------
// セーブファイル名作成
//----------------------------------------------
void makeSaveFilename(char* buf, const char* path, struct tm* tm) {
    /* path is a directory string like "saves/20251231010101", so produce
       "path/20251231010101.txt" */
    snprintf(buf, 256, "%s/%04d%02d%02d%02d%02d%02d.txt", path,
             tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour,
             tm->tm_min, tm->tm_sec);
}

//----------------------------------------------
// セーブ（シリアライズ）
//----------------------------------------------
void saveGame(char turn, const char* path, struct tm* tm) {
    char filename[256];

    /* Use current time to generate a unique filename for each autosave */
    time_t now = time(NULL);
    struct tm* now_tm = localtime(&now);
    if (!now_tm) {
        /* fallback to provided tm if localtime fails */
        now_tm = tm;
    }

    makeSaveFilename(filename, path, now_tm);

    FILE* fp = fopen(filename, "w");
    if (!fp) {
        printf("セーブ失敗\n");
        return;
    }

    fprintf(fp, "ROWS %d\n", ROWS);
    fprintf(fp, "COLS %d\n", COLS);
    fprintf(fp, "TURN %c\n", turn);
    fprintf(fp, "BOARD\n");

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            fprintf(fp, "%c ", board[r][c]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    printf("自動セーブ: %s\n", filename);
}

//----------------------------------------------
// ロード（デシリアライズ）
//----------------------------------------------
int loadGame(const char* filename, char* turn) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("ファイルを開けません: %s\n", filename);
        return 0;
    }

    char label[16];

    fscanf(fp, "%s %d", label, &ROWS);  // ROWS
    fscanf(fp, "%s %d", label, &COLS);  // COLS
    fscanf(fp, "%s %c", label, turn);   // TURN
    fscanf(fp, "%s", label);            // BOARD

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            fscanf(fp, " %c", &board[r][c]);
        }
    }

    fclose(fp);
    return 1;
}

//----------------------------------------------
// 盤面初期化
//----------------------------------------------
void initBoard() {
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++) board[r][c] = '.';
}

//----------------------------------------------
// 盤面表示
//----------------------------------------------
void displayBoard() {
    printf("\n  ");
    for (int c = 0; c < COLS; c++) printf("%d ", c);
    printf("\n");
    for (int r = 0; r < ROWS; r++) {
        printf("%d ", r);
        for (int c = 0; c < COLS; c++) {
            printf("%c ", board[r][c]);
        }
        printf("\n");
    }
}

//----------------------------------------------
// 指定列にコマを落とす
//----------------------------------------------
int dropPiece(int col, char player) {
    for (int r = ROWS - 1; r >= 0; r--) {
        if (board[r][col] == '.') {
            board[r][col] = player;
            return 1;
        }
    }
    return 0;  // 置けない
}

//----------------------------------------------
// 4連判定
//----------------------------------------------
char checkWin() {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            char p = board[r][c];
            if (p == '.') continue;

            // 横
            if (c + 3 < COLS && p == board[r][c + 1] && p == board[r][c + 2] &&
                p == board[r][c + 3])
                return p;

            // 縦
            if (r + 3 < ROWS && p == board[r + 1][c] && p == board[r + 2][c] &&
                p == board[r + 3][c])
                return p;

            // 右下
            if (r + 3 < ROWS && c + 3 < COLS && p == board[r + 1][c + 1] &&
                p == board[r + 2][c + 2] && p == board[r + 3][c + 3])
                return p;

            // 左下
            if (r + 3 < ROWS && c - 3 >= 0 && p == board[r + 1][c - 1] &&
                p == board[r + 2][c - 2] && p == board[r + 3][c - 3])
                return p;
        }
    }
    return 0;
}

//----------------------------------------------
// 盤面がすべて埋まったか
//----------------------------------------------
int isFull() {
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            if (board[r][c] == '.') return 0;
    return 1;
}

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
        }
    }

    if (menu == 1 || menu != 2) {
        printf("盤面サイズを選択してください:\n");
        printf("1: 7x6\n2: 10x8\n3: 6x4\n> ");

        int choice;
        while (1) {
            scanf("%d", &choice);
            if (choice == 1) {
                COLS = 7;
                ROWS = 6;
                break;
            }
            if (choice == 2) {
                COLS = 10;
                ROWS = 8;
                break;
            }
            if (choice == 3) {
                COLS = 6;
                ROWS = 4;
                break;
            }
            printf("無効な入力です。はじめからやり直してください。\n");
            return 0;
        }

        /* consume leftover newline (or other chars) from scanf so the next
           fgets() does not read an empty line immediately */
        {
            int _ch;
            while ((_ch = getchar()) != '\n' && _ch != EOF);
        }

        initBoard();
        turn = 'O';
    }

    int col;

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
            printf("数字を入力してください。\n");
            continue;
        }

        char* endptr = NULL;
        long val = strtol(input, &endptr, 10);
        if (endptr == input || *endptr != '\0') {
            printf("数字を入力してください。\n");
            continue;
        }

        col = (int)val;
        if (val < 0 || col >= COLS) {
            printf("範囲外です。\n");
            continue;
        }

        if (!dropPiece(col, turn)) {
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

        char winner = checkWin();
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
