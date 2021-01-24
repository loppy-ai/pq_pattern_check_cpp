#include <iostream>
#include "param_info.h"
#include "trace_pattern_board.h"

int main(int argc, char *argv[])
{
    // パラメータ設定ファイル読み込み
    // TODO:読み込み処理を自作する
    Param_Info pi("param_config.cfg");
    pi.print();

    // なぞり消しパターンファイル読み込み
    Trace_Pattern_Board tpb("trace_pattern_config.cfg");
    tpb.print("なぞり消しパターン :");


}