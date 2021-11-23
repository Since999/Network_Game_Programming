#pragma once
#include "define.h"

float elapsedTime;
int gameState = GAME_READY;

Player player[PLAYER_MAX];
Enemy enemyList[35];
Wall wallList[36];
Item itemList[4];

sc_send_struct sData;
sc_recv_struct rData;

SOCKET client_sock[PLAYER_MAX];
SOCKADDR_IN clientaddr;
int addrlen;
HANDLE hThread;
int clientCnt = 0;

CRITICAL_SECTION cs;
HANDLE hReadEvent, hWriteEvent;

DWORD WINAPI ProcessClient(LPVOID);

void SendData(sc_send_struct s_data);	// sc_send_struct 구조체 전송
sc_recv_struct RecvData(SOCKET sock, sc_recv_struct r_data);	// sc_recv_struct 구조체 수신

void sendGameStart();					// GAME_RUNNING 전환을 클라이언트에게 전송

void MakeRank();						// GAME_SET에서 세 클라이언트의 순위 결정

void MovePlayer(int key, Player& p);
void UpdatePlayer(Player p[]);			// 플레이어의 정보 갱신

void CheckPlayerByWallCollision(int key, Player& p);		// 플레이어와 벽의 충돌 체크
void CheckPlayerByPlayerCollision(int key, Player& p);	// 플레이어간의 충돌 체크
void CheckPlayerByEnemyCollision(Player& p);		// 플레이어와 Enemy의 충돌 체크
void CheckPlayerByItemCollision(Player& p);		// 플레이어와 Item의 충돌 체크

void DeleteEnemy();						// Enemy 객체의 isAlived = false;  CheckPlayerByEnemyCollision()에서 호출
void DeleteItem();						// Item 객체의 isAlived = false;   CheckPlayerByItemCollision()에서 호출

bool isPlayerAlived();					// 플레이어의 HP 검사를 통해 생사 판별
int isGameOver(Player p[]);						// 종료 조건 처리 (죽은 플레이어 수 == 3)

template <typename T>
void SetPos(T gameObject, int x, int y);

void err_quit(const char* msg);
void err_display(const char* msg);
int recvn(SOCKET s, char* buf, int len, int flags);

void InitWall()
{
	player[0].score = 10;
	player[1].score = 9;
	player[2].score = 80;


	wallList[0].pos.x = 2;
	wallList[0].pos.y = 2;
	wallList[1].pos.x = 3;
	wallList[1].pos.y = 2;
	wallList[2].pos.x = 4;
	wallList[2].pos.y = 2;
	wallList[3].pos.x = 3;
	wallList[3].pos.y = 3;
	wallList[4].pos.x = 1;
	wallList[4].pos.y = 4;
	wallList[5].pos.x = 1;
	wallList[5].pos.y = 5;
	wallList[6].pos.x = 1;
	wallList[6].pos.y = 6;
	wallList[7].pos.x = 1;
	wallList[7].pos.y = 7;
	wallList[8].pos.x = 1;
	wallList[8].pos.y = 8;
	wallList[9].pos.x = 2;
	wallList[9].pos.y = 6;
	wallList[10].pos.x = 4;
	wallList[10].pos.y = 6;
	wallList[11].pos.x = 5;
	wallList[11].pos.y = 4;
	wallList[12].pos.x = 5;
	wallList[12].pos.y = 5;
	wallList[13].pos.x = 5;
	wallList[13].pos.y = 6;
	wallList[14].pos.x = 5;
	wallList[14].pos.y = 7;
	wallList[15].pos.x = 5;
	wallList[15].pos.y = 8;
	wallList[16].pos.x = 3;
	wallList[16].pos.y = 9;
	wallList[17].pos.x = 2;
	wallList[17].pos.y = 10;
	wallList[18].pos.x = 3;
	wallList[18].pos.y = 10;
	wallList[19].pos.x = 4;
	wallList[19].pos.y = 10;
	wallList[20].pos.x = 7;
	wallList[20].pos.y = 4;
	wallList[21].pos.x = 7;
	wallList[21].pos.y = 5;
	wallList[22].pos.x = 7;
	wallList[22].pos.y = 6;
	wallList[23].pos.x = 7;
	wallList[23].pos.y = 7;
	wallList[24].pos.x = 7;
	wallList[24].pos.y = 8;
	wallList[25].pos.x = 8;
	wallList[25].pos.y = 6;
	wallList[26].pos.x = 10;
	wallList[26].pos.y = 4;
	wallList[27].pos.x = 10;
	wallList[27].pos.y = 5;
	wallList[28].pos.x = 10;
	wallList[28].pos.y = 6;
	wallList[29].pos.x = 10;
	wallList[29].pos.y = 7;
	wallList[30].pos.x = 10;
	wallList[30].pos.y = 8;
	wallList[31].pos.x = 11;
	wallList[31].pos.y = 6;
	wallList[32].pos.x = 7;
	wallList[32].pos.y = 11;
	wallList[33].pos.x = 7;
	wallList[33].pos.y = 10;
	wallList[34].pos.x = 8;
	wallList[34].pos.y = 10;
	wallList[35].pos.x = 9;
	wallList[35].pos.y = 10;


}
void InitEnemy()
{
	enemyList[0].pos.x = 2;
	enemyList[0].pos.y = 0;
	enemyList[1].pos.x = 5;
	enemyList[1].pos.y = 0;
	enemyList[2].pos.x = 9;
	enemyList[2].pos.y = 0;
	enemyList[3].pos.x = 3;
	enemyList[3].pos.y = 1;
	enemyList[4].pos.x = 1;
	enemyList[4].pos.y = 1;
	enemyList[5].pos.x = 8;
	enemyList[5].pos.y = 1;
	enemyList[6].pos.x = 9;
	enemyList[6].pos.y = 1;
	enemyList[7].pos.x = 8;
	enemyList[7].pos.y = 2;
	enemyList[8].pos.x = 9;
	enemyList[8].pos.y = 2;
	enemyList[9].pos.x = 11;
	enemyList[9].pos.y = 2;
	enemyList[10].pos.x = 2;
	enemyList[10].pos.y = 3;
	enemyList[11].pos.x = 4;
	enemyList[11].pos.y = 3;
	enemyList[12].pos.x = 2;
	enemyList[12].pos.y = 4;
	enemyList[13].pos.x = 3;
	enemyList[13].pos.y = 4;
	enemyList[14].pos.x = 4;
	enemyList[14].pos.y = 4;
	enemyList[15].pos.x = 2;
	enemyList[15].pos.y = 5;
	enemyList[16].pos.x = 3;
	enemyList[16].pos.y = 5;
	enemyList[17].pos.x = 4;
	enemyList[17].pos.y = 5;
	enemyList[18].pos.x = 0;
	enemyList[18].pos.y = 5;
	enemyList[19].pos.x = 0;
	enemyList[19].pos.y = 7;
	enemyList[20].pos.x = 6;
	enemyList[20].pos.y = 4;
	enemyList[21].pos.x = 6;
	enemyList[21].pos.y = 5;
	enemyList[22].pos.x = 6;
	enemyList[22].pos.y = 6;
	enemyList[23].pos.x = 6;
	enemyList[23].pos.y = 7;
	enemyList[24].pos.x = 6;
	enemyList[24].pos.y = 8;
	enemyList[25].pos.x = 11;
	enemyList[25].pos.y = 4;
	enemyList[26].pos.x = 11;
	enemyList[26].pos.y = 5;
	enemyList[27].pos.x = 8;
	enemyList[27].pos.y = 9;
	enemyList[28].pos.x = 9;
	enemyList[28].pos.y = 9;
	enemyList[29].pos.x = 0;
	enemyList[29].pos.y = 11;
	enemyList[30].pos.x = 2;
	enemyList[30].pos.y = 11;
	enemyList[31].pos.x = 9;
	enemyList[31].pos.y = 6;
	enemyList[32].pos.x = 3;
	enemyList[32].pos.y = 11;
	enemyList[33].pos.x = 4;
	enemyList[33].pos.y = 11;
	enemyList[34].pos.x = 5;
	enemyList[34].pos.y = 11;



}
void InitItem()
{
	itemList[0].pos.x = 8;
	itemList[0].pos.y = 11;

	itemList[1].pos.x = 11;
	itemList[1].pos.y = 7;

	itemList[2].pos.x = 2;
	itemList[2].pos.y = 7;
}