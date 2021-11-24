#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

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

#define PLAYER_SPEED 10			// �÷��̾� �ӵ�

CRITICAL_SECTION cs;
int joinClient = 0;
int gameState = GAME_READY;




struct Position {
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
	int rank=3;					// ���� ����
};

struct Enemy {
public:
	Position pos;				// ��ġ
	bool isAlived=true;				// ���翩��
};
struct SendEnemy {
	int  cnt[3] = { -1,-1,-1 };                  //�浹 �� ��
};
struct Item {
public:
	Position pos;				// ��ġ
	bool isAlived;				// ���翩��
};
struct SendItem {
	int  cnt[3] = { -1,-1,-1 };                  //�浹 �� ��
};
struct Wall {
	Position pos;
};

struct sc_send_struct {
	char size;
	char type;
	Player players[3];			// �÷��̾���� ����Ʈ
	int gameState;				// GAME_READY/GAME_RUNNING/GAME_SET
	SendEnemy enemyList;
	SendItem itemList;
};

struct sc_recv_struct {
	char size=20U;
	char type;
	int keyInputDirection;		// Ŭ���̾�Ʈ������ Ű�Է� ����
	char playerID[10];			// �ش� Ŭ���̾�Ʈ�� ID
};

float deltaTime();						// deltaTime ��ȯ

void sendGameStart();					// GAME_RUNNING ��ȯ�� Ŭ���̾�Ʈ���� ����

void SendData(sc_send_struct* s_data);	// sc_send_struct ����ü ����
void RecvData(sc_recv_struct* r_data);	// sc_recv_struct ����ü ����

void MakeRank();						// GAME_SET���� �� Ŭ���̾�Ʈ�� ���� ����

void MovePlayer(int key,Player& p);				// �÷��̾��� ��ġ ���� ����
void UpdatePlayer(Player &p);			// �÷��̾��� ���� ����

void CheckPlayerByWallCollision(int key ,Player& p);		// �÷��̾�� ���� �浹 üũ
void CheckPlayerByPlayerCollision(int key, Player& p);	// �÷��̾�� �浹 üũ
void CheckPlayerByEnemyCollision(Player& p);		// �÷��̾�� Enemy�� �浹 üũ
void CheckPlayerByItemCollision(Player& p);		// �÷��̾�� Item�� �浹 üũ

void DeleteEnemy();						// Enemy ��ü�� isAlived = false;
										// CheckPlayerByEnemyCollision()���� ȣ��
void DeleteItem();						// Item ��ü�� isAlived = false;
										// CheckPlayerByItemCollision()���� ȣ��

bool isPlayerAlived();					// �÷��̾��� HP �˻縦 ���� ���� �Ǻ�

int isGameOver(Player p);						// ���� ���� ó�� (���� �÷��̾� �� == 3)




