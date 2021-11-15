#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <iostream>
#include <vector>
#include <Windows.h>
using namespace std;

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
#define PLAYER_SPEED 10			// �÷��̾� �ӵ�

vector<SOCKET> sock_vector;

int gameState = GAME_READY;

typedef struct Position {
	float x, y;
};

class Player {
public:
	Position pos;				// ��ġ
	char playerID[10];			// �α��ν� ����� ID
	bool isAlived;				// ���翩��
	int hp;						// ���縦 �����ϴ� HP
	int score;					// Enemy�� ������ 1 ��� (���� ����)
	int clientIndex;			// �ش� Ŭ���̾�Ʈ�� �ε���
	int rank;					// ���� ����
};

class Enemy {
public:
	Position pos;				// ��ġ
	bool isAlived;				// ���翩��
};

class Item {
public:
	Position pos;				// ��ġ
	bool isAlived;				// ���翩��
};


struct sc_send_struct {
	Player players[3];			// �÷��̾���� ����Ʈ
	int gameState;				// GAME_READY/GAME_RUNNING/GAME_SET
};

struct sc_recv_struct {
	int keyInputDirection;		// Ŭ���̾�Ʈ������ Ű�Է� ����
	char playerID[10];			// �ش� Ŭ���̾�Ʈ�� ID
};

float deltaTime();						// deltaTime ��ȯ

void sendGameStart();					// GAME_RUNNING ��ȯ�� Ŭ���̾�Ʈ���� ����

void SendData(SOCKET sock, sc_send_struct s_data);	// sc_send_struct ����ü ����
void RecvData(SOCKET sock, sc_recv_struct r_data);	// sc_recv_struct ����ü ����

void MakeRank();						// GAME_SET���� �� Ŭ���̾�Ʈ�� ���� ����

void MovePlayer(Player p);				// �÷��̾��� ��ġ ���� ����
void UpdatePlayer(Player p);			// �÷��̾��� ���� ����

void CheckPlayerByWallCollision();		// �÷��̾�� ���� �浹 üũ
void CheckPlayerByPlayerCollision();	// �÷��̾�� �浹 üũ
void CheckPlayerByEnemyCollision();		// �÷��̾�� Enemy�� �浹 üũ
void CheckPlayerByItemCollision();		// �÷��̾�� Item�� �浹 üũ

void DeleteEnemy();						// Enemy ��ü�� isAlived = false;
										// CheckPlayerByEnemyCollision()���� ȣ��
void DeleteItem();						// Item ��ü�� isAlived = false;
										// CheckPlayerByItemCollision()���� ȣ��

bool isPlayerAlived();					// �÷��̾��� HP �˻縦 ���� ���� �Ǻ�

bool isGameOver();						// ���� ���� ó�� (���� �÷��̾� �� == 3)

void err_quit(const char* msg);
void err_display(const char* msg);
int recvn(SOCKET s, char* buf, int len, int flags);

sc_send_struct sData;
sc_recv_struct rData;