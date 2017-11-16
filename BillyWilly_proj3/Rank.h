#include <string>

using namespace std;

class Rank{
public:

	Rank(string name, int mode, int score){
		this->name = name;
		this->mode = mode;
		this->score = score;
	};

	string name;
	int mode;
	int score;
	//for sort. define operator.
	friend bool operator>(const Rank& a, const Rank& b){
		return a.score > b.score;
	}
	friend bool operator<(const Rank& a, const Rank& b){
		return a.score < b.score;
	}
};