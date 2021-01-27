#include <iostream>
#include <fstream>
#include <sstream>
#include <pqxx/pqxx>
#include <time.h>
#include "param_info.h"
#include "board.h"
#include "fixed_next.h"
#include "fixed_board.h"
#include "chain_info.h"
#include "print.h"
#include "size.h"

constexpr int frequency = 1000000;

int getTracePatternSize(const int);

int main()
{
    // パラメータ設定ファイル読み込み
    Param_Info pi("param_config.cfg");
    pi.setProcessPrintFlag(false);      // 最適解探索中は必ずfalseにすること
    pi.print();

    int trace_pattern_count = getTracePatternSize(pi.getMaxTrace());            // なぞり消しパターンの総数
    int max_access = (trace_pattern_count + (frequency - 1)) / frequency;       // DBアクセス回数

    // 今回は全パターンから最大倍率を求めたい
    double now_max_magnification = 0.0; // 最大倍率
    Board now_max_board;                // 最大倍率の盤面

    std::cout << "\n" << "---------------------処理開始---------------------" << std::endl;
    clock_t start_time = clock();

    // データベースへのアクセス
    try {
        pqxx::connection conn("dbname=pqdb");

        // 全パターン検索
        for (int i = 0; i < max_access; ++i) {
            // タイムスタンプ表示
            std::cout << std::right << std::setw(10) << i * frequency << " ～ " << std::right << std::setw(10) << (i + 1) * frequency << " : " << getTimeStamp() << std::endl;

            // なぞり消しパターン取得
            int start = i * frequency;
            pqxx::nontransaction work(conn);
            conn.prepare("sql", "SELECT board FROM pattern WHERE id BETWEEN $1 AND $2");
            pqxx::result result;
            if ((start + frequency) > trace_pattern_count) {
                // 余り部分だけを取得
                result = work.prepared("sql")(start + 1)(trace_pattern_count).exec();
            }
            else {
                // frequency行分を一括取得
                result = work.prepared("sql")(start + 1)(start + frequency).exec();
            }
            for (pqxx::result::const_iterator ite = result.begin(); ite != result.end(); ++ite) {
                // 48文字のデータ
                std::string board_string = ite[0].as(std::string());

                // なぞり消し盤面設定
                Board tpb;
                for (int j = 0; j < board_size; ++j) {
                    tpb.setBoardElement(j, board_string[j] - 48);
                }

                // 初期盤面設定
                Fixed_Next fnext(pi.getNextColor());
                Fixed_Board fboard(pi.getBoardPattern());

                // 連鎖情報生成
                Chain_Info ci(&pi, &fnext, &fboard, &tpb);

                // 最大チェック
                double magnification = ci.getMagnificationPerColor(&pi, pi.getNextColor());
                if (now_max_magnification < magnification) {
                    now_max_magnification = magnification;
                    now_max_board.setBoard(tpb.getBoard());     // 要素を全てコピー
                }
            } // end for
            work.commit();
        } // end for
        conn.disconnect();
    } // end try
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }

    std::cout << std::endl;

    // 結果表示のためにもう一度計算…
    Param_Info pi2("param_config.cfg");
    Fixed_Next fnext(pi.getNextColor());
    Fixed_Board fboard(pi.getBoardPattern());
    Chain_Info ci(&pi2, &fnext, &fboard, &now_max_board);
    printResult(&pi, &ci);
    std::cout << std::endl;

    std::cout << "----------------なぞり消しパターン----------------" << std::endl;
    now_max_board.print();
    std::cout << std::endl;

    // 時間表示
    clock_t end_time = clock();
    const double time = (static_cast<double>(end_time) - static_cast<double>(start_time)) / CLOCKS_PER_SEC;
    std::cout << "処理時間 : " << time << std::endl;
    std::cout << std::endl;

    // ファイル出力
    std::stringstream file_name_stream;
    file_name_stream << pi2.getNextColor() << "_" << pi2.getBoardPattern() << "_" << pi2.getMaxTrace() << "_" << pi2.getEliminationCoefficient() << "_" << pi2.getChainCoefficient() << ".txt";

    std::ofstream outputfile(file_name_stream.str());
    outputfile << "ネクストの色       : " << pi2.getNextColor() << "\n";
    outputfile << "盤面パターン       : " << pi2.getBoardPattern() << "\n";
    outputfile << "最大なぞり消し数   : " << pi2.getMaxTrace() << "\n";
    outputfile << "同時消し係数       : " << pi2.getEliminationCoefficient() << "\n";
    outputfile << "連鎖係数           : " << pi2.getChainCoefficient() << "\n";
    outputfile << "消える時の結合数   : " << pi2.getMaxConnection() << "\n";
    outputfile << "\n";
    outputfile << "-----------------------------------" << "\n";
    outputfile << "|   色   |消去数|  倍率  |ペア倍率|" << "\n";
    outputfile << "-----------------------------------" << "\n";
    outputfile << "|   赤   |" << std::right << std::setw(6) << ci.getElementCountPerColor(Red) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci.getMagnificationPerColor(&pi2, Red) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci.getMagnificationPerColor(&pi2, Red) * 5.5 << "|" << "\n";
    outputfile << "|   青   |" << std::right << std::setw(6) << ci.getElementCountPerColor(Blue) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci.getMagnificationPerColor(&pi2, Blue) << "| ------ |" << "\n";
    outputfile << "|   緑   |" << std::right << std::setw(6) << ci.getElementCountPerColor(Green) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci.getMagnificationPerColor(&pi2, Green) << "| ------ |" << "\n";
    outputfile << "|   黄   |" << std::right << std::setw(6) << ci.getElementCountPerColor(Yellow) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci.getMagnificationPerColor(&pi2, Yellow) << "| ------ |" << "\n";
    outputfile << "|   紫   |" << std::right << std::setw(6) << ci.getElementCountPerColor(Purple) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci.getMagnificationPerColor(&pi2, Purple) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci.getMagnificationPerColor(&pi2, Purple) * 5.5 << "|" << "\n";
    outputfile << "|ワイルド| ---- |" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci.getMagnificationPerColor(&pi2, None) << "| ------ |" << "\n";
    outputfile << "-----------------------------------" << "\n";
    outputfile << "\n";
    for (int i = 0; i < row_size; ++i) {
        outputfile
            << now_max_board.getBoardElement(i * column_size) << " "
            << now_max_board.getBoardElement(i * column_size + 1) << " "
            << now_max_board.getBoardElement(i * column_size + 2) << " "
            << now_max_board.getBoardElement(i * column_size + 3) << " "
            << now_max_board.getBoardElement(i * column_size + 4) << " "
            << now_max_board.getBoardElement(i * column_size + 5) << " "
            << now_max_board.getBoardElement(i * column_size + 6) << " "
            << now_max_board.getBoardElement(i * column_size + 7) << " " << "\n";
    }
    outputfile.close();
}

int getTracePatternSize(const int max_trace)
{
    int size = 0;
    switch (max_trace)
    {
    case 1:
        size = 48;
        break;
    case 2:
        size = 200;
        break;
    case 3:
        size = 804;
        break;
    case 4:
        size = 3435;
        break;
    case 5:
        size = 15359;
        break;
    case 6:
        size = 70147;
        break;
    case 7:
        size = 320111;
        break;
    case 8:
        size = 1438335;
        break;
    case 9:
        size = 6300691;
        break;
    case 10:
        size = 26702013;
        break;
    case 11:
        size = 108735877;
        break;
    case 12:
        size = 422742049;
        break;
    default:
        break;
    }
    return size;
}