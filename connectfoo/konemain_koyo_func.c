#include "konemain_koyo.h"

int TATE, YOKO;
int ren;
int holeRate;
char board[MAX_TATE][MAX_YOKO];  // '.' 空, 'O' Player1, 'X' Player2


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

    fprintf(fp, "TATE %d,", TATE);
    fprintf(fp, "YOKO %d,", YOKO);
    fprintf(fp, "TURN %c,", turn);
    fprintf(fp, "REN %d,", ren);
    fprintf(fp, "BOARD ,");

    for (int r = 0; r < TATE; r++) {
        for (int c = 0; c < YOKO; c++) {
            fprintf(fp, "%c ", board[r][c]);
        }
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

    fscanf(fp, "%s %d,", label, &TATE);  // TATE
    fscanf(fp, "%s %d,", label, &YOKO);  // YOKO
    fscanf(fp, "%s %c,", label, turn);   // TURN
    fscanf(fp, "%s %d,", label, &ren);   // REN
    fscanf(fp, "%s ,", label);           // BOARD

    for (int r = 0; r < TATE; r++) {
        for (int c = 0; c < YOKO; c++) {
            fscanf(fp, "%c ", &board[r][c]);
        }
    }

    fclose(fp);
    return 1;
}

//----------------------------------------------
// 盤面初期化
//----------------------------------------------
// int initBoard(holeRate) {
//     for (int r = 0; r < TATE; r++)
//         for (int c = 0; c < YOKO; c++) board[r][c] = '.';
//     // ------------------------------
//     // initHoles((TATE * YOKO) * 50/100);  // 例：盤面の10%
//     int holeCount = ((TATE * YOKO * holeRate) / 100);
//     // int holeCount = (TATE * YOKO * rate) / 100;
//     initHoles(holeCount);

// }

void initBoard(int holeRate) {
    for (int r = 0; r < TATE; r++)
        for (int c = 0; c < YOKO; c++)
            board[r][c] = '.';

    int holeCount = (TATE * YOKO * holeRate) / 100;
    initHoles(holeCount);
}




//----------------------------------------------
// 盤面表示
//----------------------------------------------
void displayBoard() {
    printf("\n  ");
    for (int c = 0; c < YOKO; c++) {
        if (c + 1 <= 10)
            printf(" %d", c + 1);
        else
            printf("%d", c + 1);
    }
    printf("\n");
    for (int r = 0; r < TATE; r++) {
        if (r + 1 < 10)
            printf(" %d ", r + 1);
        else
            printf("%d ", r + 1);
        for (int c = 0; c < YOKO; c++) {
            printf("%c ", board[r][c]);
        }
        printf("\n");
    }
}

//----------------------------------------------
// 指定列にコマを落とす
//----------------------------------------------
// int dropPiece(int yoko, char player) {
//     for (int r = TATE - 1; r >= 0; r--) {
//         if (board[r][yoko - 1] == '.') {
//             board[r][yoko - 1] = player;
//             return 1;
//         }
//     }
//     return 0;  // 置けない
// }

//  ----------------------------------------------
int dropPiece(int yoko, char player) {
    int c = yoko - 1;

    for (int r = TATE - 1; r >= 0; r--) {
        if (board[r][c] == '#') continue;   // 穴は通過
        if (board[r][c] == '.') {
            board[r][c] = player;
            return 1;
        }
        // 駒があったらさらに上を探す
    }
    return 0;  // 空きがない
}



//----------------------------------------------
// 落とし穴の生成
//----------------------------------------------
void initHoles(int holeCount) {
    srand((unsigned int)time(NULL));

    int placed = 0;
    while (placed < holeCount) {
        int r = rand() % TATE;
        int c = rand() % YOKO;

        if (board[r][c] == '.') {
            board[r][c] = '#';
            placed++;
        }
    }
}
// void initHoles(int holeCount) {
//     srand((unsigned int)time(NULL));

//     int holeCount = TATE * YOKO * holeRate / 100;

//     int placed = 0;
//     while (placed < holeCount) {
//         int r = rand() % TATE;
//         int c = rand() % YOKO;

//         if (board[r][c] == '.') {
//             board[r][c] = '#';
//             placed++;
//         }
//     }
// }



//----------------------------------------------
// N連判定
//----------------------------------------------
char checkWin(int N) {
    if (N < 1) return 0;

    // 方向ベクトル：横、縦、右下、左下
    const int dr[4] = {0, 1, 1, 1};
    const int dc[4] = {1, 0, 1, -1};

    for (int r = 0; r < TATE; r++) {
        for (int c = 0; c < YOKO; c++) {
            char p = board[r][c];
            // if (p == '.') continue;
            if (p == '.' || p == '#') continue;

            // 4方向をチェック
            for (int dir = 0; dir < 4; dir++) {
                int count = 1;

                for (int k = 1; k < N; k++) {
                    int nr = r + dr[dir] * k;
                    int nc = c + dc[dir] * k;

                    // 範囲外チェック
                    if (nr < 0 || nr >= TATE || nc < 0 || nc >= YOKO) break;

                    if (board[nr][nc] == p)
                        count++;
                    else
                        break;
                }

                if (count == N) return p;
            }
        }
    }
    return 0;
}

//----------------------------------------------
// 盤面がすべて埋まったか
//----------------------------------------------
int isFull() {
    for (int r = 0; r < TATE; r++)
        for (int c = 0; c < YOKO; c++)
            if (board[r][c] == '.') return 0;
    return 1;
}
