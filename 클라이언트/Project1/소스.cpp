//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

#include <windows.h>
#include <tchar.h>
#include <iostream>

#define oneSize 50
#define boardSize 500



#pragma warning(disable : 5208)

using namespace std;

HINSTANCE g_hInst;
TCHAR lpszClass[] = TEXT("Mini Pac-Man");		


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{

	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	if (!hPrevInstance) {
		WndClass.cbSize = sizeof(WNDCLASSEX);
		WndClass.cbClsExtra = 0;
		WndClass.cbWndExtra = 0;
		WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		WndClass.hInstance = hInstance;
		WndClass.lpfnWndProc = (WNDPROC)WndProc;
		WndClass.lpszClassName = lpszClass;
		WndClass.lpszMenuName = NULL;
		WndClass.style = CS_HREDRAW | CS_VREDRAW;
		WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		RegisterClassEx(&WndClass);
	}
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,
		0, 0, 800, 800, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}


struct Position {
	int x, y;
};

class Player {
public:
	Position pos;                  // 위치
	char playerID[10];             // 로그인시 사용할 ID
	bool isAlived;                 // 생사여부
	int hp;                        // 생사를 결정하는 HP
	int score{ 0 };                // Enemy를 잡으면 1 상승 (승패 결정)
	int clientIndex;               // 해당 클라이언트의 인덱스
	int rank;                      // 최종 순위
};

struct Obstacle {
	Position pos;
	COLORREF iColor{};
};

struct Enemy {
	Position pos;                  // 위치
	bool isAlived = true;          // 생사여부
};

struct Item {
	Position pos;                  // 위치
	bool isAlived = true;          // 생사여부
};

struct HPBAR {
	Position pos;                  // 위치
	bool isAlived = true;          // 생사여부
};


static int obstacleNumber{ 36 };
static int enemyNumber{ 35 };
static int itemNumber{ 3 };
static int hpNumber{ 5 };

Obstacle List[36];
Enemy enemyList[50];
Item itemList[3];
HPBAR hpList[5];


static int boardCount{ 12 };

static int xS{ 12 };
static int yS{ 12 };


void DrawBoard(HDC hdc, int boardCount, int xS, int yS);
void DrawPlayer(HDC hdc, int xS, int yS);
void InitObstacle(HDC hdc, int xS, int yS);
void DrawObstacle(HDC hdc, int xS, int yS);
void InitItem(HDC hdc, int xS, int yS);
void DrawItem(HDC hdc, int xS, int yS);
void InitEnemy(HDC hdc, int xS, int yS);
void DrawEnemy(HDC hdc, int xS, int yS);
void InitHpBar(HDC hdc, int xS, int yS);
void DrawHp(HDC hdc, int xS, int yS);

Player p;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rect;
	static bool Selection{ false };
	

	switch (iMessage) {
	case WM_CREATE:
		GetClientRect(hWnd, &rect);
		
		break;
	
	case WM_KEYDOWN:
		if (wParam == VK_LEFT && p.pos.x != 0 && !(p.pos.x == 11 && p.pos.y == 4)
			&& !(p.pos.x == 11 && p.pos.y == 5) && !(p.pos.x == 11 && p.pos.y == 7)
			&& !(p.pos.x == 11 && p.pos.y == 8) && !(p.pos.x == 5 && p.pos.y == 10)
			&& !(p.pos.x == 5 && p.pos.y == 2) && !(p.pos.x == 10 && p.pos.y == 10)
			&& !(p.pos.x == 8 && p.pos.y == 11) && !(p.pos.x == 9 && p.pos.y == 6)
			&& !(p.pos.x == 8 && p.pos.y == 4) && !(p.pos.x == 8 && p.pos.y == 5)
			&& !(p.pos.x == 8 && p.pos.y == 7) && !(p.pos.x == 8 && p.pos.y == 8)
			&& !(p.pos.x == 6 && p.pos.y == 4) && !(p.pos.x == 6 && p.pos.y == 5)
			&& !(p.pos.x == 6 && p.pos.y == 6) && !(p.pos.x == 6 && p.pos.y == 7)
			&& !(p.pos.x == 6 && p.pos.y == 8) && !(p.pos.x == 4 && p.pos.y == 3)
			&& !(p.pos.x == 4 && p.pos.y == 9) && !(p.pos.x == 3 && p.pos.y == 6)
			&& !(p.pos.x == 2 && p.pos.y == 4) && !(p.pos.x == 2 && p.pos.y == 5)
			&& !(p.pos.x == 2 && p.pos.y == 7) && !(p.pos.x == 2 && p.pos.y == 8)
			) {
			p.pos.x--;

			//itemcheck
			if (wParam == VK_LEFT && (p.pos.x == 2 && p.pos.y == 7)) 
				itemList[2].isAlived = false;
				
			
			if (wParam == VK_LEFT && (p.pos.x == 8 && p.pos.y == 11)) 
				itemList[0].isAlived = false;
				
			

			//enemycheck
			if (wParam == VK_LEFT && (p.pos.x == 2 && p.pos.y == 0)) {
				if (enemyList[0].isAlived) {
					enemyList[0].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_LEFT && (p.pos.x == 5 && p.pos.y == 0)) {
				if (enemyList[1].isAlived) {
					enemyList[1].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_LEFT && (p.pos.x == 9 && p.pos.y == 0)) {
				if (enemyList[2].isAlived) {
					enemyList[2].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_LEFT && (p.pos.x == 1 && p.pos.y == 1)) {
				if (enemyList[4].isAlived) {
					enemyList[4].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_LEFT && (p.pos.x == 3 && p.pos.y == 1)) {
				enemyList[3].isAlived = false;
				++p.score;
			}
			if (wParam == VK_LEFT && (p.pos.x == 8 && p.pos.y == 1)) {
				if (enemyList[5].isAlived) {
					enemyList[5].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_LEFT && (p.pos.x == 9 && p.pos.y == 1)) {
				if (enemyList[6].isAlived) {
					enemyList[6].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_LEFT && (p.pos.x == 8 && p.pos.y == 2)) {
				if (enemyList[7].isAlived) {
					enemyList[7].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_LEFT && (p.pos.x == 9 && p.pos.y == 2)) {
				if (enemyList[8].isAlived) {
					enemyList[8].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_LEFT && (p.pos.x == 11 && p.pos.y == 2)) {
				if (enemyList[9].isAlived) {
					enemyList[9].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_LEFT && (p.pos.x == 4 && p.pos.y == 3)) {
				if (enemyList[11].isAlived) {
					enemyList[11].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_LEFT && (p.pos.x == 2 && p.pos.y == 4)) {
				if (enemyList[12].isAlived) {
					enemyList[12].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_LEFT && (p.pos.x == 3 && p.pos.y == 4)) {
				if (enemyList[13].isAlived) {
					enemyList[13].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_LEFT && (p.pos.x == 2 && p.pos.y == 5)) {
				if (enemyList[15].isAlived) {
					enemyList[15].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_LEFT && (p.pos.x == 3 && p.pos.y == 5)) {
				if (enemyList[16].isAlived) {
					enemyList[16].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_LEFT && (p.pos.x == 8 && p.pos.y == 9)) {
				if (enemyList[27].isAlived) {
					enemyList[27].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_LEFT && (p.pos.x == 9 && p.pos.y == 9)) {
				if (enemyList[28].isAlived) {
					enemyList[28].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_LEFT && (p.pos.x == 0 && p.pos.y == 11)) {
				if (enemyList[29].isAlived) {
					enemyList[29].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_LEFT && (p.pos.x == 2 && p.pos.y == 11)) {
				if (enemyList[30].isAlived) {
					enemyList[30].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_LEFT && (p.pos.x == 3 && p.pos.y == 11)) {
				if (enemyList[32].isAlived) {
					enemyList[32].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_LEFT && (p.pos.x == 4 && p.pos.y == 11)) {
				if (enemyList[33].isAlived) {
					enemyList[33].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_LEFT && (p.pos.x == 5 && p.pos.y == 11)) {
				if (enemyList[34].isAlived) {
					enemyList[34].isAlived = false;
					++p.score;
				}
			}
		

		}
		else if (wParam == VK_RIGHT && p.pos.x != 11 && !(p.pos.x == 0 && p.pos.y == 4)
			&& !(p.pos.x == 0 && p.pos.y == 5) && !(p.pos.x == 0 && p.pos.y == 6)
			&& !(p.pos.x == 0 && p.pos.y == 7) && !(p.pos.x == 0 && p.pos.y == 8)
			&& !(p.pos.x == 1 && p.pos.y == 2) && !(p.pos.x == 1 && p.pos.y == 10)
			&& !(p.pos.x == 2 && p.pos.y == 3) && !(p.pos.x == 2 && p.pos.y == 9)
			&& !(p.pos.x == 3 && p.pos.y == 6) && !(p.pos.x == 4 && p.pos.y == 4)
			&& !(p.pos.x == 4 && p.pos.y == 5) && !(p.pos.x == 4 && p.pos.y == 7)
			&& !(p.pos.x == 4 && p.pos.y == 8) && !(p.pos.x == 6 && p.pos.y == 4)
			&& !(p.pos.x == 6 && p.pos.y == 5) && !(p.pos.x == 6 && p.pos.y == 6)
			&& !(p.pos.x == 6 && p.pos.y == 7) && !(p.pos.x == 6 && p.pos.y == 8)
			&& !(p.pos.x == 6 && p.pos.y == 10) && !(p.pos.x == 6 && p.pos.y == 11)
			&& !(p.pos.x == 9 && p.pos.y == 4) && !(p.pos.x == 9 && p.pos.y == 5)
			&& !(p.pos.x == 9 && p.pos.y == 6) && !(p.pos.x == 9 && p.pos.y == 7)
			&& !(p.pos.x == 9 && p.pos.y == 8)) {
			p.pos.x++;

			//enemycheck
			if (wParam == VK_RIGHT && (p.pos.x == 2 && p.pos.y == 0)) {
				if (enemyList[0].isAlived) {
					enemyList[0].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_RIGHT && (p.pos.x == 5 && p.pos.y == 0)) {
				if (enemyList[1].isAlived) {
					enemyList[1].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_RIGHT && (p.pos.x == 9 && p.pos.y == 0)) {
				if (enemyList[2].isAlived) {
					enemyList[2].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_RIGHT && (p.pos.x == 1 && p.pos.y == 1)) {
				if (enemyList[4].isAlived) {
					enemyList[4].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_RIGHT && (p.pos.x == 3 && p.pos.y == 1)) {
				if (enemyList[3].isAlived) {
					enemyList[3].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_RIGHT && (p.pos.x == 8 && p.pos.y == 1)) {
				if (enemyList[5].isAlived) {
					enemyList[5].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_RIGHT && (p.pos.x == 9 && p.pos.y == 1)) {
				if (enemyList[6].isAlived) {
					enemyList[6].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_RIGHT && (p.pos.x == 8 && p.pos.y == 2)) {
				if (enemyList[7].isAlived) {
					enemyList[7].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_RIGHT && (p.pos.x == 9 && p.pos.y == 2)) {
				if (enemyList[8].isAlived) {
					enemyList[8].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_RIGHT && (p.pos.x == 11 && p.pos.y == 2)) {
				if (enemyList[9].isAlived) {
					enemyList[9].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_RIGHT && (p.pos.x == 2 && p.pos.y == 3)) {
				if (enemyList[10].isAlived) {
					enemyList[10].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_RIGHT && (p.pos.x == 4 && p.pos.y == 4)) {
				if (enemyList[14].isAlived) {
					enemyList[14].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_RIGHT && (p.pos.x == 3 && p.pos.y == 4)) {
				if (enemyList[13].isAlived) {
					enemyList[13].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_RIGHT && (p.pos.x == 4 && p.pos.y == 5)) {
				if (enemyList[17].isAlived) {
					enemyList[17].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_RIGHT && (p.pos.x == 3 && p.pos.y == 5)) {
				if (enemyList[16].isAlived) {
					enemyList[16].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_RIGHT && (p.pos.x == 8 && p.pos.y == 9)) {
				if (enemyList[27].isAlived) {
					enemyList[27].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_RIGHT && (p.pos.x == 9 && p.pos.y == 9)) {
				if (enemyList[28].isAlived) {
					enemyList[28].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_RIGHT && (p.pos.x == 2 && p.pos.y == 11)) {
				if (enemyList[30].isAlived) {
					enemyList[30].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_RIGHT && (p.pos.x == 3 && p.pos.y == 11)) {
				if (enemyList[32].isAlived) {
					enemyList[32].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_RIGHT && (p.pos.x == 4 && p.pos.y == 11)) {
				if (enemyList[33].isAlived) {
					enemyList[33].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_RIGHT && (p.pos.x == 5 && p.pos.y == 11)) {
				if (enemyList[34].isAlived) {
					enemyList[34].isAlived = false;
					++p.score;
				}
			}
		}
		else if (wParam == VK_UP && p.pos.y != 0 && !(p.pos.x == 1 && p.pos.y == 9)
			&& !(p.pos.x == 2 && p.pos.y == 7) && !(p.pos.x == 2 && p.pos.y == 3)
			&& !(p.pos.x == 2 && p.pos.y == 11) && !(p.pos.x == 3 && p.pos.y == 4)
			&& !(p.pos.x == 3 && p.pos.y == 11) && !(p.pos.x == 4 && p.pos.y == 3)
			&& !(p.pos.x == 4 && p.pos.y == 7) && !(p.pos.x == 4 && p.pos.y == 11)
			&& !(p.pos.x == 5 && p.pos.y == 9) && !(p.pos.x == 7 && p.pos.y == 9)
			&& !(p.pos.x == 8 && p.pos.y == 11) && !(p.pos.x == 8 && p.pos.y == 7)
			&& !(p.pos.x == 9 && p.pos.y == 11) && !(p.pos.x == 10 && p.pos.y == 9)
			&& !(p.pos.x == 11 && p.pos.y == 7)) {
			p.pos.y--;

			
			if (wParam == VK_UP && (p.pos.x == 11 && p.pos.y == 7)) 
				itemList[1].isAlived = false;
				
			
			if (wParam == VK_UP && (p.pos.x == 2 && p.pos.y == 7)) 
				itemList[2].isAlived = false;
				
			//ENEMY
			if (wParam == VK_UP && (p.pos.x == 4 && p.pos.y == 4)) {
				if (enemyList[14].isAlived) {
					enemyList[14].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_UP && (p.pos.x == 11 && p.pos.y == 4)) {
				if (enemyList[25].isAlived) {
					enemyList[25].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_UP && (p.pos.x == 2 && p.pos.y == 0)) {
				if (enemyList[0].isAlived) {
					enemyList[0].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_UP && (p.pos.x == 5 && p.pos.y == 0)) {
				if (enemyList[1].isAlived) {
					enemyList[1].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_UP && (p.pos.x == 2 && p.pos.y == 4)) {
				if (enemyList[12].isAlived) {
					enemyList[12].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_UP && (p.pos.x == 3 && p.pos.y == 4)) {
				if (enemyList[13].isAlived) {
					enemyList[13].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_UP && (p.pos.x == 3 && p.pos.y == 5)) {
				if (enemyList[16].isAlived) {
					enemyList[16].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_UP && (p.pos.x == 4 && p.pos.y == 3)) {
				if (enemyList[11].isAlived) {
					enemyList[11].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_UP && (p.pos.x == 11 && p.pos.y == 2)) {
				if (enemyList[9].isAlived) {
					enemyList[9].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_UP && (p.pos.x == 9 && p.pos.y == 0)) {
				if (enemyList[2].isAlived) {
					enemyList[2].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_UP && (p.pos.x == 9 && p.pos.y == 1)) {
				if (enemyList[6].isAlived) {
					enemyList[6].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_UP && (p.pos.x == 9 && p.pos.y == 2)) {
				if (enemyList[8].isAlived) {
					enemyList[8].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_UP && (p.pos.x == 8 && p.pos.y == 1)) {
				if (enemyList[5].isAlived) {
					enemyList[5].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_UP && (p.pos.x == 8 && p.pos.y == 2)) {
				if (enemyList[7].isAlived) {
					enemyList[7].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_UP && (p.pos.x == 1 && p.pos.y == 1)) {
				if (enemyList[4].isAlived) {
					enemyList[4].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_UP && (p.pos.x == 6 && p.pos.y == 4)) {
				if (enemyList[20].isAlived) {
					enemyList[20].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_UP && (p.pos.x == 6 && p.pos.y == 5)) {
				if (enemyList[21].isAlived) {
					enemyList[21].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_UP && (p.pos.x == 6 && p.pos.y == 6)) {
				if (enemyList[22].isAlived) {
					enemyList[22].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_UP && (p.pos.x == 6 && p.pos.y == 7)) {
				if (enemyList[23].isAlived) {
					enemyList[23].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_UP && (p.pos.x == 6 && p.pos.y == 8)) {
				if (enemyList[24].isAlived) {
					enemyList[24].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_UP && (p.pos.x == 9 && p.pos.y == 6)) {
				if (enemyList[31].isAlived) {
					enemyList[31].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_UP && (p.pos.x == 11 && p.pos.y == 4)) {
				if (enemyList[32].isAlived) {
					enemyList[32].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_UP && (p.pos.x == 0 && p.pos.y == 5)) {
				if (enemyList[18].isAlived) {
					enemyList[18].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_UP && (p.pos.x == 0 && p.pos.y == 7)) {
				if (enemyList[19].isAlived) {
					enemyList[19].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_UP && (p.pos.x == 2 && p.pos.y == 3)) {
				if (enemyList[10].isAlived) {
					enemyList[10].isAlived = false;
					++p.score;
				}
			}
			
			
		}
		else if (wParam == VK_DOWN && p.pos.y != 11 && !(p.pos.x==1 && p.pos.y==3)
			&& !(p.pos.x == 2 && p.pos.y == 1) && !(p.pos.x == 3 && p.pos.y == 1)
			&& !(p.pos.x == 4 && p.pos.y == 1) && !(p.pos.x == 5 && p.pos.y == 3)
			&& !(p.pos.x == 7 && p.pos.y == 3) && !(p.pos.x == 10 && p.pos.y == 3)
			&& !(p.pos.x == 2 && p.pos.y == 5) && !(p.pos.x == 4 && p.pos.y == 5)
			&& !(p.pos.x == 8 && p.pos.y == 5) && !(p.pos.x == 11 && p.pos.y == 5)
			&& !(p.pos.x == 2 && p.pos.y == 9) && !(p.pos.x == 3 && p.pos.y == 8)
			&& !(p.pos.x == 4 && p.pos.y == 9) && !(p.pos.x == 7 && p.pos.y == 9)
			&& !(p.pos.x == 8 && p.pos.y == 9) && !(p.pos.x == 9 && p.pos.y == 9)) {
			p.pos.y++;

			//ENEMY
			if (wParam == VK_DOWN && (p.pos.x == 4 && p.pos.y == 4)) {
				if (enemyList[14].isAlived) {
					enemyList[14].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_DOWN && (p.pos.x == 11 && p.pos.y == 4)) {
				if (enemyList[25].isAlived) {
					enemyList[25].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_DOWN && (p.pos.x == 3 && p.pos.y == 1)) {
				if (enemyList[3].isAlived) {
					enemyList[3].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_DOWN && (p.pos.x == 11 && p.pos.y == 5)) {
				if (enemyList[26].isAlived) {
					enemyList[26].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_DOWN && (p.pos.x == 2 && p.pos.y == 4)) {
				if (enemyList[12].isAlived) {
					enemyList[12].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_DOWN && (p.pos.x == 0 && p.pos.y == 11)) {
				if (enemyList[29].isAlived) {
					enemyList[29].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_DOWN && (p.pos.x == 3 && p.pos.y == 5)) {
				if (enemyList[16].isAlived) {
					enemyList[16].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_DOWN && (p.pos.x == 2 && p.pos.y == 5)) {
				if (enemyList[15].isAlived) {
					enemyList[15].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_DOWN && (p.pos.x == 11 && p.pos.y == 2)) {
				if (enemyList[9].isAlived) {
					enemyList[9].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_DOWN && (p.pos.x == 4 && p.pos.y == 5)) {
				if (enemyList[17].isAlived) {
					enemyList[17].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_DOWN && (p.pos.x == 8 && p.pos.y == 9)) {
				if (enemyList[27].isAlived) {
					enemyList[27].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_DOWN && (p.pos.x == 5 && p.pos.y == 11)) {
				if (enemyList[34].isAlived) {
					enemyList[34].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_DOWN && (p.pos.x == 9 && p.pos.y == 1)) {
				if (enemyList[6].isAlived) {
					enemyList[6].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_DOWN && (p.pos.x == 9 && p.pos.y == 2)) {
				if (enemyList[8].isAlived) {
					enemyList[8].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_DOWN && (p.pos.x == 8 && p.pos.y == 1)) {
				if (enemyList[5].isAlived) {
					enemyList[5].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_DOWN && (p.pos.x == 8 && p.pos.y == 2)) {
				if (enemyList[7].isAlived) {
					enemyList[7].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_DOWN && (p.pos.x == 1 && p.pos.y == 1)) {
				if (enemyList[4].isAlived) {
					enemyList[4].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_DOWN && (p.pos.x == 6 && p.pos.y == 4)) {
				if (enemyList[20].isAlived) {
					enemyList[20].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_DOWN && (p.pos.x == 6 && p.pos.y == 5)) {
				if (enemyList[21].isAlived) {
					enemyList[21].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_DOWN && (p.pos.x == 6 && p.pos.y == 6)) {
				if (enemyList[22].isAlived) {
					enemyList[22].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_DOWN && (p.pos.x == 6 && p.pos.y == 7)) {
				if (enemyList[23].isAlived) {
					enemyList[23].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_DOWN && (p.pos.x == 6 && p.pos.y == 8)) {
				if (enemyList[24].isAlived) {
					enemyList[24].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_DOWN && (p.pos.x == 9 && p.pos.y == 6)) {
				if (enemyList[31].isAlived) {
					enemyList[31].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_DOWN && (p.pos.x == 11 && p.pos.y == 4)) {
				if (enemyList[32].isAlived) {
					enemyList[32].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_DOWN && (p.pos.x == 0 && p.pos.y == 5)) {
				if (enemyList[18].isAlived) {
					enemyList[18].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_DOWN && (p.pos.x == 0 && p.pos.y == 7)) {
				if (enemyList[19].isAlived) {
					enemyList[19].isAlived = false;
					++p.score;
				}
			}
			if (wParam == VK_DOWN && (p.pos.x == 9 && p.pos.y == 9)) {
				if (enemyList[28].isAlived) {
					enemyList[28].isAlived = false;
					++p.score;
				}
			}
			

		}
		InvalidateRgn(hWnd, NULL, TRUE);
		
		break;
	
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		DrawBoard(hdc, boardCount, xS, yS);
		DrawPlayer(hdc, xS, yS);
		InitObstacle(hdc, xS, yS);
		DrawObstacle(hdc, xS, yS);
		InitItem(hdc, xS, yS);
		DrawItem(hdc, xS, yS);
		InitEnemy(hdc, xS, yS);
		DrawEnemy(hdc, xS, yS);
		
		InitHpBar(hdc, xS, yS);
		DrawHp(hdc, xS, yS);
		std::cout << p.score << std::endl;
		EndPaint(hWnd, &ps);
		break;
	
	

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void DrawBoard(HDC hdc, int boardCount, int xS, int yS)
{
	int i;

	for (int i = 0; i <= boardCount; i++)
	{
		MoveToEx(hdc, 80+i * oneSize + xS, 120+0 + yS, NULL);
		LineTo(hdc, 80+i * oneSize + xS, 120+boardCount * oneSize + yS);
		MoveToEx(hdc, 80+0 + xS, 120+i * oneSize + yS, NULL);
		LineTo(hdc, 80+boardCount * oneSize + xS, 120+i * oneSize + yS);
	}
}



void DrawPlayer(HDC hdc, int xS, int yS)
{
	HBRUSH hBrush, oBrush;

	hBrush = CreateSolidBrush(RGB(0, 255, 0));
	oBrush = (HBRUSH)SelectObject(hdc, hBrush);
	
    Rectangle(hdc, 80+ p.pos.x * oneSize + xS, 120+ p.pos.y * oneSize + yS,
		80+ p.pos.x * oneSize + xS + oneSize, 120+ p.pos.y * oneSize + yS + oneSize);
	SelectObject(hdc, oBrush);
	DeleteObject(hBrush);

}




void InitObstacle(HDC hdc, int xS, int yS)
{
	List[0].pos.x = 2 ;
	List[0].pos.y = 2 ;
	List[1].pos.x = 3;
	List[1].pos.y = 2;
	List[2].pos.x = 4;
	List[2].pos.y = 2;
	List[3].pos.x = 3;
	List[3].pos.y = 3;
	List[4].pos.x = 1;
	List[4].pos.y = 4;
	List[5].pos.x = 1;
	List[5].pos.y = 5;
	List[6].pos.x = 1;
	List[6].pos.y = 6;
	List[7].pos.x = 1;
	List[7].pos.y = 7;
	List[8].pos.x = 1;
	List[8].pos.y = 8;
	List[9].pos.x = 2;
	List[9].pos.y = 6;
	List[10].pos.x = 4;
	List[10].pos.y = 6;
	List[11].pos.x = 5;
	List[11].pos.y = 4;
	List[12].pos.x = 5;
	List[12].pos.y = 5;
	List[13].pos.x = 5;
	List[13].pos.y = 6;
	List[14].pos.x = 5;
	List[14].pos.y = 7;
	List[15].pos.x = 5;
	List[15].pos.y = 8;
	List[16].pos.x = 3;
	List[16].pos.y = 9;
	List[17].pos.x = 2;
	List[17].pos.y = 10;
	List[18].pos.x = 3;
	List[18].pos.y = 10;
	List[19].pos.x = 4;
	List[19].pos.y = 10;
	List[20].pos.x = 7;
	List[20].pos.y = 4;
	List[21].pos.x = 7;
	List[21].pos.y = 5;
	List[22].pos.x = 7;
	List[22].pos.y = 6;
	List[23].pos.x = 7;
	List[23].pos.y = 7;
	List[24].pos.x = 7;
	List[24].pos.y = 8;
	List[25].pos.x = 8;
	List[25].pos.y = 6;
	List[26].pos.x = 10;
	List[26].pos.y = 4;
	List[27].pos.x = 10;
	List[27].pos.y = 5;
	List[28].pos.x = 10;
	List[28].pos.y = 6;
	List[29].pos.x = 10;
	List[29].pos.y = 7;
	List[30].pos.x = 10;
	List[30].pos.y = 8;
	List[31].pos.x = 11;
	List[31].pos.y = 6;
	List[32].pos.x = 7;
	List[32].pos.y = 11;
	List[33].pos.x = 7;
	List[33].pos.y = 10;
	List[34].pos.x = 8;
	List[34].pos.y = 10;
	List[35].pos.x = 9;
	List[35].pos.y = 10;
	

}


void InitEnemy(HDC hdc, int xS, int yS)
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

void InitItem(HDC hdc, int xS, int yS)
{
	itemList[0].pos.x = 8;
	itemList[0].pos.y = 11;

	itemList[1].pos.x = 11;
	itemList[1].pos.y = 7;

	itemList[2].pos.x = 2;
	itemList[2].pos.y = 7;
}

void InitHpBar(HDC hdc, int xS, int yS)
{
	
	hpList[0].pos.x = 5;
	hpList[0].pos.y = 0;
	hpList[1].pos.x = 6;
	hpList[1].pos.y = 0;
	hpList[2].pos.x = 7;
	hpList[2].pos.y = 0;
	hpList[3].pos.x = 8;
	hpList[3].pos.y = 0;
	hpList[4].pos.x = 9;
	hpList[4].pos.y = 0;

	
}



void DrawObstacle(HDC hdc, int xS, int yS)
{
	HBRUSH hBrush, oBrush;
	int i;

	for (i = 0; i < obstacleNumber; i++) {
		hBrush = CreateSolidBrush(RGB(0, 0, 0));
		oBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Rectangle(hdc, 80+List[i].pos.x * oneSize + xS, 120+List[i].pos.y * oneSize + yS,
			80+List[i].pos.x * oneSize + xS + oneSize, 120+List[i].pos.y * oneSize + yS + oneSize);
		SelectObject(hdc, oBrush);
		DeleteObject(hBrush);
	}
}

void DrawItem(HDC hdc, int xS, int yS)
{
	HBRUSH hBrush, oBrush;
	int i;

	for (i = 0; i < itemNumber; i++) {
		if (itemList[i].isAlived) {
			hBrush = CreateSolidBrush(RGB(255, 0, 0));
			oBrush = (HBRUSH)SelectObject(hdc, hBrush);

			Rectangle(hdc, 80 + itemList[i].pos.x * oneSize + xS, 120 + itemList[i].pos.y * oneSize + yS,
				80 + itemList[i].pos.x * oneSize + xS + oneSize, 120 + itemList[i].pos.y * oneSize + yS + oneSize);
			SelectObject(hdc, oBrush);
			DeleteObject(hBrush);
		}
	}
}

void DrawEnemy(HDC hdc, int xS, int yS)
{
	HBRUSH hBrush, oBrush;
	int i;

	for (i = 0; i < enemyNumber; i++) {
		if (enemyList[i].isAlived) {
			hBrush = CreateSolidBrush(RGB(0, 0, 255));
			oBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Rectangle(hdc, 80 + enemyList[i].pos.x * oneSize + xS, 120 + enemyList[i].pos.y * oneSize + yS,
				80 + enemyList[i].pos.x * oneSize + xS + oneSize, 120 + enemyList[i].pos.y * oneSize + yS + oneSize);
			SelectObject(hdc, oBrush);
			DeleteObject(hBrush);
		}
	}
}

void DrawHp(HDC hdc, int xS, int yS)
{
	HBRUSH hBrush, oBrush;
	int i;

	for (i = 0; i < hpNumber; i++) {
		if (hpList[i].isAlived) {
			hBrush = CreateSolidBrush(RGB(255, 0, 0));
			oBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Rectangle(hdc, hpList[i].pos.x * oneSize + xS, hpList[i].pos.y * oneSize + yS,
				hpList[i].pos.x * oneSize + xS + oneSize, hpList[i].pos.y * oneSize + yS + oneSize);
			SelectObject(hdc, oBrush);
			DeleteObject(hBrush);
		}
	}
}


