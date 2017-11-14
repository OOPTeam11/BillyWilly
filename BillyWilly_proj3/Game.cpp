#include "Game.h"
/*

Class : Game
Work : ���� �����Ͱ� ����ִ� Ŭ����

# ���Ӹ���� ����
1. ���1 : ������ �ð����� �ִ��� ���� ������ ���� ���(1�ο�)
2. ���2 : ������ ������ ���� ������ �����ϴ� ���(1�ο�)
4. ���3 : 2�ο� ���, 2���̼� 4���� ���Ѿ��� ������ ���� ���� ���
3. ���� ��� : �ð� ��� ���� ���� ���. ������ �Ű�����.(2�ο�)

*/

void Game::init(){
	GameMode = 0;
	GameTurn = PLAYER1;
	GameScore[PLAYER1] = 0;
	GameScore[PLAYER2] = 0;
	GameTime[PLAYER1] = 0;
	GameTime[PLAYER2] = 0;
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
void Game::onTurnEnd(){
	
}