#pragma once
#include "define.h"


float elapsedTime;
int gameState = GAME_RUNNING;

Player player[3];
Enemy enemyList[35];
Wall wallList[36];
Item itemList[4];

sc_send_struct sData;
sc_recv_struct rData;

DWORD WINAPI ProcessClient(LPVOID);

void SendData(SOCKET sock, sc_send_struct s_data);	// sc_send_struct ����ü ����
void RecvData(SOCKET sock, sc_recv_struct r_data);	// sc_recv_struct ����ü ����

void sendGameStart();					// GAME_RUNNING ��ȯ�� Ŭ���̾�Ʈ���� ����

void MakeRank();						// GAME_SET���� �� Ŭ���̾�Ʈ�� ���� ����

void MovePlayer(Player p);				// �÷��̾��� ��ġ ���� ����
void UpdatePlayer(Player p);			// �÷��̾��� ���� ����

void CheckPlayerByWallCollision();		// �÷��̾�� ���� �浹 üũ
void CheckPlayerByPlayerCollision();	// �÷��̾�� �浹 üũ
void CheckPlayerByEnemyCollision();		// �÷��̾�� Enemy�� �浹 üũ
void CheckPlayerByItemCollision();		// �÷��̾�� Item�� �浹 üũ

void DeleteEnemy();						// Enemy ��ü�� isAlived = false;  CheckPlayerByEnemyCollision()���� ȣ��
void DeleteItem();						// Item ��ü�� isAlived = false;   CheckPlayerByItemCollision()���� ȣ��

bool isPlayerAlived();					// �÷��̾��� HP �˻縦 ���� ���� �Ǻ�
int isGameOver();						// ���� ���� ó�� (���� �÷��̾� �� == 3)

template <typename T>
void SetPos(T gameObject, int x, int y);

void err_quit(const char* msg);
void err_display(const char* msg);
int recvn(SOCKET s, char* buf, int len, int flags);