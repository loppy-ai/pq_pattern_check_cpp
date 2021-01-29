#include <iostream>
#include <string>
#include <iomanip>
#include <pqxx/pqxx>
#include "board.h"
#include "size.h"
#include "print.h"

void checkBoard(Board*, int, int, int, int*, std::string*);
bool isConnection(Board*, const int);
int recursionCheckConnection(Board*, const int, int);

int main()
{
    int all_count = 422742049;
    /*
    1 は  1 ~  48
    2 は 49 ~ 200
    2からDBに登録したい場合は、all_countに48をセットすること    
    */
    Board board;
    int max_trace = 9;     // 最大なぞり消し数
    int now_trace = 1;
    int nest_level = 0;
    std::string query = "";

    clock_t start_time = clock();
    std::cout << "\n" << "---------------------処理開始---------------------" << std::endl;
    
    //for (int i = 0; i < max_trace; ++i) {    // 1～max_traceを登録
    { int i = max_trace - 1;   // max_traceだけ登録
        checkBoard(&board, nest_level, now_trace, i+1, &all_count, &query);

        // 余り分を登録
        if (!query.empty()) {
            try {
                pqxx::connection conn("dbname=pqdb");
                pqxx::work tran(conn);
                // 最後のカンマを消す
                query.pop_back();
                std::stringstream query_stream;
                query_stream << "INSERT INTO pattern13 (id, trace, board) VALUES " << query << ";" << std::endl;
                pqxx::result result(tran.exec(query_stream.str()));
                tran.commit();
                conn.disconnect();
            }
            catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
                exit(1);
            }
        }
        query = "";
    }
    clock_t end_time = clock();
    std::cout << "\n" << "---------------------処理終了---------------------" << std::endl;
    const double time = (static_cast<double>(end_time) - static_cast<double>(start_time)) / CLOCKS_PER_SEC;
    std::cout << "処理件数 : " << all_count << std::endl;
    std::cout << "処理時間 : " << time << std::endl;
    std::cout << std::endl;
    system("PAUSE");
}

// 盤面生成
void checkBoard(Board* board, int nest_level, int now_trace, int max_trace, int* all_count, std::string* query)
{
    int end = board_size - (max_trace - now_trace);
    for (int i = nest_level; i < end; ++i) {
        nest_level += 1;
        board->setBoardElement(i, 1);
        if (now_trace != max_trace) {
            // 組み合わせ探索を継続
            now_trace += 1;
            checkBoard(board, nest_level, now_trace, max_trace, all_count, query);
            now_trace -= 1;
        }
        else {
            if (max_trace == 1) {
                // 1の時だけは結合チェックが不要
                *all_count += 1;
                // boardをboard_size数の文字列にする
                std::string board_string;
                for (int j = 0; j < board_size; ++j) {
                    board_string += std::to_string(board->getBoardElement(j));
                }
                // 文字列作成
                std::stringstream query_ss;
                query_ss << "(" << *all_count << "," << max_trace << ",'" << board_string << "'),";
                *query += query_ss.str();
            }
            else {
                // 結合チェック
                if (isConnection(board, max_trace)) {
                    *all_count += 1;
                    // boardをboard_size数の文字列にする
                    std::string board_string;
                    for (int j = 0; j < board_size; ++j) {
                        board_string += std::to_string(board->getBoardElement(j));
                    }
                    // 文字列作成
                    std::stringstream query_ss;
                    query_ss << "(" << *all_count << "," << max_trace << ",'" << board_string << "'),";
                    *query += query_ss.str();
                    if ((*all_count & 1048575) == 0) {      // 131071 = 2^17 - 1
                        // クエリ作成
                        try {
                            pqxx::connection conn("dbname=pqdb");
                            pqxx::work tran(conn);
                            // 最後のカンマを消す
                            query->pop_back();
                            std::stringstream query_stream;
                            query_stream << "INSERT INTO pattern13 (id, trace, board) VALUES " << *query << ";" << std::endl;
                            pqxx::result result(tran.exec(query_stream.str()));
                            tran.commit();
                            conn.disconnect();
                            *query = "";
                            std::cout << std::right << std::setw(10) << *all_count << "件完了 " << getTimeStamp() << std::endl;
                        }
                        catch (const std::exception& e) {
                            std::cerr << e.what() << std::endl;
                            exit(1);
                        }
                    }
                } // end if (isConnection())
            } // end else
        }
        board->setBoardElement(i, 0);
    } // end for
}

// 結合チェック
bool isConnection(Board* board, const int max_trace)
{
    for (int i = 0; i < board_size; ++i) {
        if (board->getBoardElement(i) == 1) {
            // for文で書いているけど1回しか呼ばれない
            int count = 1;
            Board check_board;
            check_board.setBoard(board->getBoard());
            count = recursionCheckConnection(&check_board, i, count);
            if (count == max_trace) {
                return true;
            }
            else {
                return false;
            }
        }
    }
    return false;
}

// 結合チェック（再帰）
int recursionCheckConnection(Board* board, const int no, int count)
{
    board->setBoardElement(no, 2);
    if (board->canGetLeftColumn(no) && board->getBoardElement(no - 1) == 1) {
        // 左にいける
        count = recursionCheckConnection(board, no - 1, count + 1);
    }
    if (board->canGetLeftColumn(no) && board->canGetUpperRow(no) && board->getBoardElement(no - 9) == 1) {
        // 左上にいける
        count = recursionCheckConnection(board, no - 9, count + 1);
    }
    if (board->canGetUpperRow(no) && board->getBoardElement(no - 8) == 1) {
        // 上にいける
        count = recursionCheckConnection(board, no - 8, count + 1);
    }
    if (board->canGetRightColumn(no) && board->canGetUpperRow(no) && board->getBoardElement(no - 7) == 1) {
        // 右上にいける
        count = recursionCheckConnection(board, no - 7, count + 1);
    }
    if (board->canGetRightColumn(no) && board->getBoardElement(no + 1) == 1) {
        // 右にいける
        count = recursionCheckConnection(board, no + 1, count + 1);
    }
    if (board->canGetRightColumn(no) && board->canGetLowerRow(no) && board->getBoardElement(no + 9) == 1) {
        // 右下にいける
        count = recursionCheckConnection(board, no + 9, count + 1);
    }
    if (board->canGetLowerRow(no) && board->getBoardElement(no + 8) == 1) {
        // 下にいける
        count = recursionCheckConnection(board, no + 8, count + 1);
    }
    if (board->canGetLeftColumn(no) && board->canGetLowerRow(no) && board->getBoardElement(no + 7) == 1) {
        // 左下にいける
        count = recursionCheckConnection(board, no + 7, count + 1);
    }
    return count;
}