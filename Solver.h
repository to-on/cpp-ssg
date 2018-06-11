#ifndef __SOLVER__
#define __SOLVER__

#include <climits>
#include <cstring>
#include <string>
#include <vector>

// �u���b�N�̃T�C�Y
#define BOX_X 3 // 3
#define BOX_Y 3 // 3

#define BOARD_SIZE (BOX_X * BOX_Y)						// 9
#define GRID_SIZE (BOARD_SIZE * BOARD_SIZE)				// 81

class Solver
{
	// �u���b�N,�s,��̔ԍ� �� �}�X
	static std::vector< std::vector<int> > house2cells;		// house2cells[idx] <- cells (box,row,column)
	// �}�X �� �u���b�N,�s,��̔ԍ�
	static std::vector< std::vector<int> > cell2houses;		// cell2houses[cell] <- idxs
	// �}�X �� �u���b�N,�s,����̈ʒu
	static std::vector< std::vector<int> > cell2housePoss;	// cell2housePos[cell] <- positions
	// �}�X �� ���͂̃}�X
	static std::vector< std::vector<int> > cell2cells;		// cell2cells[cell] <- cells(box + row + column - cell)
	// ���� �� ���̐���
	static int num2othernums[BOARD_SIZE][BOARD_SIZE - 1];	// num2othernums[n] <- {0,1,2,...,BOARD_SIZE-1} - {n}

	static void init_house2cells();
	static void init_cell2houses();
	static void init_cell2cells();
	static void init_num2otherNums();

public:
	// ���̏�����
	static void initConstraint();
	
private:
	// ��
	//   �����l : 0,0,0,�c (����GRID_SIZE)
	int answer[GRID_SIZE];

	// �󂫃}�X�̐�
	//   �����l : GRID_SIZE
	int spaceCount;

	// ���
	//   �����l : (1 << BOARD_SIZE) - 1
	int candidates[GRID_SIZE];							// [p] <- bit
	std::vector< std::vector<int> > houseCandidates;	// [h][n] <- bit

	// �𖳂��𔻒肷��t���O
	//   �����l : false
	//            true (�𖳂�)
	bool errorFlag;

public:
	void init();
	Solver() { init(); }

	// �R�s�[
	Solver& operator=(const Solver& obj) {
		memcpy(answer, obj.answer, sizeof(obj.answer));
		spaceCount = obj.spaceCount;
		memcpy(candidates, obj.candidates, sizeof(obj.candidates));
		houseCandidates = obj.houseCandidates;
		errorFlag = obj.errorFlag;
		return *this;
	}

	// �����
	//    puz : ����GRID_SIZE, 1-9,a-z�ȊO�̕�����l��0�Ƃ���
	//     �� : 4......1..2...6..8....186341.......7....63...5.....2...8......1...5.2.....4...95.
	//     �� : 083000000500000027090000000000230050908010000000500480200040800000000061000700500
	void setPuzzle(const std::string& puz);
	void setPuzzle(const int puz[GRID_SIZE]);

	// ���̗L���𒲂ׂ�
	//   p : ���ׂ�ʒu (0 - (GRID_SIZE-1))
	//   n : ���ׂ鐔�� (0 - (BOARD_SIZE-1))
	//   return : ���̗L�� (1-:�L, 0:��)
	int getCandidate(const int p, const int n)const { return (candidates[p] & (1 << n)); }

	// �����폜����
	//   p : �폜����ʒu (0 - (GRID_SIZE-1))
	//   n : �폜���鐔�� (0 - (BOARD_SIZE-1))
	void deleteCandidate(const int p, const int n);

	// �q���g�̒ǉ�
	//   p : ������ǉ�����ʒu (0 - (GRID_SIZE-1))
	//   n : �ǉ����鐔�� (0 - (BOARD_SIZE-1))
	void putNumber(const int p, const int n);

	// �󂫃}�X�̐��𒲂ׂ�
	//   return : �󂫃}�X�� (0 - GRID_SIZE)
	int getSpaceCount()const { return spaceCount; }

	// �e�}�X�̌�␔�̕����a
	//   return : �����a (GRID_SIZE - BOARD_SIZE*BOARD_SIZE*GRID_SIZE)
	int getCandidateSquareSum()const;

	// ���̕]���l���v�Z����
	//   return : �]���l (0-)
	int getScore()const {
		int score = getCandidateSquareSum() - GRID_SIZE;
		if (errorFlag == true) score += BOARD_SIZE * BOARD_SIZE * GRID_SIZE;
		return score;
	}

	// ��
	const int* const getAnswer()const { return answer; }
	int getAnswer(const int p)const { return answer[p]; }

	// �𖳂����H
	//   return : �𖳂��� (true:�𖳂�, false:�s��)
	bool getError()const { return errorFlag; }
};

#endif