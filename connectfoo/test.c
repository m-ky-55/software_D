#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#include "konemain_beta.h"

/* テスト用初期設定 */
static void setup_board(int tate, int yoko, int n) {
    TATE = tate;
    YOKO = yoko;
    ren  = n;
    initBoard();
}

/* ==============================
   initBoard のテスト
   ============================== */
void test_initBoard() {
    setup_board(6, 7, 4);

    for (int r = 0; r < TATE; r++) {
        for (int c = 0; c < YOKO; c++) {
            assert(board[r][c] == '.');
        }
    }
    printf("[OK] initBoard\n");
}

/* ==============================
   dropPiece のテスト
   ============================== */
void test_dropPiece() {
    setup_board(4, 4, 3);

    assert(dropPiece(1, 'O') == 1);
    assert(board[3][0] == 'O');

    assert(dropPiece(1, 'X') == 1);
    assert(board[2][0] == 'X');

    /* 列を埋める */
    dropPiece(1, 'O');
    dropPiece(1, 'X');

    /* もう置けない */
    assert(dropPiece(1, 'O') == 0);

    printf("[OK] dropPiece\n");
}

/* ==============================
   横方向 勝利判定
   ============================== */
void test_checkWin_horizontal() {
    setup_board(4, 4, 3);

    board[3][0] = 'O';
    board[3][1] = 'O';
    board[3][2] = 'O';

    assert(checkWin(3) == 'O');
    printf("[OK] checkWin horizontal\n");
}

/* ==============================
   縦方向 勝利判定
   ============================== */
void test_checkWin_vertical() {
    setup_board(4, 4, 3);

    board[3][1] = 'X';
    board[2][1] = 'X';
    board[1][1] = 'X';

    assert(checkWin(3) == 'X');
    printf("[OK] checkWin vertical\n");
}

/* ==============================
   斜め方向 勝利判定
   ============================== */
void test_checkWin_diagonal() {
    setup_board(4, 4, 3);

    board[3][0] = 'O';
    board[2][1] = 'O';
    board[1][2] = 'O';

    assert(checkWin(3) == 'O');
    printf("[OK] checkWin diagonal\n");
}

/* ==============================
   isFull のテスト
   ============================== */
void test_isFull() {
    setup_board(2, 2, 2);

    assert(isFull() == 0);

    for (int r = 0; r < TATE; r++)
        for (int c = 0; c < YOKO; c++)
            board[r][c] = 'O';

    assert(isFull() == 1);
    printf("[OK] isFull\n");
}

/* ==============================
   save / load テスト
   ============================== */
void test_save_load() {
    setup_board(2, 2, 2);
    board[1][0] = 'O';
    board[1][1] = 'X';

    char path[] = "saves/test";
    createSaveFolder();
    createSaveGameFolder(path);

    time_t t = time(NULL);
    struct tm* tm = localtime(&t);

    saveGame('X', path, tm);

    /* 盤面を壊す */
    board[1][0] = '.';
    board[1][1] = '.';

    char fname[256];
    makeSaveFilename(fname, path, tm);

    char turn;
    assert(loadGame(fname, &turn) == 1);
    assert(board[1][0] == 'O');
    assert(board[1][1] == 'X');
    assert(turn == 'X');

    printf("[OK] save / load\n");
}

/* ==============================
   main（テスト実行）
   ============================== */
int main(void) {
    test_initBoard();
    test_dropPiece();
    test_checkWin_horizontal();
    test_checkWin_vertical();
    test_checkWin_diagonal();
    test_isFull();
    test_save_load();

    printf("\n=== ALL TESTS PASSED ===\n");
    return 0;
}
