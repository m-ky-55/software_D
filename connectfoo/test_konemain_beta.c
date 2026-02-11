#include <assert.h>
#include <stdio.h>

#include "konemain_beta.h"

/* -----------------------------
   テスト用セットアップ
----------------------------- */
void setupBoard(int tate, int yoko) {
    TATE = tate;
    YOKO = yoko;
    initBoard();
}

/* -----------------------------
   dropPiece テスト
----------------------------- */
void test_dropPiece() {
    printf("TESTSTART:dropPiece()\n");
    setupBoard(3, 3);

    /* ① 絶対に通る値 */
    assert(dropPiece(1, 'O') == 1);

    /* ② 境界値（通る側）最大列 */
    assert(dropPiece(3, 'X') == 1);

    /* 列を満タンにする */
    setupBoard(2, 1);
    dropPiece(1, 'O');
    dropPiece(1, 'X');

    /* ③ 境界値（通らない側）満タン */
    assert(dropPiece(1, 'O') == 0);

    /* ④ 絶対に通らない値（範囲外想定） */
    setupBoard(3, 3);
    assert(dropPiece(0, 'O') == 0);

    printf("TESTEND:dropPiece()\n");
}

/* -----------------------------
   checkWin 4方向テスト
----------------------------- */
void test_checkWin() {
    printf("TESTSTART:checkWin()\n");
    /* ===== 横方向 ===== */
    printf("HORIZONTAL\n");
    setupBoard(4, 4);
    board[3][0] = 'O';
    board[3][1] = 'O';
    board[3][2] = 'O';

    /* ① 絶対に通る */
    assert(checkWin(3) == 'O');

    /* ② 境界値（通る側） */
    assert(checkWin(1) == 'O');

    /* ③ 境界値（通らない側） */
    assert(checkWin(4) == 0);

    /* ④ 絶対に通らない値 */
    assert(checkWin(0) == 0);

    /* ===== 縦方向 ===== */
    printf("VERTICAL\n");
    setupBoard(4, 4);
    board[0][1] = 'X';
    board[1][1] = 'X';
    board[2][1] = 'X';

    assert(checkWin(3) == 'X');

    /* ===== 左上→右下（＼） ===== */
    printf("DIAGONAL\\ \n");
    setupBoard(4, 4);
    board[0][0] = 'O';
    board[1][1] = 'O';
    board[2][2] = 'O';

    assert(checkWin(3) == 'O');

    /* ===== 右上→左下（／） ===== */
    printf("DIAGONAL/ \n");
    setupBoard(4, 4);
    board[0][3] = 'X';
    board[1][2] = 'X';
    board[2][1] = 'X';

    assert(checkWin(3) == 'X');

    printf("TESTEND:checkWin()\n");
}

/* -----------------------------
   isFull テスト
----------------------------- */
void test_isFull() {
    printf("TESTSTART:isFull()\n");

    setupBoard(2, 2);

    /* ① 絶対に通らない値（空） */
    assert(isFull() == 0);

    board[0][0] = 'O';
    board[0][1] = 'X';
    board[1][0] = 'O';

    /* ② 境界値（通らない側） */
    assert(isFull() == 0);

    board[1][1] = 'X';

    /* ③ 境界値（通る側） */
    assert(isFull() == 1);

    printf("TESTEND:isFull()\n");
}

/* -----------------------------
   main
----------------------------- */
int main() {
    test_dropPiece();
    test_checkWin();
    test_isFull();

    printf("\n=== ALL TESTS PASSED ===\n");

    return 0;
}
