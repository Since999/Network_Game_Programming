#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

#define SERVERPORT 9000
#define BUFSIZE 1024

#define GAME_READY 11			// 접속한 클라이언트 수 < 3
#define GAME_RUNNING 12			// INGAME 상태 (죽은 클라이언트 수 < 3)
#define GAME_SET 13				// 죽은 클라이언트 수 == 3

#define MOVE_NONE 20			// 키 입력이 없는 상태 (로그인시 사용)
#define MOVE_UP 21				// W키 입력
#define MOVE_DOWN 22			// S키 입력
#define MOVE_LEFT 23			// A키 입력
#define MOVE_RIGHT 24			// D키 입력

#define PLAYER_SPEED 10			// 플레이어 속도

CRITICAL_SECTION cs;
int joinClient = 0;
int gameState = GAME_READY;





struct Position {
	float x, y;
};


struct EXHP {
	Position pos;                  // 위치
	bool isAlived{ false };          // 생사여부
};

class Player {
public:
	Position pos{ 0,0 };				// 위치
	char playerID[10];			// 로그인시 사용할 ID
	bool isAlived{ true };				// 생사여부
	int hp;						// 생사를 결정하는 HP

	EXHP exhpList[3];

	int score{ 0 };					// Enemy를 잡으면 1 상승 (승패 결정)
	int rank{ 3 };					// 최종 순위
};

struct Enemy {
public:
	Position pos;				// 위치
	bool isAlived{ true };				// 생사여부
};

struct Item {
public:
	Position pos;				// 위치
	bool isAlived{ true };				// 생사여부
};

struct Wall {
	Position pos;
};




/*struct sc_send_struct1 {
	int gameState;
};*/

struct sc_send_struct2 {
	int gameState;
	Player players[3];
	int enemy[3];
	int item[3];
	//EXHP exhpList[3];
	int clientIndex;			// 해당 클라이언트의 인덱스

};





/*struct sc_send_struct {
	char size=596U;
	char type;
	Player players[3];			// 플레이어들의 리스트
	int gameState;				// GAME_READY/GAME_RUNNING/GAME_SET
	Enemy enemyList[35];
	Item itemList[3];
	EXHP exhpList[3];
};*/

struct sc_recv_struct {
	char size=20U;
	char type;
	int keyInputDirection;		// 클라이언트에서의 키입력 정보
	char playerID[10];			// 해당 클라이언트의 ID
};

float deltaTime();						// deltaTime 반환

void sendGameStart();					// GAME_RUNNING 전환을 클라이언트에게 전송

void SendData(sc_send_struct2* s_data);	// sc_send_struct 구조체 전송
void RecvData(sc_recv_struct* r_data);	// sc_recv_struct 구조체 수신

void MakeRank();						// GAME_SET에서 세 클라이언트의 순위 결정

void MovePlayer(int key,Player& p, int clientIndex);				// 플레이어의 위치 정보 갱신
void UpdatePlayer(Player p);			// 플레이어의 정보 갱신

void CheckPlayerByWallCollision(int key ,Player& p);		// 플레이어와 벽의 충돌 체크
void CheckPlayerByPlayerCollision(int key, Player& p,int clientIndex);	// 플레이어간의 충돌 체크
void CheckPlayerByEnemyCollision(Player& p);		// 플레이어와 Enemy의 충돌 체크
void CheckPlayerByItemCollision(Player& p);		// 플레이어와 Item의 충돌 체크

void DeleteEnemy();						// Enemy 객체의 isAlived = false;
										// CheckPlayerByEnemyCollision()에서 호출
void DeleteItem();						// Item 객체의 isAlived = false;
										// CheckPlayerByItemCollision()에서 호출

bool isPlayerAlived();					// 플레이어의 HP 검사를 통해 생사 판별

bool isGameOver();						// 종료 조건 처리 (죽은 플레이어 수 == 3)




