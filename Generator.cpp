#include <algorithm>
#include <numeric>
#include "Generator.h"

using namespace std;

// ������
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
		// ������
		int count = 0;
		memset(puzzle, 0, sizeof(puzzle));
		solver.init();

		// �����_���Ȗ�萶��
		while (true) {
			// �q���g��1�ǉ�����
			const int p = dist81(mt);
			if (puzzle[p] != 0) continue;
			const int n = dist9(mt);
			if (solver.getCandidate(p, n) == 0) continue;
			puzzle[p] = n + 1;
			solver.putNumber(p, n);

			if (solver.getError() == true) break; // �𖳂� �� ��蒼��
			if (++count == clueCount) return; // ��������
		}
	}
}

// �Ǐ��T��
bool Generator::localSearch(int& score)
{
	// �q���g�̈ʒu�𒲂ׂ�
	vector<int> cluePosList;
	for (int i = 0; i < GRID_SIZE; ++i)
		if (puzzle[i] != 0) cluePosList.push_back(i);
	const int clueCount = cluePosList.size();

	// �T�������V���b�t��
	shuffle(cluePosList.begin(), cluePosList.end(), mt);
	shuffle(posList.begin(), posList.end(), mt);
	shuffle(numList.begin(), numList.end(), mt);

	Solver baseSolver, copySolver;

	for (const int old_p : cluePosList) {
		// �q���g��1����
		const int old_n = puzzle[old_p];
		puzzle[old_p] = 0;
		baseSolver.setPuzzle(puzzle);
		puzzle[old_p] = old_n;

		// �q���g��1�����
		for (const int new_p : posList) {
			if (baseSolver.getAnswer(new_p) != 0) continue;
			for (const int new_n : numList) {
				if (baseSolver.getCandidate(new_p, new_n) == 0) continue;
				copySolver = baseSolver;
				copySolver.putNumber(new_p, new_n);
				if (copySolver.getError() == true) continue;
				const int tmpScore = copySolver.getScore();

				// �]���l���ǂ��Ȃ�X�V����
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
	// �����_���ɖ��𐶐�����
	initPuzzle(clueCount);

	// �ŏ��̃X�R�A���v�Z����
	Solver solver;
	solver.setPuzzle(puzzle);
	int score = solver.getScore();

	// �]�����ǂ��Ȃ�悤�Ƀq���g�����ւ���
	do {
		if (score == 0) return true; // ��������
	} while (localSearch(score));

	return false; // �������s
}
