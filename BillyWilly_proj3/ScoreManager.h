#include "Rank.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class ScoreManager {
public:

	// for singleton architecture
	static ScoreManager* getInstance(){
		if (instance == NULL){ instance = new ScoreManager();  }
		return instance;
	}
	
	// @desc : ���� ó�� ����Ǿ�� �� �Լ�. ��ŷ�� �ҷ��´�.
	// @return : ��ŷ�� ���������� ����Ǿ����� true. �ƴ� ��� false
	bool loadRank();
	// @desc : ��ŷ�� ����ϴ� �Լ�
	// @return : ��ŷ�� ��ϵǾ�����, ��ϵ� rank�� ��ȯ�Ѵ�.
	int addRank(string name, int mode, int score);
	
	// @desc : ��忡 ���� ��ŷ �����͸� �����ִ� �Լ�.
	// @args : ��ŷ�� ���� ���� ��带 int ���� ��ȯ
	// @return : vector<Rank> refrence type
	vector<Rank>& getRankData(int mode);

	

private:
	ScoreManager() {
		for (int i = 0; i < sizeof(rankData)/sizeof(*rankData); i++){
			rankData[i] = vector<Rank>();
		}
	}
	~ScoreManager() {}
	static ScoreManager* instance;
	vector<Rank> rankData[2];
};