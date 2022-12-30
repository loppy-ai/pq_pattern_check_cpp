#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <sqlite3.h>
#include "param_info.h"
#include "board.h"
#include "fixed_next.h"
#include "fixed_board.h"
#include "chain_info.h"
#include "print.h"
#include "size.h"

constexpr long long frequency = 16777216;

long long getTracePatternSize(const int);

int main(int argc, char** argv)
{
    // パラメータ設定読み込み
    Param_Info pi(argv);
    long long trace_pattern_count = getTracePatternSize(pi.getMaxTrace());          // なぞり消しパターンの総数
    long long max_access = (trace_pattern_count + (frequency - 1)) / frequency;     // DBアクセス回数

    double now_max_magnification[3] = { 0.0 };  // 最大倍率
    Board now_max_board[3];                     // 最大倍率の盤面

    sqlite3* db = NULL;
    sqlite3_stmt* pStmt;
    const char* path = "D:\\sqlite\\tp.sqlite3";
    int ret;

    // DB接続
    ret = sqlite3_open_v2(
        path,
        &db,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
        nullptr
    );

    // 接続状態確認
    if (ret == SQLITE_OK) {
        printf("Connecttion Success!\n");
    }
    else {
        printf("Connection Failed...\n");
        cerr << sqlite3_errmsg(db) << endl;
        ret = sqlite3_close_v2(db);
        if (ret != SQLITE_OK) {
            printf("Disconnection Failed...\n");
        }
        return -1;
    }

    pi.print(); // ---設定情報---
    cout << "\n" << "---------------------処理開始---------------------" << endl;
    clock_t start_time = clock();

    // トランザクション開始
    sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);
    sqlite3_prepare_v2(
        db,
        "SELECT board FROM pattern WHERE id BETWEEN ? AND ?;",
        -1,
        &pStmt,
        nullptr
    );

    // 全パターン検索
    for (long long i = 0; i < max_access; ++i) {
        // タイムスタンプ表示
        cout << right << setw(10) << i * frequency + 1 << " ～ " << right << setw(10) << (i + 1) * frequency << " : " << getTimeStamp() << endl;

        // なぞり消しパターン取得
        long long start = i * frequency;
        if ((start + frequency) > trace_pattern_count) {
            // 余り部分だけを取得
            sqlite3_bind_int64(pStmt, 1, start + 1);
            sqlite3_bind_int64(pStmt, 2, trace_pattern_count);
        }
        else {
            // frequency行分を一括取得
            sqlite3_bind_int64(pStmt, 1, start + 1);
            sqlite3_bind_int64(pStmt, 2, start + frequency);
        }

        while (sqlite3_step(pStmt) == SQLITE_ROW) {
            // 48文字のデータ
            const unsigned char* board = sqlite3_column_text(pStmt, 0);
            // なぞり消し盤面設定
            Board tpb;
            for (int j = 0; j < board_size; ++j) {
                tpb.setBoardElement(j, board[j] - 48);   // '0' to 0
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
            // 更新の必要があるかとりあえず3位を確認
            if (now_max_magnification[2] < magnification) {
                // 1位
                if (now_max_magnification[0] < magnification) {
                    now_max_magnification[2] = now_max_magnification[1];
                    now_max_magnification[1] = now_max_magnification[0];
                    now_max_magnification[0] = magnification;
                    now_max_board[2].setBoard(now_max_board[1].getBoard());
                    now_max_board[1].setBoard(now_max_board[0].getBoard());
                    now_max_board[0].setBoard(tpb.getBoard());
                }
                // 同率1位
                else if (now_max_magnification[0] == magnification) {
                    // 考慮しない
                }
                // 2位
                else if (now_max_magnification[1] < magnification) {
                    now_max_magnification[2] = now_max_magnification[1];
                    now_max_magnification[1] = magnification;
                    now_max_board[2].setBoard(now_max_board[1].getBoard());
                    now_max_board[1].setBoard(tpb.getBoard());
                }
                // 同率2位
                else if (now_max_magnification[1] == magnification) {
                    // 考慮しない
                }
                // 3位
                else {
                    now_max_magnification[2] = magnification;
                    now_max_board[2].setBoard(tpb.getBoard());
                }
            }
        }
        sqlite3_reset(pStmt);
        sqlite3_clear_bindings(pStmt);
        // コミット
        sqlite3_exec(db, "COMMIT TRANSACTION", 0, 0, 0);
        // 再度トランザクション開始
        sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);
    }
    // コミット
    sqlite3_exec(db, "COMMIT TRANSACTION", 0, 0, 0);
    sqlite3_finalize(pStmt);

    // 時間表示
    clock_t end_time = clock();
    cout << endl;

    const double time = (static_cast<double>(end_time) - static_cast<double>(start_time)) / CLOCKS_PER_SEC;
    cout << "処理時間 : " << time << endl;
    cout << endl;

    // ファイル出力
    stringstream file_name_stream;
    file_name_stream << pi.getNextColor() << "_" << pi.getBoardPattern() << "_" << pi.getMaxTrace() << "_" << pi.getEliminationCoefficient() << "_" << pi.getChainCoefficient() << "_" << pi.getMaxColor() << ".txt";

    ofstream outputfile(file_name_stream.str());
    outputfile << "ネクストの色       : " << pi.getNextColor() << "\n";
    outputfile << "盤面パターン       : " << pi.getBoardPattern() << "\n";
    outputfile << "最大なぞり消し数   : " << pi.getMaxTrace() << "\n";
    outputfile << "同時消し係数       : " << pi.getEliminationCoefficient() << "\n";
    outputfile << "連鎖係数           : " << pi.getChainCoefficient() << "\n";
    outputfile << "求めたい色         : " << pi.getMaxColor() << "\n";
    outputfile << "消える時の結合数   : " << pi.getMaxConnection() << "\n";
    outputfile << "\n";
    for (int i = 0; i < 3; ++i) {
        // 結果表示のためにもう一度計算
        Fixed_Next fnext(pi.getNextColor());
        Fixed_Board fboard(pi.getBoardPattern());
        Chain_Info ci(&pi, &fnext, &fboard, &(now_max_board[i]));
        outputfile << "--------------------倍率" << i + 1 << "位--------------------" << "\n";
        for (int j = 0; j < row_size; ++j) {
            outputfile
                << now_max_board[i].getBoardElement(j * column_size)
                << now_max_board[i].getBoardElement(j * column_size + 1)
                << now_max_board[i].getBoardElement(j * column_size + 2)
                << now_max_board[i].getBoardElement(j * column_size + 3)
                << now_max_board[i].getBoardElement(j * column_size + 4)
                << now_max_board[i].getBoardElement(j * column_size + 5)
                << now_max_board[i].getBoardElement(j * column_size + 6)
                << now_max_board[i].getBoardElement(j * column_size + 7) << "\n";
        }
        outputfile << "\n";
        outputfile << "-----------------------------------------" << "\n";
        outputfile << "|   色   |消去数|  倍率  |  発生値倍率  |" << "\n";
        outputfile << "-----------------------------------------" << "\n";
        outputfile << "|   赤   |" << right << setw(6) << ci.getElementCountPerColor(Red)    << "|" << right << setw(8) << fixed << setprecision(2) << ci.getMagnificationPerColor(&pi, Red)    << "|" << right << setw(8) << fixed << setprecision(2) << ci.getMagnificationPerColor(&pi, Red)    * 6.0 << "(x6.0)|" << "\n";
        outputfile << "|   青   |" << right << setw(6) << ci.getElementCountPerColor(Blue)   << "|" << right << setw(8) << fixed << setprecision(2) << ci.getMagnificationPerColor(&pi, Blue)   << "|" << right << setw(8) << fixed << setprecision(2) << ci.getMagnificationPerColor(&pi, Blue)   * 6.2 << "(x6.2)|" << "\n";
        outputfile << "|   緑   |" << right << setw(6) << ci.getElementCountPerColor(Green)  << "|" << right << setw(8) << fixed << setprecision(2) << ci.getMagnificationPerColor(&pi, Green)  << "|" << right << setw(8) << fixed << setprecision(2) << ci.getMagnificationPerColor(&pi, Green)  * 4.5 << "(x4.5)|" << "\n";
        outputfile << "|   黄   |" << right << setw(6) << ci.getElementCountPerColor(Yellow) << "|" << right << setw(8) << fixed << setprecision(2) << ci.getMagnificationPerColor(&pi, Yellow) << "|" << right << setw(8) << fixed << setprecision(2) << ci.getMagnificationPerColor(&pi, Yellow) * 5.5 << "(x5.5)|" << "\n";
        outputfile << "|   紫   |" << right << setw(6) << ci.getElementCountPerColor(Purple) << "|" << right << setw(8) << fixed << setprecision(2) << ci.getMagnificationPerColor(&pi, Purple) << "|" << right << setw(8) << fixed << setprecision(2) << ci.getMagnificationPerColor(&pi, Purple) * 5.5 << "(x5.5)|" << "\n";
        outputfile << "|ワイルド| ---- |"                                                           << right << setw(8) << fixed << setprecision(2) << ci.getMagnificationPerColor(&pi, None)   << "|" << right << setw(8) << fixed << setprecision(2) << ci.getMagnificationPerColor(&pi, None)   * 2.2 << "(x2.2)|" << "\n";
        outputfile << "-----------------------------------------" << "\n";
        outputfile << "\n";
    }
    outputfile << "処理時間(s) : " << time;
    outputfile << "\n";
    outputfile.close();

    // DB切断
    ret = sqlite3_close_v2(db);
    if (ret == SQLITE_OK) {
        printf("Disconnection Success!\n");
    }
    else {
        printf("Disconnection Failed...\n");
    }
}

long long getTracePatternSize(const int max_trace)
{
    long long size = 0;
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
    case 14:
        size = 5425777501;
        break;
    case 15:
        size = 17710655729;
        break;
    default:
        break;
    }
    return size;
}