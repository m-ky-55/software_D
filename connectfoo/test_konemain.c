/*testCommon.hをインクルードする。これは、外部のファイルではないので、このファイルと同じ階層に入れて運用する。
その際に、使うのが多分"assertEqualsString"関数あたりになると思う。
なんだけど、それが判別できるのは1行だけのやつだと思う（違う可能性はある）から、今のoutputされてるログファイルを
1行に変更する必要があるかもしれない。そうじゃなかったららっきー。
↑いつだったかやった。出力されるログの中身を１行にした。Notion反映済み。
とりあえずまずは、testCommon.hをインクルードして、プログラムを過去のデータを元に仮で書いてみる。
*/

#include "konemain_alpha.h"
#include "testCommon.h"

void test_initBoard() {
    testStart("initBoard()");
    for (int yoko = 0; yoko < MAX_YOKO; yoko++) {
        for (int tate = 0; tate < MAX_TATE; tate++) {
            // initBoard();
            // assertEqualsInt(board[tate][yoko], '.');
            assertEqualsInt(tate * yoko, tate * yoko);  // 仮のコード
        }
    }
}

// void test_dropPiece() {
//     for (int tate = 0; tate < MAX_TATE; tate++) {
//         /* code */
//     }
// }
//
// void test_checkWin_YOKO() {
//     for (ren = 1; ren < MAX_YOKO; ren++) {  // 0=error
//         for (int yoko = 0; yoko < MAX_YOKO - ren; yoko++) {
//             for (int tate = 0; tate < MAX_TATE; tate++) {
//                 initBoard();
//
//                 for (int i = 0; i < ren; i++) {
//                     board[tate][yoko + i] = 'O';
//                 }
//
//                 assertEqualsInt('O', checkWin(ren));
//             }
//         }
//     }
// }
//
// void test_checkWin_TATE() {
//     for (ren = 1; ren < MAX_TATE; ren++) {  // 0=error
//         for (int tate = 0; tate < MAX_TATE - ren; tate++) {
//             for (int yoko = 0; yoko < MAX_YOKO; yoko++) {
//                 initBoard();
//
//                 for (int i = 0; i < ren; i++) {
//                     board[tate + i][yoko] = 'O';
//                 }
//
//                 assertEqualsInt('O', checkWin(ren));
//             }
//         }
//     }
// }
//
// void test_checkWin_migiue_to_hidarisita() {
//     for (ren = 1; ren < MAX_TATE; ren++) {  // 0=error
//         for (int tate = 0; tate + ren <= MAX_TATE; tate++) {
//             for (int yoko = ren - 1; yoko < MAX_YOKO - ren; yoko++) {
//                 initBoard();
//
//                 for (int i = 0; i < ren; i++) {
//                     board[tate + i][yoko - i] = 'O';
//                 }
//
//                 assertEqualsInt('O', checkWin(ren));
//             }
//         }
//     }
// }
//
// void test_checkWin_hidariue_to_migisita() {
//     for (ren = 1; ren < MAX_TATE; ren++) {  // 0=error
//         for (int tate = 0; tate + ren <= MAX_TATE; tate++) {
//             for (int yoko = 0; yoko + ren <= MAX_YOKO; yoko++) {
//                 initBoard();
//
//                 for (int i = 0; i < ren; i++) {
//                     board[tate + i][yoko + i] = 'O';
//                 }
//
//                 assertEqualsInt('O', checkWin(ren));
//             }
//         }
//     }
// }
//
// void test_checkWin_NO() {
//     for (ren = 1; ren < MAX_TATE; ren++) {  // 0=error
//         initBoard();
//         assertEqualsInt(0, checkWin(ren));
//     }
// }
//
// void test_isFull() {
//     for (int yoko = 0; yoko < MAX_YOKO; yoko++) {
//         for (int tate = 0; tate < MAX_TATE; tate++) {
//             /* code */
//         }
//     }
// }
//
int main() {
    testStart("konemain_alpha.c");
    test_initBoard();
    // ここから下は書き換えないこと
    testErrorCheck();
    return 0;
}