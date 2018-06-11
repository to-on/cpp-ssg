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
	// �t�@�C���ǂݍ���
	ifstream ifs(fileName);
	if (ifs.fail() == true) {
		cout << "No such file: " << fileName << endl;
		return -1;
	}
	
	Solver solver;

	int puzzleCount = 0;		// ��萔
	int solvedCount = 0;		// ��������萔
	int noSolutionCount = 0;	// �𖳂��̖�萔
	int unknownCount = 0;		// ��������Ă����؂ł͉������Ƃ��ł��Ȃ�������萔

	auto start = chrono::system_clock::now();

	// 1�s������
	string str;
	while (getline(ifs, str)) {
		solver.setPuzzle(str);
		if (solver.getError()) {
			++noSolutionCount; // �𖳂�
		}
		else{
			if (solver.getSpaceCount() == 0)
				++solvedCount; // �B���
			else
				++unknownCount; // �s��
		}
		++puzzleCount;
	}

	chrono::duration<double> sec = chrono::system_clock::now() - start;

	// ����
	cout << "Puzzle Count: " << puzzleCount << endl;
	cout << "Solved Count: " << solvedCount << endl;
	cout << "No Solution Count: " << noSolutionCount << endl;
	cout << "Unknown Count: " << unknownCount << endl;
	cout << "Time: " << sec.count() << "sec" << endl;

	return 0;
}
int generate(const int clueCount)
{
	// �ŏ��q���g���ȏォ�H
	int min = GRID_SIZE / 4 - (BOX_X - 1) * (BOX_Y - 1);
	if (BOX_X == 2 || BOX_Y == 2 || (BOX_X == 3 && BOX_Y == 3)) ++min;
	if (clueCount < min || GRID_SIZE < clueCount) return -1;

	auto start = chrono::system_clock::now();
	int trialCount = 0;

	// ����
	Generator generator;
	bool flag = false;
	chrono::duration<double> sec;
	while (flag == false) {
		++trialCount;
		flag = generator.generatePuzzle(clueCount);
		sec = chrono::system_clock::now() - start;
		cout << trialCount << ", " << sec.count() << "sec" << endl;
	}

	// �o��
	cout << puzzleToString(generator.getPuzzle()) << endl;

	return 0;
}

int main(int argc, char *argv[])
{
	// ������
	Solver::initConstraint();

	// �R�}���h
	for (int i = 0; i < argc; ++i) {
		if (strcmp(argv[i], "-solve") == 0 && i + 1 < argc)
			return solve(argv[i + 1]);
		if (strcmp(argv[i], "-generate") == 0 && i + 1 < argc)
			return generate(atoi(argv[i + 1]));
	}

	// �w���v
	cout << argv[0];
	cout << " [-solve filename]";
	cout << " [-generate clues]" << endl;

	return 0;
}
