#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Generator.h"

using namespace std;

static string puzzleToString(const int puzzle[GRID_SIZE])
{
	stringstream ss;

	for (int i = 0; i <= GRID_SIZE; ++i) {
		if (i % (BOARD_SIZE * BOX_Y) == 0) {
			for (int j = 0; j < BOX_Y; ++j) {
				ss << "+";
				for (int k = 0; k < BOX_X * 2; ++k)
					ss << "-";
			}
			ss << "+" << endl;
		}
		if (i == GRID_SIZE) break;
		if (i % BOX_X == 0) ss << "|";
		if (puzzle[i] == 0) ss << " .";
		else if (puzzle[i] < 10) ss << " " << puzzle[i];
		else ss << " " << char('a' + puzzle[i] - 10);
		if ((i + 1) % BOARD_SIZE == 0) ss << "|" << endl;
	}
	return ss.str();
}

int solve(const char* const fileName)
{
	// ファイル読み込み
	ifstream ifs(fileName);
	if (ifs.fail() == true) {
		cout << "No such file: " << fileName << endl;
		return -1;
	}
	
	Solver solver;

	int puzzleCount = 0;		// 問題数
	int solvedCount = 0;		// 解けた問題数
	int noSolutionCount = 0;	// 解無しの問題数
	int unknownCount = 0;		// 実装されている手筋では解くことができなかった問題数

	auto start = chrono::system_clock::now();

	// 1行ずつ解く
	string str;
	while (getline(ifs, str)) {
		solver.setPuzzle(str);
		if (solver.getError()) {
			++noSolutionCount; // 解無し
		}
		else{
			if (solver.getSpaceCount() == 0)
				++solvedCount; // 唯一解
			else
				++unknownCount; // 不明
		}
		++puzzleCount;
	}

	chrono::duration<double> sec = chrono::system_clock::now() - start;

	// 結果
	cout << "Puzzle Count: " << puzzleCount << endl;
	cout << "Solved Count: " << solvedCount << endl;
	cout << "No Solution Count: " << noSolutionCount << endl;
	cout << "Unknown Count: " << unknownCount << endl;
	cout << "Time: " << sec.count() << "sec" << endl;

	return 0;
}
int generate(const int clueCount)
{
	// 最少ヒント数以上か？
	int min = GRID_SIZE / 4 - (BOX_X - 1) * (BOX_Y - 1);
	if (BOX_X == 2 || BOX_Y == 2 || (BOX_X == 3 && BOX_Y == 3)) ++min;
	if (clueCount < min || GRID_SIZE < clueCount) return -1;

	auto start = chrono::system_clock::now();
	int trialCount = 0;

	// 生成
	Generator generator;
	bool flag = false;
	chrono::duration<double> sec;
	while (flag == false) {
		++trialCount;
		flag = generator.generatePuzzle(clueCount);
		sec = chrono::system_clock::now() - start;
		cout << trialCount << ", " << sec.count() << "sec" << endl;
	}

	// 出力
	cout << puzzleToString(generator.getPuzzle()) << endl;

	return 0;
}

int main(int argc, char *argv[])
{
	// 初期化
	Solver::initConstraint();

	// コマンド
	for (int i = 0; i < argc; ++i) {
		if (strcmp(argv[i], "-solve") == 0 && i + 1 < argc)
			return solve(argv[i + 1]);
		if (strcmp(argv[i], "-generate") == 0 && i + 1 < argc)
			return generate(atoi(argv[i + 1]));
	}

	// ヘルプ
	cout << argv[0];
	cout << " [-solve filename]";
	cout << " [-generate clues]" << endl;

	return 0;
}
