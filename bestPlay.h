#include "virtualLego.h"
#include <iostream>

using namespace std;

class bestPlay{
public:
	bestPlay();
	~bestPlay();
	//void saveLastStatus(CSphere	g_sphere[4], CWall g_legowall[4], CWall g_legoPlane, CSphere g_target_blueball, CLight	g_light); // 스페이스바가 눌렸을 때 출발할 때의 4개의 공 상태 저장.
	bool threeCushion();
	void showReplay();
	int cusionCount;
private:

};