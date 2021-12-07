#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)
#pragma warning(disable : 5208)
using namespace std;

#include <winsock2.h>
#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>


#define SERVERPORT 9000
#define BUFSIZE 1024

#define oneSize 50
#define boardSize 500
#define SERVERIP   "127.0.0.1"



#define GAME_READY 11			// 접속한 클라이언트 수 < 3
#define GAME_RUNNING 12			// INGAME 상태 (죽은 클라이언트 수 < 3)
#define GAME_SET 13				// 죽은 클라이언트 수 == 3

#define MOVE_NONE 20			// 키 입력이 없는 상태 (로그인시 사용)
#define MOVE_UP 21				// W키 입력
#define MOVE_DOWN 22			// S키 입력
#define MOVE_LEFT 23			// A키 입력
#define MOVE_RIGHT 24			// D키 입력

#define PLAYER_MAX 1
#define PLAYER_SPEED 10			// 플레이어 속도

static int obstacleNumber{ 36 };
static int enemyNumber{ 35 };
static int itemNumber{ 3 };
static int hpNumber{ 5 };
static int boardCount{ 12 };
static int xS{ 12 };
static int yS{ 12 };

struct Position {
	float x, y;
};

class Player {
public:
	Position pos;                  // 위치
	char playerID[10];             // 로그인시 사용할 ID
	bool isAlived;                 // 생사여부
	int hp;                        // 생사를 결정하는 HP
	int score{ 0 };                // Enemy를 잡으면 1 상승 (승패 결정)
	int clientIndex;               // 해당 클라이언트의 인덱스
	int rank;                      // 최종 순위
};

struct Obstacle {
	Position pos;
	COLORREF iColor{};
};

struct Enemy {
	Position pos;                  // 위치
	bool isAlived = true;          // 생사여부
};

struct Item {
	Position pos;                  // 위치
	bool isAlived = true;          // 생사여부
};

struct HPBAR {
	Position pos;                  // 위치
	bool isAlived = true;          // 생사여부
};

struct cs_send_struct {
	int keyInputDirection;
	char playerID[10];
};

struct cs_recv_struct {
	Player players[PLAYER_MAX];
	int gameState;
	Enemy enemyList[36];
	Item itemList[4];
};