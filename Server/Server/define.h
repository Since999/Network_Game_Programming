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

#define GAME_READY 10			// ������ Ŭ���̾�Ʈ �� < 3
#define GAME_RUNNING 11			// INGAME ���� (���� Ŭ���̾�Ʈ �� < 3)
#define GAME_SET 12				// ���� Ŭ���̾�Ʈ �� == 3

#define MOVE_NONE 20			// Ű �Է��� ���� ���� (�α��ν� ���)
#define MOVE_UP 21				// WŰ �Է�
#define MOVE_DOWN 22			// SŰ �Է�
#define MOVE_LEFT 23			// AŰ �Է�
#define MOVE_RIGHT 24			// DŰ �Է�

#define PLAYER_MAX 1
#define PLAYER_SPEED 10			// �÷��̾� �ӵ�#pragma once


typedef struct Position {
	float x, y;
};

class GameObject {
public:
	Position Pos;
};

class Player : GameObject {
public:
	Position pos;				// ��ġ
	char playerID[10];			// �α��ν� ����� ID
	bool isAlived;				// ���翩��
	int hp;						// ���縦 �����ϴ� HP
	int score;					// Enemy�� ������ 1 ��� (���� ����)
	int clientIndex;			// �ش� Ŭ���̾�Ʈ�� �ε���
	int rank;					// ���� ����
};

class Enemy : GameObject {
public:
	Position pos;				// ��ġ
	bool isAlived;				// ���翩��
};

class Item : GameObject {
public:
	Position pos;				// ��ġ
	bool isAlived;				// ���翩��
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
	int keyInputDirection;		// Ŭ���̾�Ʈ������ Ű�Է� ����
	char playerID[10];			// �ش� Ŭ���̾�Ʈ�� ID
};