#include <iostream>
#include "param_info.h"
#include "trace_pattern_board.h"
#include "next.h"
#include "fixed_next.h"
#include "board.h"
#include "fixed_board.h"

int main()
{
    // パラメータ設定ファイル読み込み
    Param_Info pi("param_config.cfg");
    pi.print();

    // なぞり消しパターンファイル読み込み
    Trace_Pattern_Board tpb("trace_pattern.tpb");
    tpb.print("なぞり消しパターン :");

    // 盤面初期設定
    Fixed_Next fnext(pi.getNextColor());
    Fixed_Board fboard(pi.getBoardPattern());
    fnext.print();
    fboard.print();
}