#include <iostream>
#include "param_info.h"
#include "trace_pattern_board.h"
#include "next.h"
#include "fixed_next.h"
#include "board.h"
#include "fixed_board.h"
#include "chain_info.h"

int main()
{
    // パラメータ設定ファイル読み込み
    Param_Info pi("param_config.cfg");
    std::cout << "---------------------設定情報---------------------" << std::endl;
    pi.print();

    // なぞり消しパターンファイル読み込み
    Trace_Pattern_Board tpb("trace_pattern.tpb");
    tpb.print("なぞり消しパターン :");
    std::cout << std::endl;

    // 初期盤面設定
    Fixed_Next fnext(pi.getNextColor());
    Fixed_Board fboard(pi.getBoardPattern());
    // fnext.print();
    // fboard.print();

    // 連鎖情報インスタンス生成
    Chain_Info ci(&pi, &fnext, &fboard, &tpb);
}