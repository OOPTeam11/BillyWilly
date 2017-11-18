#pragma once
#include "ScoreManager.h"
#include "virtualLego.h"
using namespace std;

ScoreManager* ScoreManager::instance = nullptr;

Rank parseRank(string line){
	int     nCutPos;
	int     nIndex = 0;
	vector<string> strResult;

	while ((nCutPos = line.find_first_of(":")) != line.npos)
	{
		if (nCutPos > 0)
		{
			strResult.push_back(line.substr(0, nCutPos));
		}
		line = line.substr(nCutPos + 1);
	}

	if (line.length() > 0)
	{
		strResult.push_back(line.substr(0, nCutPos));
	}

	if (strResult.size() == 3){
		return Rank(strResult.at(0), stoi(strResult.at(1)), stoi(strResult.at(2)));
	}
	else {
		return Rank("", -1, -1);
	}
}

bool CmpMode1(const Rank& a, const Rank& b){
	return a.score > b.score;
}
bool CmpMode2(const Rank& a, const Rank& b){
	return a.score < b.score;
}

bool ScoreManager::saveRank(){
	ofstream rankFile("rank.txt");
	if (rankFile.is_open() == true){
		for (int mode = 0; mode < 2; mode++){
			for (auto rank = rankData[mode].begin(); rank < rankData[mode].end(); rank++){
				string str = rank->name + ":" + std::to_string(rank->mode) + ":" + std::to_string(rank->score) + "\n";
				OutputDebugString(str.c_str());
				rankFile << str;
			}
			rankFile.close();
		}
		return true;
	}
	else {
		return false;
	}
}

bool ScoreManager::loadRank(){
	ifstream rankFile("rank.txt");
	if (rankFile.is_open()){ // 파일이 있을 경우
		string line;
		while (getline(rankFile, line)){
			Rank rank = parseRank(line);
			if (rank.mode == -1 && rank.score == -1){
				// parsing err - line skip
			}
			else {
				rankData[rank.mode].push_back(rank);
			}
		}
		rankFile.close();
	}
	else { // 파일이 없을 경우
		ofstream file("rank.txt");
		if (file.is_open() == true) file.close(); // 파일 생성.
		else return false; // 파일 생성 안되므로 return false.
	}

	// 순위 정렬
	sort(rankData[0].begin(), rankData[0].end(), CmpMode1);
	sort(rankData[1].begin(), rankData[1].end(), CmpMode2);
	return true;
}

int ScoreManager::addRank(string name, int mode, int score){
	// TODO: 중복 문제 해결
	Rank rank(name, mode, score);
	if (mode == MODE_1){
		int index = 0;
		if (rankData[mode].size() == 0){
			rankData[mode].insert(rankData[mode].begin() + index, rank);
			return index;
		}
		for (auto i = rankData[mode].begin(); i < rankData[mode].end(); i++){
			if (i->score < score){
				vector<Rank>::iterator it;
				it = rankData[mode].begin();
				rankData[mode].insert(it + index, rank);
				break;
			}
			index++;
		}
		return index;
	}
	else if (mode == MODE_2){
		int index = 0;
		if (rankData[mode].size() == 0){
			rankData[mode].insert(rankData[mode].begin() + index, rank);
			return index;
		}
		for (auto i = rankData[mode].begin(); i < rankData[mode].end(); i++){
			if (i->score < score){
				vector<Rank>::iterator it;
				it = rankData[mode].begin();
				rankData[mode].insert(it + index, rank);
				break;
			}
			index++;
		}
		return index;
	}
	return 1;
}

vector<Rank>& ScoreManager::getRankData(int mode){
	return rankData[mode];
}