#include <algorithm>
#include "Solver.h"

using namespace std;

static inline int popcnt(int n)
{
#if defined _MSC_VER
	return __popcnt(n);
#elif defined __GNUC__
	return __builtin_popcount(n);
#endif
}
static inline int ctz(int n)
{
#if defined _MSC_VER
	unsigned long idx;
	_BitScanForward(&idx, n);
	return idx;
#elif defined __GNUC__
	return __builtin_ctz(n);
#endif
}

vector< vector<int> > Solver::house2cells;		// house2cells[house] <- cells (box,row,column)
vector< vector<int> > Solver::cell2houses;		// cell2houses[cell] <- houses
vector< vector<int> > Solver::cell2housePoss;	// cell2housePoss[cell] <- positions
vector< vector<int> > Solver::cell2cells;		// cell2cells[cell] <- cells(box + row + column - cell)
int Solver::num2othernums[BOARD_SIZE][BOARD_SIZE - 1];	// num2othernums[n] <- {0,1,2,...,BOARD_SIZE-1} - {n}

void Solver::init_house2cells()
{
	// box
	//   (BOX_X == 3 && BOX_Y == 3 ‚Ìê‡)
	//     { 0, 1, 2, 9,10,11,18,19,20},
	//     { 3, 4, 5,12,13,14,21,22,23}, ...
	for (int i = 0; i < BOARD_SIZE; ++i) {
		const int bx = i % BOX_Y * BOX_X;
		const int by = i / BOX_Y * BOX_Y;
		vector<int> box;
		for (int j = 0; j < BOARD_SIZE; ++j)
			box.push_back((bx + j % BOX_X) + (by + j / BOX_X) * BOARD_SIZE);
		house2cells.push_back(box);
	}

	// row
	//   (BOX_X == 3 && BOX_Y == 3 ‚Ìê‡)
	//     { 0, 1, 2, 3, 4, 5, 6, 7, 8},
	//     { 9,10,11,12,13,14,15,16,17}, ...
	for (int y = 0; y < BOARD_SIZE; ++y) {
		vector<int> row;
		for (int x = 0; x < BOARD_SIZE; ++x)
			row.push_back(x + y * BOARD_SIZE);
		house2cells.push_back(row);
	}

	// column
	//   (BOX_X == 3 && BOX_Y == 3 ‚Ìê‡)
	//     { 0, 9,18,27,36,45,54,63,72},
	//     { 1,10,19,28,37,46,55,64,73}, ...
	for (int x = 0; x < BOARD_SIZE; ++x) {
		vector<int> column;
		for (int y = 0; y < BOARD_SIZE; ++y)
			column.push_back(x + y * BOARD_SIZE);
		house2cells.push_back(column);
	}

	// diagonal
	//   (BOX_X == 3 && BOX_Y == 3 ‚Ìê‡)
	//     { 0,10,20,30,40,50,60,70,80}
	//     { 8,16,24,32,40,48,56,64,72}
	if (false) {
		vector<int> mainDiagonal;
		vector<int> antiDiagonal;
		for (int i = 0; i < BOARD_SIZE; ++i) {
			mainDiagonal.push_back(i + i * BOARD_SIZE);
			antiDiagonal.push_back((BOARD_SIZE - 1 - i) + i * BOARD_SIZE);
		}
		house2cells.push_back(mainDiagonal);
		house2cells.push_back(antiDiagonal);
	}
}
void Solver::init_cell2houses()
{
	// cell2houses
	//   (BOX_X == 3 && BOX_Y == 3 ‚Ìê‡)
	//     { 0, 9,18}, { 0, 9,19}, { 0, 9,20}, ...
	// cell2housePoss
	//   (BOX_X == 3 && BOX_Y == 3 ‚Ìê‡)
	//     { 0, 0, 0}, { 1, 1, 0}, { 2, 2, 0}, ...
	cell2houses.resize(GRID_SIZE);
	cell2housePoss.resize(GRID_SIZE);
	for (int i = 0; i < house2cells.size(); ++i) {
		for (int j = 0; j < house2cells[i].size(); ++j) {
			cell2houses[house2cells[i][j]].push_back(i);
			cell2housePoss[house2cells[i][j]].push_back(j);
		}
	}
}
void Solver::init_cell2cells()
{
	// cell2cells
	//   (BOX_X == 3 && BOX_Y == 3 ‚Ìê‡)
	//     { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,18,19,20,27,36,45,54,63,72},
	//     { 0, 2, 3, 4, 5, 6, 7, 8, 9,10,11,18,19,20,28,37,46,55,64,73}, ...
	cell2cells.resize(GRID_SIZE);
	for (int i = 0; i < GRID_SIZE; ++i) {
		for (int j : cell2houses[i])
			for (int k : house2cells[j])
				if (i != k) cell2cells[i].push_back(k);
		sort(cell2cells[i].begin(), cell2cells[i].end());
		cell2cells[i].erase(unique(cell2cells[i].begin(), cell2cells[i].end()), cell2cells[i].end());
	}
}
void Solver::init_num2otherNums()
{
	// num2othernums
	//   (BOARD_SIZE == 9 ‚Ìê‡)
	//     { 1, 2, 3, 4, 5, 6, 7, 8},
	//     { 0, 2, 3, 4, 5, 6, 7, 8}, ...
	for (int i = 0; i < BOARD_SIZE; ++i) {
		int p = 0;
		for (int j = 0; j < BOARD_SIZE; ++j)
			if (i != j) num2othernums[i][p++] = j;
	}
}

void Solver::initConstraint()
{
	init_house2cells();
	init_cell2houses();
	init_cell2cells();
	init_num2otherNums();
}

void Solver::init()
{
	memset(answer, 0, sizeof(answer));
	spaceCount = GRID_SIZE;

	const int x1ff = (1 << BOARD_SIZE) - 1;
	for (int i = 0; i < GRID_SIZE; ++i)
		candidates[i] = x1ff;

	houseCandidates = vector< vector<int> >(house2cells.size(), vector<int>(BOARD_SIZE, x1ff));
	errorFlag = false;
}

// –â‘è‘ã“ü 1-9
void Solver::setPuzzle(const string& puz)
{
	init();
	const int len = puz.length();
	for (int i = 0; i < len && i < GRID_SIZE; ++i) {
		const int n = puz[i] - '0';
		const int a = puz[i] - 'a' + 10;
		if (1 <= n && n <= BOARD_SIZE)
			putNumber(i, n - 1);
		else if (10 <= a && a <= BOARD_SIZE)
			putNumber(i, n - 1);
	}
}
void Solver::setPuzzle(const int puz[GRID_SIZE])
{
	init();
	for (int i = 0; i < GRID_SIZE; ++i)
		if (1 <= puz[i] && puz[i] <= BOARD_SIZE)
			putNumber(i, puz[i] - 1);
}

// Œó•â‚Ìíœ 0-8
void Solver::deleteCandidate(const int p, const int n)
{
	if ((candidates[p] & (1 << n)) == 0 || errorFlag == true) return;

	// delete
	candidates[p] &= ~(1 << n);
	const int cell2houses_p_size = cell2houses[p].size();
	for (int i = 0; i < cell2houses_p_size; ++i)
		houseCandidates[cell2houses[p][i]][n] &= ~(1 << cell2housePoss[p][i]);

	// hidden single
	for (int i = 0; i < cell2houses_p_size; ++i) {
		const int h = cell2houses[p][i];
		switch (popcnt(houseCandidates[h][n])) {
		case 0:
			errorFlag = true;
			return;
			break;
		case 1:
			putNumber(house2cells[h][ctz(houseCandidates[h][n])], n);
			break;
		}
	}

	// naked single
	switch (popcnt(candidates[p])) {
	case 0:
		errorFlag = true;
		return;
		break;
	case 1:
		putNumber(p, ctz(candidates[p]));
		break;
	}
}

// ”Žš‚Ì’Ç‰Á 0-8
void Solver::putNumber(const int p, const int n)
{
	if (errorFlag == true) return;
	if (answer[p] != 0) {
		if (answer[p] != n + 1) errorFlag = true;
		return;
	}

	if ((candidates[p] & (1 << n)) == 0) {
		errorFlag = true;
		return;
	}

	// Šm’èˆ—
	answer[p] = n + 1;
	--spaceCount;

	// Œó•â‚Ìíœ
	for (const int i : cell2cells[p])
		deleteCandidate(i, n);
	for (int i = 0; i < BOARD_SIZE - 1; ++i)
		deleteCandidate(p, num2othernums[n][i]);
}

// Œó•â”‚Ì•½•û˜a
int Solver::getCandidateSquareSum()const
{
	int sum = 0;
	for (int i = 0; i < GRID_SIZE; ++i) {
		const int cnt = popcnt(candidates[i]);
		sum += cnt * cnt;
	}
	return sum;
}
