/*testCommon.hをインクルードする。これは、外部のファイルではないので、このファイルと同じ階層に入れて運用する。
その際に、使うのが多分"assertEqualsString"関数あたりになると思う。
なんだけど、それが判別できるのは1行だけのやつだと思う（違う可能性はある）から、今のoutputされてるログファイルを
1行に変更する必要があるかもしれない。そうじゃなかったららっきー。

とりあえずまずは、testCommon.hをインクルードして、プログラムを過去のデータを元に仮で書いてみる。
*/
#include "konemain_alpha.h"
#include "testCommon.h"

void something() {}

int main() {}