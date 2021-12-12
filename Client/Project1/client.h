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
//#include <atlstr.h>
#include <conio.h>

#define IDC_BUTTON 100
#define IDC_EDIT 101

#define SERVERPORT 9000
#define BUFSIZE 1024

#define oneSize 50
#define boardSize 500
//#define SERVERIP   "220.120.241.242"
#define SERVERIP "127.0.0.1"

#define PLAYER_SPEED 10			// �÷��̾� �ӵ�
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

static int obstacleNumber{ 36 };
static int enemyNumber{ 35 };
static int itemNumber{ 3 };
static int hpNumber{ 5 };
static int extrahpNumber{ 3 };
static int boardCount{ 12 };
static int xS{ 12 };
static int yS{ 12 };

int gamestate;
int clientIndex;
DWORD curTime;
DWORD lastTime = GetTickCount();

HANDLE hThread;
HANDLE hSocketEvent, hRenderEvent;

#pragma pack(1)
struct Position {
	float x, y;
};

struct EXHP {
public:
	Position pos;                  // ��ġ
	bool isAlived{ false };         // ���翩��
};

class Player {
public:
	Position pos{ 0,0 };                  // ��ġ
	char playerID[10];             // �α��ν� ����� ID
	bool isAlived{ true };         // ���翩��

	int exhpList = 0;

	int hp{ 5 };                        // ���縦 �����ϴ� HP
	
	COLORREF color;
	int score{ 0 };                // Enemy�� ������ 1 ��� (���� ����)
	int rank;                      // ���� ����
};

struct Obstacle {
	Position pos;
	COLORREF iColor{};
};

struct Enemy {
	Position pos;                  // ��ġ
	bool isAlived{ true };          // ���翩��
};

struct Item {
	Position pos;                  // ��ġ
	bool isAlived{ true };          // ���翩��
};

struct HPBAR {
	Position pos;                  // ��ġ
	bool isAlived{ true };          // ���翩��
};



struct cs_send_struct {
	char size = 20U;
	char type;
	bool isAlive = true;
	int keyInputDirection;
	char playerID[10];

};


/*struct cs_recv_struct1 {
	int gameState;
};*/

struct cs_recv_struct2 {
	int gameState;
	Player players[3];
	int enemy[3];
	int item[3];
	int clientIndex;               // �ش� Ŭ���̾�Ʈ�� �ε���

};

#pragma pack()

/*struct cs_recv_struct {
	char size = 596U;
	char type;
	Player player[3];
	int gameState;
	Enemy enemyList[35];
	Item itemList[3];
	EXHP exhpList[3];
};*/

HINSTANCE g_hInst;
TCHAR lpszClass[] = TEXT("Mini Pac-Man");
TCHAR lpszClass2[] = TEXT("Mini Pac-Man Lobby");
TCHAR lpszClass3[] = TEXT("Mini Pac-Man Result");

SOCKET sock;
Obstacle List[36];
Enemy enemyList[35];
Item itemList[3];
HPBAR hpList[5];
EXHP exhpList[3];

Player p;
cs_send_struct clientSend;
//cs_recv_struct1 clientRecv1;
cs_recv_struct2 clientRecv2;
Player temp;

TCHAR str[10];
HWND hButton, hEdit;

int hpCnt = 0;

void CALLBACK TimerProc(HWND hWnd, UINT iMessage, UINT idEvent, DWORD dwTime);

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildProc2(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

DWORD WINAPI RecvThread(LPVOID arg);

void InitObstacle();
void InitItem();
void InitEnemy();
void InitHpBar();
void InitExHp();
void InitPlayer();

void Update();
void UpdateHP(int cnt);

void DrawBoard(HDC hdc, int boardCount, int xS, int yS);
void DrawPlayer(HDC hdc, int xS, int yS, cs_recv_struct2 recv);
void DrawObstacle(HDC hdc, int xS, int yS);
void DrawItem(HDC hdc, int xS, int yS);
void DrawEnemy(HDC hdc, int xS, int yS);
void DrawHp(HDC hdc, int xS, int yS);
void DrawExHp(HDC hdc, int xS, int yS, Player& p);

int recvn(SOCKET s, char* buf, int len, int flags);
void err_quit(const char* msg);
void err_display(const char* msg);