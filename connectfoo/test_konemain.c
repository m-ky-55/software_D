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
    for (int y = 0; y < MAX_YOKO; y++) {
        for (int t = 0; t < MAX_TATE; t++) {
            // initBoard();
            // assertEqualsInt(board[tate][yoko], '.');
            assertEqualsInt(t * y, t * y);  // 仮のコード
        }
    }
}

// void test_dropPiece() {
//     for (int t = 0; t < MAX_TATE; t++) {
//         /* code */
//     }
// }
//
// void test_checkWin_YOKO() {
//     for (ren = 1; ren < MAX_YOKO; ren++) {  // 0=error
//         for (int y = 0; y < MAX_YOKO - ren; y++) {
//             for (int t = 0; t < MAX_TATE; t++) {
//                 initBoard();
//
//                 for (int i = 0; i < ren; i++) {
//                     board[t][y + i] = 'O';
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
//         for (int t = 0; t < MAX_TATE - ren; t++) {
//             for (int y = 0; y < MAX_YOKO; y++) {
//                 initBoard();
//
//                 for (int i = 0; i < ren; i++) {
//                     board[t + i][y] = 'O';
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
//         for (int t = 0; t + ren <= MAX_TATE; t++) {
//             for (int y = ren - 1; y < MAX_YOKO - ren; y++) {
//                 initBoard();
//
//                 for (int i = 0; i < ren; i++) {
//                     board[t + i][y - i] = 'O';
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
//         for (int t = 0; t + ren <= MAX_TATE; t++) {
//             for (int y = 0; y + ren <= MAX_YOKO; y++) {
//                 initBoard();
//
//                 for (int i = 0; i < ren; i++) {
//                     board[t + i][y + i] = 'O';
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
//     for (int y = 0; y < MAX_YOKO; y++) {
//         for (int t = 0; t < MAX_TATE; t++) {
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