#ifndef __GENERATOR_LS__
#define __GENERATOR_LS__

#include <random>
#include "Solver.h"

class Generator
{
	std::mt19937 mt;

	// �T����
	std::vector<int> posList; // posList <- {0, 1, 2, ... , GRID_SIZE-1}
	std::vector<int> numList; // numList <- {0, 1, 2, ... , BOARD_SIZE-1}

	// �������̖��
	int puzzle[GRID_SIZE];

	void init();

	// ���(puzzle)������������
	//   clueCount : �q���g��
	void initPuzzle(const int clueCount);

	// �T��
	//   score : puzzle�̕]���l
	//   return : �]�����ǂ��Ȃ����� (true:�ǂ��Ȃ���, false:�Ƃ���)
	bool localSearch(int& score);

public:
	Generator() { init(); }

	// �i���v���𐶐�����
	//   clueCount : �q���g��
	//   return : �����ł����� (true:����, false:���s)
	bool generatePuzzle(const int clueCount);

	// �����������
	const int* const getPuzzle()const { return puzzle; }
};

#endif