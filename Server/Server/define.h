#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <iostream>
#include <vector>
#include <Windows.h>
using namespace std;

#define SERVERPORT 9000
#define BUFSIZE 1024

#define GAME_READY 10			// 접속한 클라이언트 수 < 3
#define GAME_RUNNING 11			// INGAME 상태 (죽은 클라이언트 수 < 3)
#define GAME_SET 12				// 죽은 클라이언트 수 == 3

#define MOVE_NONE 20			// 키 입력이 없는 상태 (로그인시 사용)
#define MOVE_UP 21				// W키 입력
#define MOVE_DOWN 22			// S키 입력
#define MOVE_LEFT 23			// A키 입력
#define MOVE_RIGHT 24			// D키 입력

#define PLAYER_MAX 1
#define PLAYER_SPEED 10			// 플레이어 속도#pragma once


typedef struct Position {
	float x, y;
};

class GameObject {
public:
	Position Pos;
};

class Player : GameObject {
public:
	Position pos;				// 위치
	char playerID[10];			// 로그인시 사용할 ID
	bool isAlived;				// 생사여부
	int hp;						// 생사를 결정하는 HP
	int score;					// Enemy를 잡으면 1 상승 (승패 결정)
	int clientIndex;			// 해당 클라이언트의 인덱스
	int rank;					// 최종 순위
};

class Enemy : GameObject {
public:
	Position pos;				// 위치
	bool isAlived;				// 생사여부
};

class Item : GameObject {
public:
	Position pos;				// 위치
	bool isAlived;				// 생사여부
};

struct Wall : GameObject 
{
public:
	Position pos;
};

struct sc_send_struct {
	Player players[PLAYER_MAX];
	Enemy enemyList[36];
	Item itemList[4];
	int gameState;				// GAME_READY / GAME_RUNNING / GAME_SET
};

struct sc_recv_struct {
	int keyInputDirection;		// 클라이언트에서의 키입력 정보
	char playerID[10];			// 해당 클라이언트의 ID
};