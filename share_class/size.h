#pragma once

constexpr int row_size = 6;							// 縦
constexpr int column_size = 8;						// 横
constexpr int board_size = row_size * column_size;	// 全体
constexpr int next_size = column_size;				// ネクストの大きさ
constexpr int end_of_board = board_size - 1;		// 
constexpr int max_num_of_chain = 16;