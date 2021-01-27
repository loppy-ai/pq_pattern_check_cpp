#include "param_info.h"
#include "trace_pattern_board.h"
#include "next.h"
#include "fixed_next.h"
#include "board.h"
#include "fixed_board.h"
#include "chain_info.h"
#include "print.h"

int main()
{
    // パラメータ設定ファイル読み込み
    Param_Info pi("param_config.cfg");
    pi.print();

    // なぞり消しパターンファイル読み込み
    Trace_Pattern_Board tpb("trace_pattern.tpb");
    tpb.print("なぞり消しパターン :");

    // 初期盤面設定
    Fixed_Next fnext(pi.getNextColor());
    Fixed_Board fboard(pi.getBoardPattern());

    // 連鎖情報生成
    Chain_Info ci(&pi, &fnext, &fboard, &tpb);

    // 結果表示
    printResult(&pi, &ci);
    system("PAUSE");
}