#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
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



#define GAME_READY 11			// ������ Ŭ���̾�Ʈ �� < 3
#define GAME_RUNNING 12			// INGAME ���� (���� Ŭ���̾�Ʈ �� < 3)
#define GAME_SET 13				// ���� Ŭ���̾�Ʈ �� == 3

#define MOVE_NONE 20			// Ű �Է��� ���� ���� (�α��ν� ���)
#define MOVE_UP 21				// WŰ �Է�
#define MOVE_DOWN 22			// SŰ �Է�
#define MOVE_LEFT 23			// AŰ �Է�
#define MOVE_RIGHT 24			// DŰ �Է�

#define PLAYER_MAX 1
#define PLAYER_SPEED 10			// �÷��̾� �ӵ�

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
	Position pos;                  // ��ġ
	char playerID[10];             // �α��ν� ����� ID
	bool isAlived;                 // ���翩��
	int hp;                        // ���縦 �����ϴ� HP
	int score{ 0 };                // Enemy�� ������ 1 ��� (���� ����)
	int clientIndex;               // �ش� Ŭ���̾�Ʈ�� �ε���
	int rank;                      // ���� ����
};

struct Obstacle {
	Position pos;
	COLORREF iColor{};
};

struct Enemy {
	Position pos;                  // ��ġ
	bool isAlived = true;          // ���翩��
};

struct Item {
	Position pos;                  // ��ġ
	bool isAlived = true;          // ���翩��
};

struct HPBAR {
	Position pos;                  // ��ġ
	bool isAlived = true;          // ���翩��
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