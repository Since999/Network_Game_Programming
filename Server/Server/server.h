#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#pragma comment(lib, "winmm.lib")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <iostream>
#include <vector>
using namespace std;

#define SERVERPORT 9000
#define BUFSIZE    4096

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

#define PLAYER_MAX 3
#define FPS 30

CRITICAL_SECTION cs;
int gameState = GAME_READY;

DWORD curTime;
DWORD lastTime = GetTickCount();

#pragma pack(1)
struct Position {
	float x, y;
};


struct EXHP {
public:	
	Position pos;						// 위치
	bool isAlived{ false };				// 생사여부
};

class Player {
public:
	Position pos{ 0,0 };				// 위치
	char playerID[10];					// 로그인시 사용할 ID
	bool isAlived{ true };				// 생사여부
	int exhpList=0;
	int hp{ 5 };						// 생사를 결정하는 HP
	COLORREF color;
	int score{ 0 };						// Enemy를 잡으면 1 상승 (승패 결정)
	int rank{ 3 };						// 최종 순위
};

struct Enemy {
public:
	Position pos;						// 위치
	bool isAlived{ true };				// 생사여부
};

struct Item {
public:
	Position pos;						// 위치
	bool isAlived{ true };				// 생사여부
};

struct Wall {
	Position pos;
};

struct sc_send_struct2 {
	int gameState;
	Player players[3];
	int enemy[3];
	int item[3];
	int clientIndex;			// 해당 클라이언트의 인덱스

};

struct sc_recv_struct {
	char size = 20U;
	char type;
	bool isAlive = true;
	int keyInputDirection;		// 클라이언트에서의 키입력 정보
	char playerID[10];			// 해당 클라이언트의 ID
};

#pragma pack()

sc_send_struct2 ServerSend2;

Enemy enemyList[35];
Wall List[36];
Item itemList[3];

Player player[3];
int clientCnt = 0;
sc_recv_struct ServerRecv;

SOCKET listen_sock;
SOCKET clientSock[3];

HANDLE hWriteEvent;
HANDLE hClientEvent[PLAYER_MAX];

void InitItem();
void InitEnemy();
void InitWall();
void InitPlayer();

void Accept(int clientIndex);

void MakeRank();						// GAME_SET에서 세 클라이언트의 순위 결정

void MovePlayer(int key, Player& p, int clientIndex);				// 플레이어의 위치 정보 갱신
void UpdatePlayer(Player& p, int clientIndex);			// 플레이어의 정보 갱신

void CheckPlayerByWallCollision(int key, Player& p);		// 플레이어와 벽의 충돌 체크
void CheckPlayerByPlayerCollision(int key, Player& p, int clientIndex);	// 플레이어간의 충돌 체크
void CheckPlayerByEnemyCollision(Player& p, int clientIndex);		// 플레이어와 Enemy의 충돌 체크
void CheckPlayerByItemCollision(Player& p, int clientIndex);		// 플레이어와 Item의 충돌 체크

int isGameOver(Player p[]);						// 종료 조건 처리 (죽은 플레이어 수 == 3)

int recvn(SOCKET s, char* buf, int len, int flags);
void err_quit(const char* msg);
void err_display(const char* msg);

