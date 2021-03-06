#ifndef __GENERATOR_LS__
#define __GENERATOR_LS__

#include <random>
#include "Solver.h"

class Generator
{
	std::mt19937 mt;

	// 探索順
	std::vector<int> posList; // posList <- {0, 1, 2, ... , GRID_SIZE-1}
	std::vector<int> numList; // numList <- {0, 1, 2, ... , BOARD_SIZE-1}

	// 生成中の問題
	int puzzle[GRID_SIZE];

	void init();

	// 問題(puzzle)を初期化する
	//   clueCount : ヒント数
	void initPuzzle(const int clueCount);

	// 探索
	//   score : puzzleの評価値
	//   return : 評価が良くなったか (true:良くなった, false:嵌った)
	bool localSearch(int& score);

public:
	Generator() { init(); }

	// ナンプレを生成する
	//   clueCount : ヒント数
	//   return : 生成できたか (true:成功, false:失敗)
	bool generatePuzzle(const int clueCount);

	// 生成した問題
	const int* const getPuzzle()const { return puzzle; }
};

#endif