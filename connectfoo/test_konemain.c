/*testCommon.hをインクルードする。これは、外部のファイルではないので、このファイルと同じ階層に入れて運用する。
その際に、使うのが多分"assertEqualsString"関数あたりになると思う。
なんだけど、それが判別できるのは1行だけのやつだと思う（違う可能性はある）から、今のoutputされてるログファイルを
1行に変更する必要があるかもしれない。そうじゃなかったららっきー。
↑いつだったかやった。出力されるログの中身を１行にした。Notion反映済み。
とりあえずまずは、testCommon.hをインクルードして、プログラムを過去のデータを元に仮で書いてみる。
*/
#include "konemain_alpha.h"
#include "testCommon.h"

/*void something() {}*/

void test_initBoard() {
    for (int i = 0; i < MAX_YOKO; i++) {
        for (int j = 0; j < MAX_TATE; j++) {
            initBoard();
            assertEqualsString(board[j][i], '.');
        }
    }
}

void test_dropPiece() {
    for (int i = 0; i < ; i++) {
        /* code */
    }
}

int main() {
    testStart("conekemain_alpha.c");
    test_initBoard();
    // ここから下は書き換えないこと
    testErrorCheck();
    return 0;
}