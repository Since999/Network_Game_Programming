#include "client.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	InitObstacle();
	InitItem();
	InitEnemy();
	InitHpBar();
	InitExHp();
	gamestate = GAME_READY;
	p.hp = 1;

	hReadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hReadEvent == NULL) return 1;
	hWriteEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if (hWriteEvent == NULL) return 1;
	
	HWND hWnd;
	MSG Message;
	Message.message = WM_NULL;
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

		WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
		WndClass.lpszClassName = lpszClass2;
		WndClass.lpfnWndProc = ChildProc;
		RegisterClassEx(&WndClass);

		WndClass.lpszClassName = lpszClass3;
		WndClass.lpfnWndProc = ChildProc2;
		RegisterClassEx(&WndClass);
	}
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,
		0, 0, 800, 800, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//while (GetMessage(&Message, 0, 0, 0)) {
	//	TranslateMessage(&Message);
	//	DispatchMessage(&Message);
	//}

	while (Message.message != WM_QUIT) {	
		//if (lastTime + 100 < curTime) {
		//	lastTime = curTime;
		//}

		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else {
			curTime = GetTickCount();
			if (curTime - lastTime >= 33) {
				Update();
				//Render();
				InvalidateRect(hWnd, NULL, TRUE);
				lastTime = GetTickCount();
			}
		}
	}


	CloseHandle(hReadEvent);
	CloseHandle(hWriteEvent);

	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	HDC mem1dc;
	HDC mem2dc;
	HWND child_hWnd;
	HWND child_hWnd2;

	PAINTSTRUCT ps;
	static RECT rect;
	static bool Selection{ false };
	static int Timer1Count = 10;
	//static int Timer2Count = 0;
	static HBITMAP hBit1, hBit2, oldBit1, oldBit2;
		

		
	//static int count = 1;
	int retval;
	strcpy(clientSend.playerID, "asd");
	switch (iMessage) {

	case WM_CREATE:
		GetClientRect(hWnd, &rect);
		

		if (gamestate == GAME_READY)
		{
			child_hWnd = CreateWindow(lpszClass2, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE,
				0, 0, 800, 800, hWnd, NULL, g_hInst, NULL);
			/*child_hWnd2 = CreateWindow(lpszClass3, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE,
				0, 0, 800, 800, hWnd, NULL, g_hInst, NULL);*/
			

		}

		break;

	
	case WM_TIMER:
		//Update();
		//InvalidateRect(hWnd, NULL, TRUE);
		switch (wParam) {
		case 1:
			//if (Timer1Count > 0)
			//	Timer1Count--;
			break;

		/*case 2:
			Timer2Count++;
			count++;
			if (count - Timer2Count == 1) {
				clientSend.keyInputDirection = 0;
				retval = send(sock, (char*)&clientSend, clientSend.size, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}
				else {
					//p.pos = clientRecv2.player[clientRecv2.clientIndex].pos;

					for (int i = 0; i < enemyNumber; ++i)
						if (i == clientRecv2.enemy[0])
							enemyList[i].isAlived = false;


					for (int i = 0; i < itemNumber; ++i)
						if (i == clientRecv2.item[0])
							itemList[i].isAlived = false;

				}



				retval = recvn(sock, (char*)&clientRecv2, sizeof(clientRecv2), 0);


				if (retval == 0) {
					break;
				}
				p.pos = clientRecv2.player[clientRecv2.clientIndex].pos;
				


				InvalidateRgn(hWnd, NULL, FALSE);
			}

			break;*/
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON:
			hdc = GetDC(hWnd);
			gamestate = GAME_SET;
			cout << gamestate << endl;
			ReleaseDC(hWnd, hdc);

			break;


		default:
			break;
		}
		break;



	case WM_KEYDOWN:
		if (gamestate == GAME_RUNNING)
		{
			WaitForSingleObject(hReadEvent, INFINITE);
			//if (count-Timer2Count == 1) {
			if (p.isAlived) {
				if (wParam == VK_LEFT)
				{
					clientSend.keyInputDirection = MOVE_LEFT;
				}
				else if (wParam == VK_RIGHT) {
					clientSend.keyInputDirection = MOVE_RIGHT;
				}

				else if (wParam == VK_UP)
				{
					clientSend.keyInputDirection = MOVE_UP;

				}
				else if (wParam == VK_DOWN)
				{
					clientSend.keyInputDirection = MOVE_DOWN;

				}

				//InvalidateRgn(hWnd, NULL, FALSE);
			}
			SetEvent(hWriteEvent);
		}
		break;
	case WM_KEYUP:
		clientSend.keyInputDirection = MOVE_NONE;
		break;
	
	case WM_PAINT:
		if (gamestate == GAME_RUNNING)
		{
			//SetTimer(hWnd, 2, 33, TimerProc);
			//SetTimer(hWnd, 1, 3000, NULL);
			hdc = BeginPaint(hWnd, &ps);
			hBit1 = CreateCompatibleBitmap(hdc, 800, 800);


			mem1dc = CreateCompatibleDC(hdc);
			mem2dc = CreateCompatibleDC(mem1dc);

			oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);
			oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit2);

			Rectangle(mem1dc, 0, 0, 800, 800);
			Rectangle(mem2dc, 0, 0, 800, 800);


			DrawBoard(mem1dc, boardCount, xS, yS);
			DrawEnemy(mem1dc, xS, yS);
			DrawPlayer(mem1dc, xS, yS, clientRecv2);
			DrawObstacle(mem1dc, xS, yS);
			DrawItem(mem1dc, xS, yS);
			DrawHp(mem1dc, xS, yS);
			DrawExHp(mem1dc, xS, yS);



			if (Timer1Count == 9)
				hpList[4].isAlived = false;
			else if (Timer1Count == 8)
				hpList[3].isAlived = false;
			else if (Timer1Count == 7)
				hpList[2].isAlived = false;
			else if (Timer1Count == 6)
				hpList[1].isAlived = false;
			else if (Timer1Count == 5)
				hpList[0].isAlived = false;

			if (exhpList[2].isAlived || exhpList[1].isAlived || exhpList[0].isAlived) {
				if (Timer1Count == 4) {
					if (exhpList[0].isAlived) {
						if (exhpList[1].isAlived) {
							if (exhpList[2].isAlived) {
								exhpList[2].isAlived = false;
							}
							else exhpList[1].isAlived = false;
						}
						else exhpList[0].isAlived = false;
					}
				}

				else if (Timer1Count == 3) {
					if (exhpList[0].isAlived) {
						if (exhpList[1].isAlived) {
							if (exhpList[2].isAlived) {
								exhpList[2].isAlived = false;
							}
							else exhpList[1].isAlived = false;
						}
						else exhpList[0].isAlived = false;
					}
				}

				else if (Timer1Count == 2) {
					if (exhpList[0].isAlived) {
						if (exhpList[1].isAlived) {
							if (exhpList[2].isAlived) {
								exhpList[2].isAlived = false;
							}
							else exhpList[1].isAlived = false;
						}
						else exhpList[0].isAlived = false;
					}
				}
			}

			if (!exhpList[2].isAlived && !exhpList[1].isAlived && !exhpList[0].isAlived
				&& !hpList[0].isAlived && !hpList[1].isAlived && !hpList[2].isAlived
				&& !hpList[3].isAlived && !hpList[4].isAlived) {
				TextOut(mem1dc, 350, 100, L"GAME OVER", strlen("GAME OVER"));
				p.isAlived = false;
			}


			TextOut(mem1dc, 80, 32, L"HP", strlen("HP"));
			DrawHp(mem1dc, xS, yS);


			TextOut(mem1dc, 450, 32, L"EXTRA", strlen("EXTRA"));
			DrawExHp(mem1dc, xS, yS);


			BitBlt(hdc, 0, 0, 800, 800, mem1dc, 0, 0, SRCCOPY);
			BitBlt(mem1dc, 0, 0, 800, 800, mem2dc, 0, 0, SRCCOPY);

			if (hBit1 == NULL) {
				hBit1 = CreateCompatibleBitmap(hdc, 800, 800);
			}

			if (hBit2 == NULL) {
				hBit2 = CreateCompatibleBitmap(mem1dc, 800, 800);
			}

			DeleteDC(mem1dc);
			DeleteDC(mem2dc);

			EndPaint(hWnd, &ps);
		}
		break;
	
	

	case WM_DESTROY:
		//KillTimer(hWnd, 1);
		//KillTimer(hWnd, 2);
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void Update()
{

}


DWORD WINAPI SendThread(LPVOID arg)
{
	int retval = 0;
	SOCKET client_sock = (SOCKET)arg;
	while (1) {
		switch (gamestate) {
		case GAME_RUNNING:
			if (p.isAlived) {
				WaitForSingleObject(hWriteEvent, INFINITE);
				//cout << clientSend.keyInputDirection << endl;
				retval = send(sock, (char*)&clientSend, clientSend.size, 0);
				if (retval == SOCKET_ERROR) {
					err_display("send()");
					break;
				}
				SetEvent(hReadEvent);
			}
			break;
		}
	}
}

DWORD WINAPI RecvThread(LPVOID arg)
{
	int retval = 0;
	SOCKET client_sock = (SOCKET)arg;
	while (1) {
		switch (gamestate) {
		case GAME_RUNNING:
			retval = recvn(sock, (char*)&clientRecv2, sizeof(clientRecv2), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("recv()");
				break;
			}
			p.pos = clientRecv2.players[clientRecv2.clientIndex].pos;
			//p.pos = clientRecv2.player->pos;
			cout << p.pos.x << "       " << p.pos.y << endl;

			p.score = clientRecv2.players->score;
			std::cout << "score: " << clientRecv2.players->score << endl;

			for (int i = 0; i < enemyNumber; ++i)
				if (i == clientRecv2.enemy[0])
					enemyList[i].isAlived = false;

			for (int i = 0; i < itemNumber; ++i)
				if (i == clientRecv2.item[0]) {
					itemList[i].isAlived = false;

					/*	if (Timer1Count > 4) {
							for (int i = 0; i < extrahpNumber; ++i)
								exhpList[i].isAlived = clientRecv2.player->exhpList[i].isAlived;*/
				}
			break;
		}
	}
}

//void CALLBACK TimerProc(HWND hWnd, UINT iMessage, UINT idEvent, DWORD dwTime)
//{
//	HDC hdc;
//	HWND child_hWnd2;
//	hdc = GetDC(hWnd);
//	
//	static int Timer2Count = 0;
//	static int count = 1;
//	int retval;
//	//GetClientRect(hWnd, &rect);
//	//MyBrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
//	//MyPen = CreatePen(PS_SOLID, rand() % 5, RGB(rand() % 255, rand() % 255, rand() % 255)); OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);
//	//OldPen = (HPEN)SelectObject(hdc, MyPen);
//	//Ellipse(hdc, rand() % (rect.right), rand() % (rect.bottom), rand() % (rect.right), rand() % (rect.bottom)); SelectObject(hdc, OldBrush);
//	//SelectObject(hdc, OldPen); DeleteObject(MyBrush); DeleteObject(MyPen);
//	Timer2Count++;
//	count++;
//	if (count - Timer2Count == 1) {
//		clientSend.keyInputDirection = 0;
//		//retval = send(sock, (char*)&clientSend, clientSend.size, 0);
//		/*if (retval == SOCKET_ERROR) {
//			err_display("send()");
//			break;
//		}*/
//		//else {
//			//p.pos = clientRecv2.player[clientRecv2.clientIndex].pos;
//
//			for (int i = 0; i < enemyNumber; ++i)
//				if (i == clientRecv2.enemy[0])
//					enemyList[i].isAlived = false;
//
//
//			for (int i = 0; i < itemNumber; ++i)
//				if (i == clientRecv2.item[0]) {
//					itemList[i].isAlived = false;
//					/*if (!exhpList[2].isAlived) {
//						if (!exhpList[1].isAlived) {
//							if (!exhpList[0].isAlived) {
//								exhpList[0].isAlived = true;
//							}
//							else exhpList[1].isAlived = true;
//						}
//						else exhpList[2].isAlived = true;
//					}*/
//				}
//
//		//}
//
//
//
//	//	retval = recvn(sock, (char*)&clientRecv2, sizeof(clientRecv2), 0);
//
//
//		/*if (retval == 0) {
//			break;
//		}*/
//		p.pos = clientRecv2.player[clientRecv2.clientIndex].pos;
//
//				if (gamestate == GAME_SET)
//				{
//					/*child_hWnd2 = CreateWindow(lpszClass3, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE,
//						0, 0, 800, 800, hWnd, NULL, g_hInst, NULL);*/
//					//DestroyWindow(hWnd);
//
//
//				}
//
//		InvalidateRgn(hWnd, NULL, FALSE);
//	}
//
//	ReleaseDC(hWnd, hdc);
//}

LRESULT CALLBACK ChildProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	hdc = GetDC(hWnd);
	char s[10];
	int len = 10;
	switch (iMessage) {
	case WM_CREATE:
		hButton = CreateWindow(L"button", L"LOGIN", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			500, 600, 100, 25, hWnd, (HMENU)IDC_BUTTON, g_hInst, NULL);
		hEdit = CreateWindow(L"edit", L"edit", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE,
			300, 600, 200, 30, hWnd, (HMENU)IDC_EDIT, g_hInst, NULL);
		break;
	case WM_COMMAND:
		cout << "123" << endl;

		switch (LOWORD(wParam))
		{
		case IDC_BUTTON:
			
			//TextOut(hdc, 400, 400, L"Hello", 5);

			gamestate = GAME_RUNNING;
			
			cout << gamestate << endl;
			int retval;

			// 윈속 초기화
			WSADATA wsa;
			if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
				return 1;

			// socket()
			sock = socket(AF_INET, SOCK_STREAM, 0);

			if (sock == INVALID_SOCKET) err_quit("socket()");

			// connect()
			SOCKADDR_IN serveraddr;
			ZeroMemory(&serveraddr, sizeof(serveraddr));
			serveraddr.sin_family = AF_INET;
			serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
			serveraddr.sin_port = htons(SERVERPORT);
			retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
			if (retval == SOCKET_ERROR) err_quit("connect()");

			hThread[0] = CreateThread(NULL, 0, SendThread, (LPVOID)sock, 0, NULL);
			hThread[1] = CreateThread(NULL, 0, RecvThread, (LPVOID)sock, 0, NULL);

			DestroyWindow(hWnd);
			ReleaseDC(hWnd, hdc);
			
			break;
		case IDC_EDIT:
			
			GetDlgItemText(hWnd, IDC_EDIT, str, 10);
			hdc = GetDC(hWnd);
			WideCharToMultiByte(CP_ACP, 0, str, len, clientSend.playerID, len, NULL, NULL);

			ReleaseDC(hWnd, hdc);
			break;

		default:
			break;
		}
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}
LRESULT CALLBACK ChildProc2(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	char s[10];
	int len = 10;
	switch (iMessage) {
	case WM_CREATE:
		hButton = CreateWindow(L"button", L"확인", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			500, 600, 100, 25, hWnd, (HMENU)IDC_BUTTON, g_hInst, NULL);

		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON:

			hdc = GetDC(hWnd);
			DestroyWindow(hWnd);
			ReleaseDC(hWnd, hdc);

			break;


		default:
			break;
		}
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
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
void DrawPlayer(HDC hdc, int xS, int yS, cs_recv_struct2 recv)
{
	//1P
	HBRUSH hBrush, oBrush;

	hBrush = CreateSolidBrush(RGB(0, 0, 0));
	oBrush = (HBRUSH)SelectObject(hdc, hBrush);

		
			hBrush = CreateSolidBrush(RGB(0, 255, 0));
			oBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Rectangle(hdc, 80 + recv.players[0].pos.x * oneSize + xS, 120 + recv.players[0].pos.y * oneSize + yS,
				80 + recv.players[0].pos.x * oneSize + xS + oneSize, 120 + recv.players[0].pos.y * oneSize + yS + oneSize);
			DeleteObject(hBrush);
			hBrush = CreateSolidBrush(RGB(255, 255, 0));
			oBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Rectangle(hdc, 80 + recv.players[1].pos.x * oneSize + xS, 120 + recv.players[1].pos.y * oneSize + yS,
				80 + recv.players[1].pos.x * oneSize + xS + oneSize, 120 + recv.players[1].pos.y * oneSize + yS + oneSize);
			DeleteObject(hBrush);
			hBrush = CreateSolidBrush(RGB(255, 0, 255));
			oBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Rectangle(hdc, 80 + recv.players[2].pos.x * oneSize + xS, 120 + recv.players[2].pos.y * oneSize + yS,
				80 + recv.players[2].pos.x * oneSize + xS + oneSize, 120 + recv.players[2].pos.y * oneSize + yS + oneSize);

	
		
	
	SelectObject(hdc, oBrush);
	DeleteObject(hBrush);

}
void InitObstacle()
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
void InitHpBar()
{
	
	hpList[0].pos.x = 2;
	hpList[0].pos.y = 0;
	hpList[1].pos.x = 3;
	hpList[1].pos.y = 0;
	hpList[2].pos.x = 4;
	hpList[2].pos.y = 0;
	hpList[3].pos.x = 5;
	hpList[3].pos.y = 0;
	hpList[4].pos.x = 6;
	hpList[4].pos.y = 0;

	
}

void InitExHp()
{

	exhpList[0].pos.x = 10;
	exhpList[0].pos.y = 0;
	exhpList[1].pos.x = 11;
	exhpList[1].pos.y = 0;
	exhpList[2].pos.x = 12;
	exhpList[2].pos.y = 0;


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


void DrawExHp(HDC hdc, int xS, int yS)
{
	HBRUSH hBrush, oBrush;
	int i;

	for (i = 0; i < extrahpNumber; i++) {
		if (exhpList[i].isAlived) {
			hBrush = CreateSolidBrush(RGB(255, 0, 0));
			oBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Rectangle(hdc, exhpList[i].pos.x * oneSize + xS, exhpList[i].pos.y * oneSize + yS,
				exhpList[i].pos.x * oneSize + xS + oneSize, exhpList[i].pos.y * oneSize + yS + oneSize);
			SelectObject(hdc, oBrush);
			DeleteObject(hBrush);
		}
	}
}



// 소켓 함수 오류 출력 후 종료
void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	//MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}
// 소켓 함수 오류 출력
void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}
// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}