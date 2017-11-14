/*

Class : Game
Work : ���� �����Ͱ� ����ִ� Ŭ����

# ���Ӹ���� ����
1. ���1 : ������ �ð����� �ִ��� ���� ������ ���� ���(1�ο�)
2. ���2 : ������ ������ ���� ������ �����ϴ� ���(1�ο�)
4. ���3 : 2�ο� ���, 2���̼� 4���� ���Ѿ��� ������ ���� ���� ���
3. ���� ��� : �ð� ��� ���� ���� ���. ������ �Ű�����.(1�ο�)

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

	// @desc : ���� ���� ������ �ʱ�ȭ�Ѵ�.
	void init();

	// ===========================================================
	//                     ���� ��� ���� �Լ�
	// ===========================================================
	/**
	@desc : ������ ��带 �����ϴ� �Լ�
	*/
	void setMode(int Mode);
	/**
	@desc : ������ ��带 �������� �Լ�
	@return : ���� ���� ���
	*/
	int getMode();

	// ===========================================================
	//                        ���� ���� �Լ�
	// ===========================================================
	/**
	@desc : ���ʸ� ���Ƿ� ����
	@return : ���� ���� �ʰ� ������ �� false.
	*/
	bool setTurn(int Player);
	/**
	@desc : ���� ���ʷ� �ѱ�
	@return : ���� ����
	*/
	int nextTurn();
	/**
	@desc : ���ʸ� �������� �Լ�
	@return : ���� ����
	*/
	int getTurn();

	// ===========================================================
	//                     �÷��̾� ���� ���� 
	// ===========================================================
	/**
	@desc : Player �� ������ �������� �Լ�
	@return : Player �� ����
	*/
	int getPlayerScore(int Player);
	/**
	@desc : Player �� ������ Score �� �����ϴ� �Լ�
	*/
	void setPlayerScore(int Player, int Score);
	void addPlayerScore(int Player, int Score);

	int getPlayerTime(int Player);
	void addPlayerTime(int Player, int Time);
	void setPlayerTime(int Player, int Time);

	// ===========================================================
	//                     �̺�Ʈ ���� 
	// ===========================================================
	/**
	@desc : ������ ���۵Ǿ��� �� ȣ�� �Ǿ�� �ϴ� �Լ�
	*/
	void onGameStart();

	/**
	@desc : �÷��̾��� ���� ����Ǿ��� �� ȣ��Ǿ���ϴ� �Լ�
	        ������ index : 0, 1
			����� index : 2
			�Ͼ�� index : 3
	@args : currentBallIndex - ���� ����Ǳ� ���� ������ ���� index
	        CollidedBalls - hasCollided ���� ������
	*/
	void onTurnEnd(int currentBallIndex, bool hasCollided[4]);

private:

	// ���� ������ � ������� ����Ǿ� �ִ� ����
	int GameMode;
	// ���� �������� ����Ǿ� �ִ� ����
	int GameTurn;
	// Player1,2 �� ����
	int GameScore[2];
	// Player1,2 �� ���� ��� �ð�
	int GameTime[2];
	clock_t StartTime;
	int MaxTime = MAX_TIME;
	int MaxScore = MAX_SCORE;
};