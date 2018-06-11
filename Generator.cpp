#include <algorithm>
#include <numeric>
#include "Generator.h"

using namespace std;

// 初期化
void Generator::init()
{
	mt.seed(random_device()());

	posList.resize(GRID_SIZE);
	iota(posList.begin(), posList.end(), 0);

	numList.resize(BOARD_SIZE);
	iota(numList.begin(), numList.end(), 0);
}
void Generator::initPuzzle(const int clueCount)
{
	uniform_int_distribution<> dist9(0, BOARD_SIZE - 1);
	uniform_int_distribution<> dist81(0, GRID_SIZE - 1);

	Solver solver;

	while (true) {
		// 初期化
		int count = 0;
		memset(puzzle, 0, sizeof(puzzle));
		solver.init();

		// ランダムな問題生成
		while (true) {
			// ヒントを1つ追加する
			const int p = dist81(mt);
			if (puzzle[p] != 0) continue;
			const int n = dist9(mt);
			if (solver.getCandidate(p, n) == 0) continue;
			puzzle[p] = n + 1;
			solver.putNumber(p, n);

			if (solver.getError() == true) break; // 解無し → 作り直し
			if (++count == clueCount) return; // 生成完了
		}
	}
}

// 局所探索
bool Generator::localSearch(int& score)
{
	// ヒントの位置を調べる
	vector<int> cluePosList;
	for (int i = 0; i < GRID_SIZE; ++i)
		if (puzzle[i] != 0) cluePosList.push_back(i);
	const int clueCount = cluePosList.size();

	// 探索順をシャッフル
	shuffle(cluePosList.begin(), cluePosList.end(), mt);
	shuffle(posList.begin(), posList.end(), mt);
	shuffle(numList.begin(), numList.end(), mt);

	Solver baseSolver, copySolver;

	for (const int old_p : cluePosList) {
		// ヒントを1つ抜く
		const int old_n = puzzle[old_p];
		puzzle[old_p] = 0;
		baseSolver.setPuzzle(puzzle);
		puzzle[old_p] = old_n;

		// ヒントを1つ入れる
		for (const int new_p : posList) {
			if (baseSolver.getAnswer(new_p) != 0) continue;
			for (const int new_n : numList) {
				if (baseSolver.getCandidate(new_p, new_n) == 0) continue;
				copySolver = baseSolver;
				copySolver.putNumber(new_p, new_n);
				if (copySolver.getError() == true) continue;
				const int tmpScore = copySolver.getScore();

				// 評価値が良いなら更新する
				if (tmpScore < score) {
					puzzle[old_p] = 0;
					puzzle[new_p] = new_n + 1;
					score = tmpScore;
					return true;
				}
			}
		}
	}
	return false;
}
bool Generator::generatePuzzle(const int clueCount)
{
	// ランダムに問題を生成する
	initPuzzle(clueCount);

	// 最初のスコアを計算する
	Solver solver;
	solver.setPuzzle(puzzle);
	int score = solver.getScore();

	// 評価が良くなるようにヒントを入れ替える
	do {
		if (score == 0) return true; // 生成成功
	} while (localSearch(score));

	return false; // 生成失敗
}
