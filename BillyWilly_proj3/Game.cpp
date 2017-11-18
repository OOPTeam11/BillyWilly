#include "Game.h"
/*

Class : Game
Work : 게임 데이터가 담겨있는 클래스

# 게임모드의 종류
1. 모드1 : 정해진 시간동안 최대한 많은 점수를 내는 모드(1인용)
2. 모드2 : 정해진 점수를 가장 빠르게 도달하는 모드(1인용)
3. 모드3 : 2인용 모드, 2인이서 4구를 제한없이 점수만 내며 즐기는 모드
4. 연습 모드 : 시간 상관 없이 즐기는 모드. 점수는 매겨진다.(2인용)

*/

Game::Game(){
	init();
}

void Game::init(){
	GameMode = 3;
	GameTurn = PLAYER1;
	GameScore[PLAYER1] = 0;
	GameScore[PLAYER2] = 0;
	GameTime[PLAYER1] = 0;
	GameTime[PLAYER2] = 0;
	StartTime = 0;
}

// ===========================================================
//                     게임 모드 관련 함수
// ===========================================================

void Game::setMode(int Mode){
	this->GameMode = Mode;
}

int Game::getMode(){
	return this->GameMode;
}

// ===========================================================
//                        차례 관련 함수
// ===========================================================

bool Game::setTurn(int Player){
	if (this->GameMode == MODE_1 || this->GameMode == MODE_2 || this->GameMode == MODE_PRACTICE){ // if one person mode
		if (Player == PLAYER1){
			this->GameTurn = PLAYER1;
			return true;
		}
		else {
			return false;
		}

	}
	else if (this->GameMode == MODE_3){ // if two person mode
		if (Player != PLAYER1 && Player != PLAYER2){
			return false;
		}
		else {
			this->GameTurn = Player;
			return true;
		}
	}
}

int Game::nextTurn(){
	if (this->GameMode == MODE_1 || this->GameMode == MODE_2 || this->GameMode == MODE_PRACTICE){ // if one person mode
		this->GameTurn = PLAYER1;
	}
	else if (this->GameMode == MODE_3){ // if two person mode
		if (this->GameTurn == PLAYER1) this->GameTurn = PLAYER2;
		else if (this->GameTurn == PLAYER2) this->GameTurn = PLAYER1;
	}
	return this->GameTurn;
}

int Game::getTurn(){
	return this->GameTurn;
}

// ===========================================================
//                     플레이어 점수 관련 
// ===========================================================

int Game::getPlayerScore(int Player){
	if (Player != PLAYER1 && Player != PLAYER2){
		return -1;
	}
	else {
		return this->GameScore[Player];
	}
}

void Game::setPlayerScore(int Player, int Score){
	if (Player != PLAYER1 && Player != PLAYER2){
		return;
	}
	else {
		this->GameScore[Player] = Score;
	}
}

void Game::addPlayerScore(int Player, int Score){
	if (Player != PLAYER1 && Player != PLAYER2){
		return;
	}
	else {
		this->GameScore[Player] += Score;
	}
}


int Game::getPlayerTime(int Player){
	if (Player != PLAYER1 && Player != PLAYER2){
		return -1;
	}
	else {
		return this->GameTime[Player];
	}
}
void Game::addPlayerTime(int Player, int Time){
	if (Player != PLAYER1 && Player != PLAYER2){
		return;
	}
	else {
		this->GameTime[Player] += Time;
	}
}
void Game::setPlayerTime(int Player, int Time){
	if (Player != PLAYER1 && Player != PLAYER2){
		return;
	}
	else {
		this->GameTime[Player] = Time;
	}
}

// ===========================================================
//                     이벤트 관련 
// ===========================================================

void Game::registerEndCallBack(void function(int)){
	this->endCallBack = function;
}

void Game::onGameStart(){

	// 시간 측정 시작
	if (this->getMode() == MODE_1){
		this->StartTime = clock();
	}
}

void Game::onTurnEnd(int currentBallIndex, bool hasCollided[4], bool& isTurnChange){
	
	// 턴을 바꿀지 말지 결정하는 변수
	isTurnChange = true;
	//
	// 점수 관리
	// 
	if (this->getTurn() == PLAYER1){ // Player1 차례가 끝났을 경우 (하얀공)
		if (hasCollided[2] == true){ // Player2 (노란공) 을 쳤을 경우
			this->addPlayerScore(PLAYER1, -10); // 10점을 감점한다.
		}
		else { // 상대방을 치지 않았을 경우

			if (hasCollided[0] == true && hasCollided[1] == true){ // 빨간공을 다 쳤을 경우
				this->addPlayerScore(PLAYER1, 10); // 10첨 추가
				isTurnChange = false;
			}
			else if (hasCollided[0] == false && hasCollided[1] == false){ // 두 공다 치지 못했을 경우
				this->addPlayerScore(PLAYER1, -10);
			}
			else {} // 아무것도 하지 않는다.

		}
	}
	else { // Player2 차례가 끝났을 경우 (노란공)
		if (hasCollided[2] == true){ // Player1 (하얀공) 을 쳤을 경우
			this->addPlayerScore(PLAYER2, -10); // 10점을 감점한다.
		}
		else {  // 상대방을 치지 않았을 경우

			if (hasCollided[0] == true && hasCollided[1] == true){ // 빨간공을 다 쳤을 경우
				this->addPlayerScore(PLAYER2, 10); // 10첨 추가
				isTurnChange = false;
			}
			else if (hasCollided[0] == false && hasCollided[1] == false){ // 두 공다 치지 못했을 경우
				this->addPlayerScore(PLAYER2, -10); // 10점 추가
			}
			else {} // 아무것도 하지 않는다.

		}
	}

	//
	// 게임 엔딩 관리
	//

	int mode = this->getMode();
	if (mode == MODE_1){ // 모드 1인 경우
		clock_t current = clock();
		GameTime[GameTurn] += current - this->StartTime;
		this->StartTime = clock();
		if (GameTime[GameTurn] >= MAX_TIME){ // 시간을 넘기면 엔딩
			this->endCallBack(GameTurn);
		}
	}
	else if (mode == MODE_2){
		clock_t current = clock();
		GameTime[GameTurn] += current - this->StartTime;
		this->StartTime = clock();
		if (this->getPlayerScore(PLAYER1) >= MAX_SCORE) { // 점수를 넘기면 엔딩
			this->endCallBack(GameTurn);
		}
	}
	else {
		// 나머지 모드는 엔딩이 없다.
	}

	//
	// 모드에 따른 공 바꾸는 이벤트
	//
	switch (this->getMode()){
	case MODE_1:
	case MODE_2:
	case MODE_PRACTICE:
		isTurnChange = false;
		break;
	}
}