#ifndef __SOLVER__
#define __SOLVER__

#include <climits>
#include <cstring>
#include <string>
#include <vector>

// ブロックのサイズ
#define BOX_X 3 // 3
#define BOX_Y 3 // 3

#define BOARD_SIZE (BOX_X * BOX_Y)						// 9
#define GRID_SIZE (BOARD_SIZE * BOARD_SIZE)				// 81

class Solver
{
	// ブロック,行,列の番号 → マス
	static std::vector< std::vector<int> > house2cells;		// house2cells[idx] <- cells (box,row,column)
	// マス → ブロック,行,列の番号
	static std::vector< std::vector<int> > cell2houses;		// cell2houses[cell] <- idxs
	// マス → ブロック,行,列内の位置
	static std::vector< std::vector<int> > cell2housePoss;	// cell2housePos[cell] <- positions
	// マス → 周囲のマス
	static std::vector< std::vector<int> > cell2cells;		// cell2cells[cell] <- cells(box + row + column - cell)
	// 数字 → 他の数字
	static int num2othernums[BOARD_SIZE][BOARD_SIZE - 1];	// num2othernums[n] <- {0,1,2,...,BOARD_SIZE-1} - {n}

	static void init_house2cells();
	static void init_cell2houses();
	static void init_cell2cells();
	static void init_num2otherNums();

public:
	// ↑の初期化
	static void initConstraint();
	
private:
	// 解答
	//   初期値 : 0,0,0,… (長さGRID_SIZE)
	int answer[GRID_SIZE];

	// 空きマスの数
	//   初期値 : GRID_SIZE
	int spaceCount;

	// 候補
	//   初期値 : (1 << BOARD_SIZE) - 1
	int candidates[GRID_SIZE];							// [p] <- bit
	std::vector< std::vector<int> > houseCandidates;	// [h][n] <- bit

	// 解無しを判定するフラグ
	//   初期値 : false
	//            true (解無し)
	bool errorFlag;

public:
	void init();
	Solver() { init(); }

	// コピー
	Solver& operator=(const Solver& obj) {
		memcpy(answer, obj.answer, sizeof(obj.answer));
		spaceCount = obj.spaceCount;
		memcpy(candidates, obj.candidates, sizeof(obj.candidates));
		houseCandidates = obj.houseCandidates;
		errorFlag = obj.errorFlag;
		return *this;
	}

	// 問題代入
	//    puz : 長さGRID_SIZE, 1-9,a-z以外の文字や値は0とする
	//     例 : 4......1..2...6..8....186341.......7....63...5.....2...8......1...5.2.....4...95.
	//     例 : 083000000500000027090000000000230050908010000000500480200040800000000061000700500
	void setPuzzle(const std::string& puz);
	void setPuzzle(const int puz[GRID_SIZE]);

	// 候補の有無を調べる
	//   p : 調べる位置 (0 - (GRID_SIZE-1))
	//   n : 調べる数字 (0 - (BOARD_SIZE-1))
	//   return : 候補の有無 (1-:有, 0:無)
	int getCandidate(const int p, const int n)const { return (candidates[p] & (1 << n)); }

	// 候補を削除する
	//   p : 削除する位置 (0 - (GRID_SIZE-1))
	//   n : 削除する数字 (0 - (BOARD_SIZE-1))
	void deleteCandidate(const int p, const int n);

	// ヒントの追加
	//   p : 数字を追加する位置 (0 - (GRID_SIZE-1))
	//   n : 追加する数字 (0 - (BOARD_SIZE-1))
	void putNumber(const int p, const int n);

	// 空きマスの数を調べる
	//   return : 空きマス数 (0 - GRID_SIZE)
	int getSpaceCount()const { return spaceCount; }

	// 各マスの候補数の平方和
	//   return : 平方和 (GRID_SIZE - BOARD_SIZE*BOARD_SIZE*GRID_SIZE)
	int getCandidateSquareSum()const;

	// 問題の評価値を計算する
	//   return : 評価値 (0-)
	int getScore()const {
		int score = getCandidateSquareSum() - GRID_SIZE;
		if (errorFlag == true) score += BOARD_SIZE * BOARD_SIZE * GRID_SIZE;
		return score;
	}

	// 解答
	const int* const getAnswer()const { return answer; }
	int getAnswer(const int p)const { return answer[p]; }

	// 解無しか？
	//   return : 解無しか (true:解無し, false:不明)
	bool getError()const { return errorFlag; }
};

#endif