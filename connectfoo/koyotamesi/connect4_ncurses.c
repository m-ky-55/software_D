#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <ncurses.h>
#include <locale.h>
#include <errno.h>




#define MAX_TATE 20
#define MAX_YOKO 20

int TATE, YOKO;
int REN;
char board[MAX_TATE][MAX_YOKO];

//================================================
// セーブ関連
//================================================
void createSaveFolder() {
#ifdef _WIN32
    mkdir("saves");
#else
    mkdir("saves", 0777);
#endif
}

void createSaveGameFolder(const char* path) {
#ifdef _WIN32
    mkdir(path);
#else
    mkdir(path, 0777);
#endif
}

void makeSaveFilename(char* buf, const char* path, struct tm* tm) {
    snprintf(buf, 256, "%s/%04d%02d%02d%02d%02d%02d.txt",
             path,
             tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
             tm->tm_hour, tm->tm_min, tm->tm_sec);
}

void saveGame(char nextTurn, const char* path, struct tm* tm) {
    char filename[256];

    time_t now = time(NULL);
    struct tm* now_tm = localtime(&now);
    if (!now_tm) now_tm = tm;

    makeSaveFilename(filename, path, now_tm);

    FILE* fp = fopen(filename, "w");
    if (!fp) {
        perror("セーブ失敗 fopen");
        return;
    }

    fprintf(fp, "TATE %d,", TATE);
    fprintf(fp, "YOKO %d,", YOKO);
    fprintf(fp, "TURN %c,", nextTurn);
    fprintf(fp, "REN %d,", REN);
    fprintf(fp, "BOARD ,");

    for (int r = 0; r < TATE; r++)
        for (int c = 0; c < YOKO; c++)
            fprintf(fp, "%c ", board[r][c]);

    fclose(fp);
}



//================================================
// 盤面初期化
//================================================
void initHoles(int holeCount) {
    srand((unsigned)time(NULL));
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

void initBoard(int holeRate) {
    for (int r = 0; r < TATE; r++)
        for (int c = 0; c < YOKO; c++)
            board[r][c] = '.';

    int holeCount = (TATE * YOKO * holeRate) / 100;
    initHoles(holeCount);
}

//================================================
// ゲームロジック
//================================================
int dropPiece(int yoko, char player) {
    int c = yoko - 1;
    for (int r = TATE - 1; r >= 0; r--) {
        if (board[r][c] == '#') continue;
        if (board[r][c] == '.') {
            board[r][c] = player;
            return 1;
        }
    }
    return 0;
}

char checkWin(int N) {
    const int dr[4] = {0, 1, 1, 1};
    const int dc[4] = {1, 0, 1, -1};

    for (int r = 0; r < TATE; r++) {
        for (int c = 0; c < YOKO; c++) {
            char p = board[r][c];
            if (p == '.' || p == '#') continue;

            for (int d = 0; d < 4; d++) {
                int cnt = 1;
                for (int k = 1; k < N; k++) {
                    int nr = r + dr[d] * k;
                    int nc = c + dc[d] * k;
                    if (nr < 0 || nr >= TATE || nc < 0 || nc >= YOKO) break;
                    if (board[nr][nc] == p) cnt++;
                    else break;
                }
                if (cnt == N) return p;
            }
        }
    }
    return 0;
}

int isFull() {
    for (int r = 0; r < TATE; r++)
        for (int c = 0; c < YOKO; c++)
            if (board[r][c] == '.') return 0;
    return 1;
}

//================================================
// ncurses UI
//================================================
int inputNumber(const char* msg, int min, int max) {
    int val;
    while (1) {
        clear();
        mvprintw(2, 2, "%s (%d〜%d)", msg, min, max);
        mvprintw(4, 2, "入力: ");
        echo();
        scanw("%d", &val);
        noecho();
        if (val >= min && val <= max) return val;
    }
}

void drawBoard(int cursor) {
    clear();

    mvprintw(1, 4 + cursor * 4, "▼");

    for (int c = 0; c < YOKO; c++)
        mvprintw(2, 4 + c * 4, "%2d", c + 1);

    for (int r = 0; r < TATE; r++) {
        for (int c = 0; c < YOKO; c++) {
            int y = 4 + r;
            int x = 4 + c * 4;
            char v = board[r][c];

            if (v == 'O') {
                attron(COLOR_PAIR(1));
                mvprintw(y, x, " O ");
                attroff(COLOR_PAIR(1));
            } else if (v == 'X') {
                attron(COLOR_PAIR(2));
                mvprintw(y, x, " X ");
                attroff(COLOR_PAIR(2));
            } else if (v == '#') {
                mvprintw(y, x, " # ");
            } else {
                mvprintw(y, x, " . ");
            }
        }
    }
}


//================================================
// ロード機能
//================================================
int loadGame(const char *fname, char *turn) {
    char fullpath[512];

    if (strncmp(fname, "saves/", 6) == 0)
        snprintf(fullpath, sizeof(fullpath), "%s", fname);
    else
        snprintf(fullpath, sizeof(fullpath), "saves/%s", fname);

    FILE *fp = fopen(fullpath, "r");
    if (!fp) {
        perror("ロード失敗 fopen");
        return 0;
    }

    char line[4096];
    if (!fgets(line, sizeof(line), fp)) {
        fclose(fp);
        return 0;
    }
    fclose(fp);

    char *token;

    token = strtok(line, ",");
    if (!token || sscanf(token, "TATE %d", &TATE) != 1) return 0;

    token = strtok(NULL, ",");
    if (!token || sscanf(token, "YOKO %d", &YOKO) != 1) return 0;

    token = strtok(NULL, ",");
    if (!token || sscanf(token, "TURN %c", turn) != 1) return 0;

    token = strtok(NULL, ",");
    if (!token || sscanf(token, "REN %d", &REN) != 1) return 0;

    token = strtok(NULL, ",");
    if (!token || strcmp(token, "BOARD ") != 0) return 0;

    token = strtok(NULL, ",");   // ← ここが重要（盤面）
    if (!token) return 0;

    int r = 0, c = 0;
    for (char *p = token; *p && r < TATE; p++) {
        if (*p == '.' || *p == '#' || *p == 'O' || *p == 'X') {
            board[r][c++] = *p;
            if (c == YOKO) {
                c = 0;
                r++;
            }
        }
    }

    return (r == TATE && c == 0);
}





//================================================
// main
//================================================
int main() {
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);

    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);

    createSaveFolder();

    clear();
    mvprintw(2, 2, "1: 新規ゲーム");
    mvprintw(3, 2, "2: ロードして再開");
    mvprintw(5, 2, "選択: ");
    refresh();

    int menu;
    echo();
    scanw("%d", &menu);
    noecho();

    char turn = 'O';

    if (menu == 2) {
        char fname[256];
        clear();
        mvprintw(2, 2, "saves/以下のファイルパスを入力:");
        echo();
        scanw("%255s", fname);
        noecho();



        if (!loadGame(fname, &turn)) {
            mvprintw(4, 2, "ロード失敗 → 新規ゲーム開始");
            getch();
            menu = 1;
        } else {
            // TATE / YOKO / REN / board / turn が復元済み
        }
        mvprintw(1, 2, "TATE=%d YOKO=%d REN=%d TURN=%c", TATE, YOKO, REN, turn);
        
    }
    

    int holeRate = 0;

    if (menu == 1) {
        YOKO = inputNumber("盤面の横サイズ", 1, MAX_YOKO);
        TATE = inputNumber("盤面の縦サイズ", 1, MAX_TATE);
        REN  = inputNumber("連続コマ数", 1, (TATE > YOKO ? TATE : YOKO));
        holeRate = inputNumber("落とし穴の割合(%)", 0, 50);
        initBoard(holeRate);
        turn = 'O';
    }

    time_t t = time(NULL);
    struct tm* tm = localtime(&t);

    char path[256];
    snprintf(path, sizeof(path), "saves/%04d%02d%02d%02d%02d%02d",
             tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
             tm->tm_hour, tm->tm_min, tm->tm_sec);
    createSaveGameFolder(path);

    int cursor = 0;

    while (1) {
        drawBoard(cursor);
        mvprintw(6 + TATE, 2,
                 "Player %c  ← → 移動 / Enter 決定", turn);
        refresh();

        int ch = getch();
        if (ch == KEY_LEFT && cursor > 0) cursor--;
        else if (ch == KEY_RIGHT && cursor < YOKO - 1) cursor++;
        else if (ch == '\n') {
            if (!dropPiece(cursor + 1, turn)) continue;

            char nextTurn = (turn == 'O') ? 'X' : 'O';
            saveGame(nextTurn, path, tm);

            char w = checkWin(REN);
            if (w || isFull()) {
                drawBoard(cursor);
                if (w) {
                    mvprintw(8 + TATE, 2, "Player %c の勝利！", w);
                } else {
                    mvprintw(8 + TATE, 2, "引き分けです");
                }

                mvprintw(10 + TATE, 2, "Enterキーを押すと終了します");
                refresh();

                // Enterが押されるまで待つ
                int ch;
                while ((ch = getch()) != '\n') {
                    /* 何もしない */
                }
                mvprintw(10 + TATE, 2, "Player %c の勝利！!!", w);
                break;
            }
            turn = nextTurn;
        }
    }

    endwin();
    return 0;
}

// 実行：gcc connect4_ncurses.c -lncurses -o connect4./connect4
