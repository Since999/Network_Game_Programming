#include "client.h"
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
HWND hWnd;
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	InitObstacle();
	InitItem();
	InitEnemy();
	InitHpBar();
	InitExHp();
	gamestate = GAME_READY;
	//p.hp = 1;

	MSG Message;
	Message.message = WM_NULL;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;
	HWND child_hWnd2;
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

		WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		WndClass.lpszClassName = lpszClass3;
		WndClass.lpfnWndProc = ChildProc2;
		RegisterClassEx(&WndClass);
	}
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,
		0, 0, 800, 800, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (Message.message != WM_QUIT) {

		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else {
			if (gamestate == GAME_RUNNING) {
				curTime = GetTickCount();
				if (curTime - lastTime >= FPS) {
					int retval = 0;
					//Render();
					hpCnt++;
					if (hpCnt % 90 == 0) {
						UpdateHP(hpCnt);
					}
					if (p.isAlived == false)
					{
						clientSend.isAlive = p.isAlived;
						
						retval = send(sock, (char*)&clientSend, clientSend.size, 0);
					}
					//gamestate = clientRecv2.gameState;

					InvalidateRect(hWnd, NULL, FALSE);
					lastTime = GetTickCount();
				}
			}
			else if (gamestate == GAME_SET)
			{
				static RECT rect;
				GetClientRect(hWnd, &rect);

				child_hWnd2 = CreateWindow(lpszClass3, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE,
					0, 0, 800, 800, hWnd, NULL, g_hInst, NULL);
				gamestate = 0;
				printf("%d\n", gamestate);
			}
		}
	}

	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	HDC mem1dc;
	HDC mem2dc;
	HWND child_hWnd;



	PAINTSTRUCT ps;
	static RECT rect;
	static bool Selection{ false };
	static int Timer1Count = 10;
	//static int Timer2Count = 0;
	static HBITMAP hBit1, hBit2, oldBit1, oldBit2;



	//static int count = 1;
	int retval;

	switch (iMessage) {

	case WM_CREATE:
		GetClientRect(hWnd, &rect);

		if (gamestate == GAME_READY)
		{
			child_hWnd = CreateWindow(lpszClass2, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE,
				0, 0, 800, 800, hWnd, NULL, g_hInst, NULL);
		}
		break;

	case WM_TIMER:
		//InvalidateRect(hWnd, NULL, TRUE);
		switch (wParam) {
		case 1:
			//if (Timer1Count > 0)
			//	Timer1Count--;
			break;
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON:
			hdc = GetDC(hWnd);


			ReleaseDC(hWnd, hdc);

			break;


		default:
			break;
		}
		break;



	case WM_KEYDOWN:
		if (gamestate == GAME_RUNNING)
		{
			if (p.isAlived) {
				if (wParam == VK_LEFT)
				{
					clientSend.keyInputDirection = MOVE_LEFT;
					retval = send(sock, (char*)&clientSend, clientSend.size, 0);
				}
				else if (wParam == VK_RIGHT) {
					clientSend.keyInputDirection = MOVE_RIGHT;
					retval = send(sock, (char*)&clientSend, clientSend.size, 0);
				}
				else if (wParam == VK_UP)
				{
					clientSend.keyInputDirection = MOVE_UP;
					retval = send(sock, (char*)&clientSend, clientSend.size, 0);
				}
				else if (wParam == VK_DOWN)
				{
					clientSend.keyInputDirection = MOVE_DOWN;
					retval = send(sock, (char*)&clientSend, clientSend.size, 0);
				}
			}
			retval = recvn(sock, (char*)&clientRecv2, sizeof(clientRecv2), 0);
			p.exhpList = clientRecv2.players[clientIndex].exhpList;
			for (int i = 0;i < 3;i++)
			{
				if (p.exhpList >= i)
					exhpList[i].isAlived=true;
			}
		}
		break;

	case WM_PAINT:
		if (gamestate == GAME_RUNNING)
		{
			static HDC hdc, MemDC, tmpDC;
			static HBITMAP BackBit, oldBackBit;
			static RECT bufferRT;

			hdc = BeginPaint(hWnd, &ps);
			GetClientRect(hWnd, &bufferRT);
			MemDC = CreateCompatibleDC(hdc);
			BackBit = CreateCompatibleBitmap(hdc, bufferRT.right, bufferRT.bottom);
			oldBackBit = (HBITMAP)SelectObject(MemDC, BackBit);
			PatBlt(MemDC, 0, 0, bufferRT.right, bufferRT.bottom, WHITENESS);
			tmpDC = hdc;
			hdc = MemDC;
			MemDC = tmpDC;



			DrawBoard(hdc, boardCount, xS, yS);
			DrawEnemy(hdc, xS, yS);
			DrawPlayer(hdc, xS, yS, clientRecv2);
			DrawObstacle(hdc, xS, yS);
			DrawItem(hdc, xS, yS);
			DrawHp(hdc, xS, yS);
			DrawExHp(hdc, xS, yS, p);


			if (!p.isAlived)
				TextOut(hdc, 350, 100, L"GAME OVER", strlen("GAME OVER"));

			TextOut(hdc, 80, 32, L"HP", strlen("HP"));
			//DrawHp(hdc, xS, yS);


			TextOut(hdc, 450, 32, L"EXTRA", strlen("EXTRA"));
			//DrawExHp(hdc, xS, yS);


			tmpDC = hdc;
			hdc = MemDC;
			MemDC = tmpDC;
			GetClientRect(hWnd, &bufferRT);
			BitBlt(hdc, 0, 0, bufferRT.right, bufferRT.bottom, MemDC, 0, 0, SRCCOPY);
			SelectObject(MemDC, oldBackBit);
			DeleteObject(BackBit);
			DeleteDC(MemDC);

			EndPaint(hWnd, &ps);

		}


		break;



	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void Update()
{

}

void UpdateHP(int cnt)
{
	if (int(cnt / 90) <= 5) {
		hpList[5 - int(cnt / 90)].isAlived = false;
		//hpNumber--;
		if (hpList[0].isAlived == false)
		{
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
		printf("ex : %d\n", p.exhpList);
	}
	
	if (!exhpList[2].isAlived && !exhpList[1].isAlived && !exhpList[0].isAlived
		&& !hpList[0].isAlived ) {
		p.isAlived = false;
	}
}

DWORD WINAPI RecvThread(LPVOID arg)
{
	HWND child_hWnd2;
	int retval = 0;
	SOCKET client_sock = (SOCKET)arg;
	while (1) {
		switch (gamestate) {
		case GAME_READY:
			retval = recvn(sock, (char*)&clientRecv2, sizeof(clientRecv2), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("recv()");
				break;
			}
			gamestate = clientRecv2.gameState;
			break;

		case GAME_RUNNING:
			retval = recvn(sock, (char*)&clientRecv2, sizeof(clientRecv2), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("recv()");
				break;
			}

			clientIndex = clientRecv2.clientIndex;
			//printf("%d\n",clientIndex);
			//p.pos = clientRecv2.players[clientRecv2.clientIndex].pos;
			//p.score = clientRecv2.players[clientRecv2.clientIndex].score;
			//cout << "SCORE : " << p.score << endl;

			for (int i = 0; i < enemyNumber; ++i)
				for (int j = 0; j < PLAYER_MAX; j++)
					if (i == clientRecv2.enemy[j])
						enemyList[i].isAlived = false;

			for (int i = 0; i < itemNumber; ++i)
				for (int j = 0; j < PLAYER_MAX; j++)
					if (i == clientRecv2.item[j]) {
						itemList[i].isAlived = false;
					}

			
			//printf(":%d\n", p.exhpList);
			//printf("::%d\n", clientRecv2.players[clientRecv2.clientIndex].exhpList);
			//if (p.hpok)
				//hpNumber++;

			gamestate = clientRecv2.gameState;
			
			break;
		case GAME_SET:
		{
			printf("asd\n");
			ExitThread(1);



		}
		}

	}

}

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
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON:
			int retval;
			WideCharToMultiByte(CP_ACP, 0, str, len, clientSend.playerID, len, NULL, NULL);
			printf("%s \n", clientSend.playerID);
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

			hThread = CreateThread(NULL, 0, RecvThread, (LPVOID)sock, 0, NULL);
			retval = send(sock, (char*)&clientSend, clientSend.size, 0);

			DestroyWindow(hWnd);
			ReleaseDC(hWnd, hdc);

			break;
		case IDC_EDIT:

			GetDlgItemText(hWnd, IDC_EDIT, str, 10);
			hdc = GetDC(hWnd);

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

	HWND child_hWnd;
	hdc = GetDC(hWnd);

	char firstP[60] = "";
	char secondP[10] = "";
	char thirdP[10] = "";
	for (int i = 0; i < 3; i++)
	{
		if (clientRecv2.players[i].rank == 1)
		{
			strcpy(firstP, clientRecv2.players[i].playerID);
		}
	}
	for (int i = 0; i < 3; i++)
	{
		if (clientRecv2.players[i].rank == 2)
		{
			strcpy(secondP, clientRecv2.players[i].playerID);
		}
	}
	for (int i = 0; i < 3; i++)
	{
		if (clientRecv2.players[i].rank == 3)
		{
			strcpy(thirdP, clientRecv2.players[i].playerID);
		}
	}
	printf("%s, %s, %s \n", firstP, secondP, thirdP);
	char first[10] = "   1st\r\n";
	char second[10] = "   2nd\r\n";
	char third[10] = "   3rd\r\n";
	int len;



	strcat(firstP, first);
	strcat(firstP, secondP);
	strcat(firstP, second);
	strcat(firstP, thirdP);
	strcat(firstP, third);

	//strcat(a, c);

	len = sizeof(firstP);

	wchar_t wtext[61];
	//wchar_t wtext2[100];
	//abc
	mbstowcs(wtext, firstP, len + 1);
	//mbstowcs(wtext2, s2, len + 1);



	switch (iMessage) {
	case WM_CREATE:
		//hButton = CreateWindow(L"button", L"DONE", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,500, 600, 100, 25, hWnd, (HMENU)IDC_BUTTON, g_hInst, NULL);
		hEdit = CreateWindow(L"edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE, 300, 200, 200, 100, hWnd, (HMENU)IDC_EDIT, g_hInst, NULL);

		SetWindowText(hEdit, wtext);
		//SetWindowText(hEdit, wtext2);

		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON:



			ReleaseDC(hWnd, hdc);
			DestroyWindow(hWnd);
			SendMessage(hWnd, WM_DESTROY, 1, 0);



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
		MoveToEx(hdc, 80 + i * oneSize + xS, 120 + 0 + yS, NULL);
		LineTo(hdc, 80 + i * oneSize + xS, 120 + boardCount * oneSize + yS);
		MoveToEx(hdc, 80 + 0 + xS, 120 + i * oneSize + yS, NULL);
		LineTo(hdc, 80 + boardCount * oneSize + xS, 120 + i * oneSize + yS);
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
	List[0].pos.x = 2;
	List[0].pos.y = 2;
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
		Rectangle(hdc, 80 + List[i].pos.x * oneSize + xS, 120 + List[i].pos.y * oneSize + yS,
			80 + List[i].pos.x * oneSize + xS + oneSize, 120 + List[i].pos.y * oneSize + yS + oneSize);
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

	//hBrush = CreateSolidBrush(RGB(255, 0, 0));
	//oBrush = (HBRUSH)SelectObject(hdc, hBrush);
	/*if (hpNumber == 1) {
		Rectangle(hdc, hpList[0].pos.x * oneSize + xS, hpList[0].pos.y * oneSize + yS,
			hpList[0].pos.x * oneSize + xS + oneSize, hpList[0].pos.y * oneSize + yS + oneSize);
	}
	else if (hpNumber == 2) {
		Rectangle(hdc, hpList[0].pos.x * oneSize + xS, hpList[0].pos.y * oneSize + yS,
			hpList[0].pos.x * oneSize + xS + oneSize, hpList[0].pos.y * oneSize + yS + oneSize);
		Rectangle(hdc, hpList[1].pos.x * oneSize + xS, hpList[1].pos.y * oneSize + yS,
		hpList[1].pos.x * oneSize + xS + oneSize, hpList[1].pos.y * oneSize + yS + oneSize);
	}
	else if (hpNumber == 3) {
		Rectangle(hdc, hpList[0].pos.x * oneSize + xS, hpList[0].pos.y * oneSize + yS,
			hpList[0].pos.x * oneSize + xS + oneSize, hpList[0].pos.y * oneSize + yS + oneSize);
		Rectangle(hdc, exhpList[1].pos.x * oneSize + xS, exhpList[1].pos.y * oneSize + yS,
			hpList[1].pos.x * oneSize + xS + oneSize, hpList[1].pos.y * oneSize + yS + oneSize);
		Rectangle(hdc, hpList[2].pos.x * oneSize + xS, hpList[2].pos.y * oneSize + yS,
			hpList[2].pos.x * oneSize + xS + oneSize, hpList[2].pos.y * oneSize + yS + oneSize);
	}
	else if (hpNumber == 4) {
		Rectangle(hdc, hpList[0].pos.x * oneSize + xS, hpList[0].pos.y * oneSize + yS,
			hpList[0].pos.x * oneSize + xS + oneSize, hpList[0].pos.y * oneSize + yS + oneSize);
		Rectangle(hdc, exhpList[1].pos.x * oneSize + xS, exhpList[1].pos.y * oneSize + yS,
			hpList[1].pos.x * oneSize + xS + oneSize, hpList[1].pos.y * oneSize + yS + oneSize);
		Rectangle(hdc, hpList[2].pos.x * oneSize + xS, hpList[2].pos.y * oneSize + yS,
			hpList[2].pos.x * oneSize + xS + oneSize, hpList[2].pos.y * oneSize + yS + oneSize);
		Rectangle(hdc, hpList[3].pos.x * oneSize + xS, hpList[3].pos.y * oneSize + yS,
			hpList[3].pos.x * oneSize + xS + oneSize, hpList[3].pos.y * oneSize + yS + oneSize);
	}
	else if (hpNumber == 5) {
		Rectangle(hdc, hpList[0].pos.x * oneSize + xS, hpList[0].pos.y * oneSize + yS,
			hpList[0].pos.x * oneSize + xS + oneSize, hpList[0].pos.y * oneSize + yS + oneSize);
		Rectangle(hdc, exhpList[1].pos.x * oneSize + xS, exhpList[1].pos.y * oneSize + yS,
			hpList[1].pos.x * oneSize + xS + oneSize, hpList[1].pos.y * oneSize + yS + oneSize);
		Rectangle(hdc, hpList[2].pos.x * oneSize + xS, hpList[2].pos.y * oneSize + yS,
			hpList[2].pos.x * oneSize + xS + oneSize, hpList[2].pos.y * oneSize + yS + oneSize);
		Rectangle(hdc, hpList[3].pos.x * oneSize + xS, hpList[3].pos.y * oneSize + yS,
			hpList[3].pos.x * oneSize + xS + oneSize, hpList[3].pos.y * oneSize + yS + oneSize);
		Rectangle(hdc, hpList[4].pos.x * oneSize + xS, hpList[4].pos.y * oneSize + yS,
			hpList[4].pos.x * oneSize + xS + oneSize, hpList[4].pos.y * oneSize + yS + oneSize);
	}
	SelectObject(hdc, oBrush);
	DeleteObject(hBrush);*/

	


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
void DrawExHp(HDC hdc, int xS, int yS, Player& p)
{
	HBRUSH hBrush, oBrush;
	int i;


	//if (exhpList[i].isAlived) {
	hBrush = CreateSolidBrush(RGB(255, 0, 0));
	oBrush = (HBRUSH)SelectObject(hdc, hBrush);
	if (p.exhpList == 1) {
		Rectangle(hdc, exhpList[0].pos.x * oneSize + xS, exhpList[0].pos.y * oneSize + yS,
			exhpList[0].pos.x * oneSize + xS + oneSize, exhpList[0].pos.y * oneSize + yS + oneSize);
	}
	else if (p.exhpList == 2) {
		Rectangle(hdc, exhpList[0].pos.x * oneSize + xS, exhpList[0].pos.y * oneSize + yS,
			exhpList[0].pos.x * oneSize + xS + oneSize, exhpList[0].pos.y * oneSize + yS + oneSize);
		Rectangle(hdc, exhpList[1].pos.x * oneSize + xS, exhpList[1].pos.y * oneSize + yS,
			exhpList[1].pos.x * oneSize + xS + oneSize, exhpList[1].pos.y * oneSize + yS + oneSize);
	}
	else if (p.exhpList == 3) {
		Rectangle(hdc, exhpList[0].pos.x * oneSize + xS, exhpList[0].pos.y * oneSize + yS,
			exhpList[0].pos.x * oneSize + xS + oneSize, exhpList[0].pos.y * oneSize + yS + oneSize);
		Rectangle(hdc, exhpList[1].pos.x * oneSize + xS, exhpList[1].pos.y * oneSize + yS,
			exhpList[1].pos.x * oneSize + xS + oneSize, exhpList[1].pos.y * oneSize + yS + oneSize);
		Rectangle(hdc, exhpList[2].pos.x * oneSize + xS, exhpList[2].pos.y * oneSize + yS,
			exhpList[2].pos.x * oneSize + xS + oneSize, exhpList[2].pos.y * oneSize + yS + oneSize);
	}
	SelectObject(hdc, oBrush);
	DeleteObject(hBrush);
	//}

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