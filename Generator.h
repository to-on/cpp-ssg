#ifndef __GENERATOR_LS__
#define __GENERATOR_LS__

#include <random>
#include "Solver.h"

class Generator
{
	std::mt19937 mt;

	// ’Tõ‡
	std::vector<int> posList; // posList <- {0, 1, 2, ... , GRID_SIZE-1}
	std::vector<int> numList; // numList <- {0, 1, 2, ... , BOARD_SIZE-1}

	// ¶¬’†‚Ì–â‘è
	int puzzle[GRID_SIZE];

	void init();

	// –â‘è(puzzle)‚ğ‰Šú‰»‚·‚é
	//   clueCount : ƒqƒ“ƒg”
	void initPuzzle(const int clueCount);

	// ’Tõ
	//   score : puzzle‚Ì•]‰¿’l
	//   return : •]‰¿‚ª—Ç‚­‚È‚Á‚½‚© (true:—Ç‚­‚È‚Á‚½, false:›Æ‚Á‚½)
	bool localSearch(int& score);

public:
	Generator() { init(); }

	// ƒiƒ“ƒvƒŒ‚ğ¶¬‚·‚é
	//   clueCount : ƒqƒ“ƒg”
	//   return : ¶¬‚Å‚«‚½‚© (true:¬Œ÷, false:¸”s)
	bool generatePuzzle(const int clueCount);

	// ¶¬‚µ‚½–â‘è
	const int* const getPuzzle()const { return puzzle; }
};

#endif