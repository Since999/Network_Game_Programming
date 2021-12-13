#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
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

#define GAME_READY 11			// ������ Ŭ���̾�Ʈ �� < 3
#define GAME_RUNNING 12			// INGAME ���� (���� Ŭ���̾�Ʈ �� < 3)
#define GAME_SET 13				// ���� Ŭ���̾�Ʈ �� == 3

#define MOVE_NONE 20			// Ű �Է��� ���� ���� (�α��ν� ���)
#define MOVE_UP 21				// WŰ �Է�
#define MOVE_DOWN 22			// SŰ �Է�
#define MOVE_LEFT 23			// AŰ �Է�
#define MOVE_RIGHT 24			// DŰ �Է�

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
	Position pos;						// ��ġ
	bool isAlived{ false };				// ���翩��
};

class Player {
public:
	Position pos{ 0,0 };				// ��ġ
	char playerID[10];					// �α��ν� ����� ID
	bool isAlived{ true };				// ���翩��
	int exhpList=0;
	int hp{ 5 };						// ���縦 �����ϴ� HP
	COLORREF color;
	int score{ 0 };						// Enemy�� ������ 1 ��� (���� ����)
	int rank{ 3 };						// ���� ����
};

struct Enemy {
public:
	Position pos;						// ��ġ
	bool isAlived{ true };				// ���翩��
};

struct Item {
public:
	Position pos;						// ��ġ
	bool isAlived{ true };				// ���翩��
};

struct Wall {
	Position pos;
};

struct sc_send_struct2 {
	int gameState;
	Player players[3];
	int enemy[3];
	int item[3];
	int clientIndex;			// �ش� Ŭ���̾�Ʈ�� �ε���

};

struct sc_recv_struct {
	char size = 20U;
	char type;
	bool isAlive = true;
	int keyInputDirection;		// Ŭ���̾�Ʈ������ Ű�Է� ����
	char playerID[10];			// �ش� Ŭ���̾�Ʈ�� ID
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

void MakeRank();						// GAME_SET���� �� Ŭ���̾�Ʈ�� ���� ����

void MovePlayer(int key, Player& p, int clientIndex);				// �÷��̾��� ��ġ ���� ����
void UpdatePlayer(Player& p, int clientIndex);			// �÷��̾��� ���� ����

void CheckPlayerByWallCollision(int key, Player& p);		// �÷��̾�� ���� �浹 üũ
void CheckPlayerByPlayerCollision(int key, Player& p, int clientIndex);	// �÷��̾�� �浹 üũ
void CheckPlayerByEnemyCollision(Player& p, int clientIndex);		// �÷��̾�� Enemy�� �浹 üũ
void CheckPlayerByItemCollision(Player& p, int clientIndex);		// �÷��̾�� Item�� �浹 üũ

int isGameOver(Player p[]);						// ���� ���� ó�� (���� �÷��̾� �� == 3)

int recvn(SOCKET s, char* buf, int len, int flags);
void err_quit(const char* msg);
void err_display(const char* msg);

