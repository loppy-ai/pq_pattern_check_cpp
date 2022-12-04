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

constexpr int frequency = 10000000;

int getTracePatternSize(const int);

int main(int argc, char** argv)
{
    // パラメータ設定読み込み
    Param_Info pi(argv);
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
                    tpb.setBoardElement(j, board_string[j] - 48);   // '0' to 0
                }
                // 初期盤面設定
                Fixed_Next fnext(pi.getNextColor());
                Fixed_Board fboard(pi.getBoardPattern());
                // 連鎖情報生成
                Chain_Info ci(&pi, &fnext, &fboard, &tpb);
                // 最大チェック
                double magnification = ci.getMagnificationPerColor(&pi, pi.getMaxColor());
                int prism_count = ci.getElementCountPerColor(Prism);
                int blue_count = ci.getElementCountPerColor(Blue);
                if ((pi.getBoardPattern() > 210) && (pi.getBoardPattern() < 220) && (prism_count == 0)) {
                    continue;   // しろいマール盤面でプリズムボールを消していなかったら何もしない
                }
                if ((pi.getBoardPattern() > 310) && (pi.getBoardPattern() < 320) && (prism_count == 0)) {
                    continue;   // あたり＆プーボ盤面でプリズムボールを消していなかったら何もしない
                }
                if ((pi.getBoardPattern() == 323) && (blue_count == 0)) {
                    continue;   // あたり＆プーボ盤面3で青ぷよを消していなかったら何もしない
                }
                if (now_max_magnification < magnification) {
                    now_max_magnification = magnification;
                    now_max_board.setBoard(tpb.getBoard());     // 要素を全てコピー
                }
            } // end for(result_ite)
            work.commit();
        } // end for(max_access)
        conn.disconnect();
    } // end try
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }

    // 時間表示
    clock_t end_time = clock();
    std::cout << std::endl;

    // 結果表示のためにもう一度計算
    pi.print();
    std::cout << std::endl;
    Fixed_Next fnext(pi.getNextColor());
    Fixed_Board fboard(pi.getBoardPattern());
    Chain_Info ci(&pi, &fnext, &fboard, &now_max_board);
    printResult(&pi, &ci);
    std::cout << std::endl;

    std::cout << "----------------なぞり消しパターン----------------" << std::endl;
    now_max_board.print();
    std::cout << std::endl;

    const double time = (static_cast<double>(end_time) - static_cast<double>(start_time)) / CLOCKS_PER_SEC;
    std::cout << "処理時間 : " << time << std::endl;
    std::cout << std::endl;

    // ファイル出力
    std::stringstream file_name_stream;
    file_name_stream << pi.getNextColor() << "_" << pi.getBoardPattern() << "_" << pi.getMaxTrace() << "_" << pi.getEliminationCoefficient() << "_" << pi.getChainCoefficient() << "_" << pi.getMaxColor() << ".txt";

    std::ofstream outputfile(file_name_stream.str());
    outputfile << "ネクストの色       : " << pi.getNextColor() << "\n";
    outputfile << "盤面パターン       : " << pi.getBoardPattern() << "\n";
    outputfile << "最大なぞり消し数   : " << pi.getMaxTrace() << "\n";
    outputfile << "同時消し係数       : " << pi.getEliminationCoefficient() << "\n";
    outputfile << "連鎖係数           : " << pi.getChainCoefficient() << "\n";
    outputfile << "求めたい色         : " << pi.getMaxColor() << "\n";
    outputfile << "消える時の結合数   : " << pi.getMaxConnection() << "\n";
    outputfile << "\n";
    outputfile << "-----------------------------------" << "\n";
    outputfile << "|   色   |消去数|  倍率  |ペア倍率|" << "\n";
    outputfile << "-----------------------------------" << "\n";
    outputfile << "|   赤   |" << std::right << std::setw(6) << ci.getElementCountPerColor(Red) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci.getMagnificationPerColor(&pi, Red) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci.getMagnificationPerColor(&pi, Red) * 5.5 << "|" << "\n";
    outputfile << "|   青   |" << std::right << std::setw(6) << ci.getElementCountPerColor(Blue) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci.getMagnificationPerColor(&pi, Blue) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci.getMagnificationPerColor(&pi, Blue) * 6.2 << "|" << "\n";
    outputfile << "|   緑   |" << std::right << std::setw(6) << ci.getElementCountPerColor(Green) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci.getMagnificationPerColor(&pi, Green) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci.getMagnificationPerColor(&pi, Green) * 4.5 << "|" << "\n";
    outputfile << "|   黄   |" << std::right << std::setw(6) << ci.getElementCountPerColor(Yellow) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci.getMagnificationPerColor(&pi, Yellow) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci.getMagnificationPerColor(&pi, Yellow) * 5.5 << "|" << "\n";
    outputfile << "|   紫   |" << std::right << std::setw(6) << ci.getElementCountPerColor(Purple) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci.getMagnificationPerColor(&pi, Purple) << "|" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci.getMagnificationPerColor(&pi, Purple) * 5.5 << "|" << "\n";
    outputfile << "|ワイルド| ---- |" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << ci.getMagnificationPerColor(&pi, None) << "| ------ |" << "\n";
    outputfile << "-----------------------------------" << "\n";
    outputfile << "\n";
    for (int i = 0; i < row_size; ++i) {
        outputfile
            << now_max_board.getBoardElement(i * column_size)
            << now_max_board.getBoardElement(i * column_size + 1)
            << now_max_board.getBoardElement(i * column_size + 2)
            << now_max_board.getBoardElement(i * column_size + 3)
            << now_max_board.getBoardElement(i * column_size + 4)
            << now_max_board.getBoardElement(i * column_size + 5)
            << now_max_board.getBoardElement(i * column_size + 6)
            << now_max_board.getBoardElement(i * column_size + 7) << "\n";
    }
    outputfile << "\n";
    outputfile << "処理時間(s) : " << time;
    outputfile << "\n";
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
    case 13:
        size = 1559392541;
        break;
    default:
        break;
    }
    return size;
}