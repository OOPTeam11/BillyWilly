/*

Class : Game
Work : 게임 데이터가 담겨있는 클래스

# 게임모드의 종류
1. 모드1 : 정해진 시간동안 최대한 많은 점수를 내는 모드(1인용)
2. 모드2 : 정해진 점수를 가장 빠르게 도달하는 모드(1인용)
4. 모드3 : 2인용 모드, 2인이서 4구를 제한없이 점수만 내며 즐기는 모드
3. 연습 모드 : 시간 상관 없이 즐기는 모드. 점수는 매겨진다.(1인용)

*/

#include <time.h>

#define MAX_TIME 180
#define MAX_SCORE 1000

#define MODE_1 0
#define MODE_2 1
#define MODE_3 2
#define MODE_PRACTICE 3

#define PLAYER1 0
#define PLAYER2 1

class Game {
public:

	// constructor
	Game(){ init(); };
	// destructor
	~Game(){};

	// @desc : 게임 관련 변수를 초기화한다.
	void init();

	// ===========================================================
	//                     게임 모드 관련 함수
	// ===========================================================
	/**
	@desc : 게임의 모드를 설정하는 함수
	*/
	void setMode(int Mode);
	/**
	@desc : 게임의 모드를 가져오는 함수
	@return : 현제 게임 모드
	*/
	int getMode();

	// ===========================================================
	//                        차례 관련 함수
	// ===========================================================
	/**
	@desc : 차례를 임의로 설정
	@return : 모드와 맞지 않게 설정할 시 false.
	*/
	bool setTurn(int Player);
	/**
	@desc : 다음 차례로 넘김
	@return : 다음 차례
	*/
	int nextTurn();
	/**
	@desc : 차례를 가져오는 함수
	@return : 현제 차례
	*/
	int getTurn();

	// ===========================================================
	//                     플레이어 점수 관련 
	// ===========================================================
	/**
	@desc : Player 의 점수를 가져오는 함수
	@return : Player 의 점수
	*/
	int getPlayerScore(int Player);
	/**
	@desc : Player 의 점수를 Score 로 설정하는 함수
	*/
	void setPlayerScore(int Player, int Score);
	void addPlayerScore(int Player, int Score);

	int getPlayerTime(int Player);
	void addPlayerTime(int Player, int Time);
	void setPlayerTime(int Player, int Time);

	// ===========================================================
	//                     이벤트 관련 
	// ===========================================================
	/**
	@desc : 게임이 시작되었을 때 호출 되어야 하는 함수
	*/
	void onGameStart();

	/**
	@desc : 플레이어의 턴이 종료되었을 때 호출되어야하는 함수
	        빨강공 index : 0, 1
			노랑공 index : 2
			하얀공 index : 3
	@args : currentBallIndex - 턴이 종료되기 전에 조종한 공의 index
	        CollidedBalls - hasCollided 값을 가져옴
	*/
	void onTurnEnd(int currentBallIndex, bool hasCollided[4]);

private:

	// 현재 게임이 어떤 모드인지 저장되어 있는 변수
	int GameMode;
	// 누구 차례인지 저장되어 있는 변수
	int GameTurn;
	// Player1,2 의 점수
	int GameScore[2];
	// Player1,2 의 게임 경과 시간
	int GameTime[2];
	clock_t StartTime;
	int MaxTime = MAX_TIME;
	int MaxScore = MAX_SCORE;
};