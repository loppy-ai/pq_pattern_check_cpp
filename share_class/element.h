#pragma once

enum Element
{
	Elimination,	// なぞった or 消えた瞬間
	Red,			// 赤ぷよ
	Blue,			// 青ぷよ
	Green,			// 緑ぷよ
	Yellow,			// 黄ぷよ
	Purple,			// 紫ぷよ
	Ojama,			// おじゃまぷよ
	Kata,			// かたぷよ
	Heart,			// ハートBOX
	None,			// 既に消えた箇所（もしくはワイルド状態）
	Prism			// プリズムボール
};