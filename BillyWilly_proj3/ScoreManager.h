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
	
	// @desc : 가장 처음 실행되어야 할 함수. 랭킹을 불러온다.
	// @return : 랭킹이 성공적으로 저장되었으면 true. 아닐 경우 false
	bool loadRank();
	// @desc : 랭킹에 등록하는 함수
	// @return : 랭킹에 등록되었으면, 등록된 rank를 반환한다.
	int addRank(string name, int mode, int score);
	
	// @desc : 모드에 따라 랭킹 데이터를 보여주는 함수.
	// @args : 랭킹을 보고 싶은 모드를 int 형을 변환
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