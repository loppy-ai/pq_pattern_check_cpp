#include <iostream>
#include <ios>
#include <iomanip>
#include <cstdlib>
#include "param_info.h"
#include "trace_pattern_board.h"
#include "next.h"
#include "fixed_next.h"
#include "board.h"
#include "fixed_board.h"
#include "chain_info.h"
#include "damage_info.h"

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

    // 連鎖情報インスタンス生成
    Chain_Info ci(&pi, &fnext, &fboard, &tpb);

    // 結果表示
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "|   色   |消去数|  倍率  |ペア倍率|" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "|   赤   |" << std::right << std::setw(6) << ci.getElementCountPerColor(Red) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci.getMagnificationPerColor(&pi, Red) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci.getMagnificationPerColor(&pi, Red) * 5.5 << "|" << std::endl;
    std::cout << "|   青   |" << std::right << std::setw(6) << ci.getElementCountPerColor(Blue) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci.getMagnificationPerColor(&pi, Blue) << "| ------ |" << std::endl;
    std::cout << "|   緑   |" << std::right << std::setw(6) << ci.getElementCountPerColor(Green) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci.getMagnificationPerColor(&pi, Green) << "| ------ |" << std::endl;
    std::cout << "|   黄   |" << std::right << std::setw(6) << ci.getElementCountPerColor(Yellow) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci.getMagnificationPerColor(&pi, Yellow) << "| ------ |" << std::endl;
    std::cout << "|   紫   |" << std::right << std::setw(6) << ci.getElementCountPerColor(Purple) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci.getMagnificationPerColor(&pi, Purple) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci.getMagnificationPerColor(&pi, Purple) * 5.5 << "|" << std::endl;
    std::cout << "|ワイルド| ---- |" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci.getMagnificationPerColor(&pi, None) << "| ------ |" << std::endl;
    std::cout << "-----------------------------------" << std::endl;

    system("PAUSE");
}