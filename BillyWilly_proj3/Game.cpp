#include "Game.h"
/*

Class : Game
Work : ���� �����Ͱ� ����ִ� Ŭ����

# ���Ӹ���� ����
1. ���1 : ������ �ð����� �ִ��� ���� ������ ���� ���(1�ο�)
2. ���2 : ������ ������ ���� ������ �����ϴ� ���(1�ο�)
3. ���3 : 2�ο� ���, 2���̼� 4���� ���Ѿ��� ������ ���� ���� ���
4. ���� ��� : �ð� ��� ���� ���� ���. ������ �Ű�����.(2�ο�)

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
//                     ���� ��� ���� �Լ�
// ===========================================================

void Game::setMode(int Mode){
	this->GameMode = Mode;
}

int Game::getMode(){
	return this->GameMode;
}

// ===========================================================
//                        ���� ���� �Լ�
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
//                     �÷��̾� ���� ���� 
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
//                     �̺�Ʈ ���� 
// ===========================================================

void Game::registerEndCallBack(void function(int)){
	this->endCallBack = function;
}

void Game::onGameStart(){

	// �ð� ���� ����
	if (this->getMode() == MODE_1){
		this->StartTime = clock();
	}
}

void Game::onTurnEnd(int currentBallIndex, bool hasCollided[4], bool& isTurnChange){
	
	// ���� �ٲ��� ���� �����ϴ� ����
	isTurnChange = true;
	//
	// ���� ����
	// 
	if (this->getTurn() == PLAYER1){ // Player1 ���ʰ� ������ ��� (�Ͼ��)
		if (hasCollided[2] == true){ // Player2 (�����) �� ���� ���
			this->addPlayerScore(PLAYER1, -10); // 10���� �����Ѵ�.
		}
		else { // ������ ġ�� �ʾ��� ���

			if (hasCollided[0] == true && hasCollided[1] == true){ // �������� �� ���� ���
				this->addPlayerScore(PLAYER1, 10); // 10÷ �߰�
				isTurnChange = false;
			}
			else if (hasCollided[0] == false && hasCollided[1] == false){ // �� ���� ġ�� ������ ���
				this->addPlayerScore(PLAYER1, -10);
			}
			else {} // �ƹ��͵� ���� �ʴ´�.

		}
	}
	else { // Player2 ���ʰ� ������ ��� (�����)
		if (hasCollided[2] == true){ // Player1 (�Ͼ��) �� ���� ���
			this->addPlayerScore(PLAYER2, -10); // 10���� �����Ѵ�.
		}
		else {  // ������ ġ�� �ʾ��� ���

			if (hasCollided[0] == true && hasCollided[1] == true){ // �������� �� ���� ���
				this->addPlayerScore(PLAYER2, 10); // 10÷ �߰�
				isTurnChange = false;
			}
			else if (hasCollided[0] == false && hasCollided[1] == false){ // �� ���� ġ�� ������ ���
				this->addPlayerScore(PLAYER2, -10); // 10�� �߰�
			}
			else {} // �ƹ��͵� ���� �ʴ´�.

		}
	}

	//
	// ���� ���� ����
	//

	int mode = this->getMode();
	if (mode == MODE_1){ // ��� 1�� ���
		clock_t current = clock();
		GameTime[GameTurn] += current - this->StartTime;
		this->StartTime = clock();
		if (GameTime[GameTurn] >= MAX_TIME){ // �ð��� �ѱ�� ����
			this->endCallBack(GameTurn);
		}
	}
	else if (mode == MODE_2){
		clock_t current = clock();
		GameTime[GameTurn] += current - this->StartTime;
		this->StartTime = clock();
		if (this->getPlayerScore(PLAYER1) >= MAX_SCORE) { // ������ �ѱ�� ����
			this->endCallBack(GameTurn);
		}
	}
	else {
		// ������ ���� ������ ����.
	}

	//
	// ��忡 ���� �� �ٲٴ� �̺�Ʈ
	//
	switch (this->getMode()){
	case MODE_1:
	case MODE_2:
	case MODE_PRACTICE:
		isTurnChange = false;
		break;
	}
}