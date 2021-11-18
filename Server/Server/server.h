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

void SendData(SOCKET sock, sc_send_struct s_data);	// sc_send_struct 구조체 전송
void RecvData(SOCKET sock, sc_recv_struct r_data);	// sc_recv_struct 구조체 수신

void sendGameStart();					// GAME_RUNNING 전환을 클라이언트에게 전송

void MakeRank();						// GAME_SET에서 세 클라이언트의 순위 결정

void MovePlayer(Player p);				// 플레이어의 위치 정보 갱신
void UpdatePlayer(Player p);			// 플레이어의 정보 갱신

void CheckPlayerByWallCollision();		// 플레이어와 벽의 충돌 체크
void CheckPlayerByPlayerCollision();	// 플레이어간의 충돌 체크
void CheckPlayerByEnemyCollision();		// 플레이어와 Enemy의 충돌 체크
void CheckPlayerByItemCollision();		// 플레이어와 Item의 충돌 체크

void DeleteEnemy();						// Enemy 객체의 isAlived = false;  CheckPlayerByEnemyCollision()에서 호출
void DeleteItem();						// Item 객체의 isAlived = false;   CheckPlayerByItemCollision()에서 호출

bool isPlayerAlived();					// 플레이어의 HP 검사를 통해 생사 판별
int isGameOver();						// 종료 조건 처리 (죽은 플레이어 수 == 3)

template <typename T>
void SetPos(T gameObject, int x, int y);

void err_quit(const char* msg);
void err_display(const char* msg);
int recvn(SOCKET s, char* buf, int len, int flags);