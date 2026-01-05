/*testCommon.hをインクルードする。これは、外部のファイルではないので、このファイルと同じ階層に入れて運用する。
その際に、使うのが多分"assertEqualsString"関数あたりになると思う。
なんだけど、それが判別できるのは1行だけのやつだと思う（違う可能性はある）から、今のoutputされてるログファイルを
1行に変更する必要があるかもしれない。そうじゃなかったららっきー。
↑いつだったかやった。出力されるログの中身を１行にした。Notion反映済み。
とりあえずまずは、testCommon.hをインクルードして、プログラムを過去のデータを元に仮で書いてみる。
*/

#include "konemain_beta.h"
#include "testCommon.h"

void test_initBoard() {
    testStart("initBoard()");
    TATE = MAX_TATE;
    YOKO = MAX_YOKO;
    initBoard();
    for (int yoko = 0; yoko < MAX_YOKO; yoko++) {
        for (int tate = 0; tate < MAX_TATE; tate++) {
            char initboard[2] = {board[tate][yoko], '\0'};  // char→C文字列
            assertEqualsString(initboard, ".");
        }
    }
}

void test_dropPiece() {
    for (int tate = 0; tate < MAX_TATE; tate++) {
        for (int yoko = 0; yoko < MAX_YOKO; yoko++) {
            initBoard();
            for (int i = 0; i <= tate; i++) {
                dropPiece(yoko + 1, 'O');
            }
            char dropPiece[2] = {board[tate][MAX_YOKO - 1 - yoko],
                                 '\0'};  // char→C文字列
            assertEqualsString(dropPiece, "O");
        }
    }
}
void test_checkWin_YOKO() {
    for (ren = 1; ren < MAX_YOKO; ren++) {  // 0=error
        for (int yoko = 0; yoko < MAX_YOKO - ren; yoko++) {
            for (int tate = 0; tate < MAX_TATE; tate++) {
                initBoard();
                for (int i = 0; i < ren; i++) {
                    board[tate][yoko + i] = 'O';
                }
                char checkwin_YOKO[2] = {checkWin(ren), '\0'};  // char→C文字列
                assertEqualsString(checkwin_YOKO, "O");
            }
        }
    }
}
void test_checkWin_TATE() {
    for (ren = 1; ren < MAX_TATE; ren++) {  // 0=error
        for (int tate = 0; tate < MAX_TATE - ren; tate++) {
            for (int yoko = 0; yoko < MAX_YOKO; yoko++) {
                initBoard();
                for (int i = 0; i < ren; i++) {
                    board[tate + i][yoko] = 'O';
                }
                char checkwin_TATE[2] = {checkWin(ren), '\0'};  // char→C文字列
                assertEqualsString(checkwin_TATE, "O");
            }
        }
    }
}
void test_checkWin_migiue_to_hidarisita() {
    for (ren = 1; ren < MAX_TATE; ren++) {  // 0=error
        for (int tate = 0; tate + ren <= MAX_TATE; tate++) {
            for (int yoko = ren - 1; yoko < MAX_YOKO - ren; yoko++) {
                initBoard();
                for (int i = 0; i < ren; i++) {
                    board[tate + i][yoko - i] = 'O';
                }
                char checkwin_migiue_to_hidarisita[2] = {checkWin(ren),
                                                         '\0'};  // char→C文字列
                assertEqualsString(checkwin_migiue_to_hidarisita, "O");
            }
        }
    }
}
void test_checkWin_hidariue_to_migisita() {
    for (ren = 1; ren < MAX_TATE; ren++) {  // 0=error
        for (int tate = 0; tate + ren <= MAX_TATE; tate++) {
            for (int yoko = 0; yoko + ren <= MAX_YOKO; yoko++) {
                initBoard();
                for (int i = 0; i < ren; i++) {
                    board[tate + i][yoko + i] = 'O';
                }
                char checkwin_hidariue_to_migisita[2] = {checkWin(ren),
                                                         '\0'};  // char→C文字列
                assertEqualsString(checkwin_hidariue_to_migisita, "O");
            }
        }
    }
}
void test_checkWin_NO() {
    for (ren = 1; ren < MAX_TATE; ren++) {  // 0=error
        initBoard();
        for (int i = 0; i < ren; i++) {
            dropPiece(i + 1, 'O');
        }
        char checkwin_NO[2] = {checkWin(ren), '\0'};  // char→C文字列
        assertEqualsString(checkwin_NO, "0");
    }
}
void test_isFull() {
    for (int yoko = 0; yoko < MAX_YOKO; yoko++) {
        for (int tate = 0; tate < MAX_TATE; tate++) {
            initBoard();
            for (int r = 0; r < MAX_TATE; r++) {
                for (int c = 0; c < MAX_YOKO; c++) {
                    board[r][c] = 'O';
                }
            }
            assertEqualsInt(isFull(), 1);
        }
    }
}
int main() {
    testStart("konemain_beta.c");
    // test_initBoard();
    test_dropPiece();  // error
    // test_checkWin_YOKO(); //error
    // test_checkWin_TATE();　//error
    // test_checkWin_migiue_to_hidarisita(); //error
    // test_checkWin_hidariue_to_migisita(); //error
    // test_checkWin_NO();//error
    // test_isFull();

    //  ここから下は書き換えないこと
    testErrorCheck();
    return 0;
}