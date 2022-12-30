#include <iostream>
#include <string>
#include <sqlite3.h>
#include <iomanip>
#include "board.h"
#include "size.h"
#include "print.h"

using namespace std;

void checkBoard(Board*, int, int, int, int64_t*, sqlite3_stmt*, sqlite3*);
bool isConnection(Board*, const int);
int recursionCheckConnection(Board*, const int, int);

int main()
{
    int64_t all_count = 0;
    /*
    1 は  1 ~  48
    2 は 49 ~ 200
    2からDBに登録したい場合は、all_countに48をセットすること
    */
    Board board;
    int max_trace = 15;     // 最大なぞり消し数
    int now_trace = 1;
    int nest_level = 0;
    sqlite3* db = NULL;
    sqlite3_stmt* pStmt;
    const char* path = "D:\\sqlite\\tp.sqlite3";
    int ret;

    // 接続
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

    // 追加開始
    clock_t start_time = clock();
    cout << "\n" << "---------------------処理開始---------------------" << endl;

    // トランザクション開始
    sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);

    sqlite3_prepare_v2(
        db,
        "INSERT INTO pattern(id, trace, board) VALUES(?, ?, ?);",
        -1,
        &pStmt,
        nullptr
    );

    for (int i = 1; i <= max_trace; ++i) {    // 1～max_traceを登録
        checkBoard(&board, nest_level, now_trace, i, &all_count, pStmt, db);
        cout << "最大なぞり : " << i << " 完了 ( 件数 : " << all_count << " )" << endl;
    }
    // 残りのコミット
    sqlite3_exec(db, "COMMIT TRANSACTION", 0, 0, 0);
    sqlite3_finalize(pStmt);

    clock_t end_time = clock();
    cout << "\n" << "---------------------処理終了---------------------" << endl;
    const double time = (static_cast<double>(end_time) - static_cast<double>(start_time)) / CLOCKS_PER_SEC;
    cout << "処理件数 : " << all_count << endl;
    cout << "処理時間 : " << time << endl;
    cout << endl;

    // 切断
    ret = sqlite3_close_v2(db);
    if (ret == SQLITE_OK) {
        printf("Disconnection Success!\n");
    }
    else {
        printf("Disconnection Failed...\n");
    }
    system("PAUSE");
}

// 盤面生成
void checkBoard(Board* board, int nest_level, int now_trace, int max_trace, int64_t* all_count, sqlite3_stmt* pStmt, sqlite3* db)
{
    int end = board_size - (max_trace - now_trace);
    for (int i = nest_level; i < end; ++i) {
        nest_level += 1;
        board->setBoardElement(i, 1);
        if (now_trace != max_trace) {
            // 組み合わせ探索を継続
            now_trace += 1;
            checkBoard(board, nest_level, now_trace, max_trace, all_count, pStmt, db);
            now_trace -= 1;
        }
        else {
            if (max_trace == 1) {
                // 1の時だけは結合チェックが不要
                *all_count += 1;
                // boardをboard_size数の文字列にする
                string board_string;
                for (int j = 0; j < board_size; ++j) {
                    board_string += to_string(board->getBoardElement(j));
                }
                // DB登録
                sqlite3_bind_int64(pStmt, 1, *all_count);
                sqlite3_bind_int64(pStmt, 2, max_trace);
                sqlite3_bind_text(pStmt, 3, board_string.c_str(), board_size, SQLITE_TRANSIENT);
                while (sqlite3_step(pStmt) == SQLITE_BUSY) {}
                sqlite3_reset(pStmt);
                sqlite3_clear_bindings(pStmt);
            }
            else {
                // 結合チェック
                if (isConnection(board, max_trace)) {
                    *all_count += 1;
                    if ((*all_count & 16777215) == 0) {
                        // コミット
                        sqlite3_exec(db, "COMMIT TRANSACTION", 0, 0, 0);
                        cout << *all_count << "件 " << getTimeStamp() << endl;
                        // 再度トランザクション開始
                        sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);
                    }
                    // boardをboard_size数の文字列にする
                    string board_string;
                    for (int j = 0; j < board_size; ++j) {
                        board_string += to_string(board->getBoardElement(j));
                    }
                    // DB登録
                    sqlite3_bind_int64(pStmt, 1, *all_count);
                    sqlite3_bind_int64(pStmt, 2, max_trace);
                    sqlite3_bind_text(pStmt, 3, board_string.c_str(), board_size, SQLITE_TRANSIENT);
                    while (sqlite3_step(pStmt) == SQLITE_BUSY) {}
                    sqlite3_reset(pStmt);
                    sqlite3_clear_bindings(pStmt);
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
