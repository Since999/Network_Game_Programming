#pragma once
#include "define.h"

HINSTANCE g_hInst;
TCHAR lpszClass[] = TEXT("Mini Pac-Man");

SOCKET sock;
Obstacle List[36];
Enemy enemyList[35];
Item itemList[3];
HPBAR hpList[5];

Player p;
cs_send_struct clientSend;
cs_recv_struct clientRecv;
Player temp;
char clientPlayerID[10];
int clientIndex = -1;
bool m_bLoop = false;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

void InitObstacle();
void InitItem();
void InitEnemy();
void InitHpBar();

void DrawBoard(HDC hdc, int boardCount, int xS, int yS);
void DrawPlayer(HDC hdc, int xS, int yS);
void DrawObstacle(HDC hdc, int xS, int yS);
void DrawItem(HDC hdc, int xS, int yS);
void DrawEnemy(HDC hdc, int xS, int yS);
void DrawHp(HDC hdc, int xS, int yS);

void err_quit(const char* msg);
void err_display(const char* msg);
int recvn(SOCKET s, char* buf, int len, int flags);

void SendData(cs_send_struct s_data);
// void RecvData(SOCKET sock, cs_recv_struct r_data);

void CheckClientID(cs_recv_struct r_data);
void UpdateObject(cs_recv_struct r_data);