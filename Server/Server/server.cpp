#include "server.h"

int main() 
{
	//hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	//if (hReadEvent == NULL) return 1;
	//hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	//if (hWriteEvent == NULL) return 1;

	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 0;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);

	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	InitializeCriticalSection(&cs);

	while (clientCnt < 3) {
		addrlen = sizeof(clientaddr);
		client_sock[clientCnt] = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock[clientCnt] == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock[clientCnt], 0, NULL);
		if (hThread == NULL) { closesocket(client_sock[clientCnt]); }
		else { CloseHandle(hThread); }

	}

	DeleteCriticalSection(&cs);

	//CloseHandle(hReadEvent);
	//CloseHandle(hWriteEvent);
		
	closesocket(listen_sock);
	WSACleanup();
	return 0;
}

// Client_Thread
DWORD WINAPI ProcessClient(LPVOID arg)
{
	int retval = 0;
	SOCKET client_sock = (SOCKET)arg;
	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);
	
	EnterCriticalSection(&cs);
	int clientIndex = clientCnt;
	clientCnt++;
	LeaveCriticalSection(&cs);

	rData = RecvData(client_sock, rData);
	sData.players[clientIndex].clientIndex = clientIndex;
	strcpy(sData.players[clientIndex].playerID, rData.playerID);
	for (int i = 0; i < PLAYER_MAX; i++)
		cout << sData.players[i].playerID << endl;	
	SendData(sData);																// 인식할 수 있는 ClientID 부여
	
	
	while (1) {
		if (gameState == GAME_RUNNING) {
			RecvData(client_sock, rData);											// client의 키입력 recv
			
			MovePlayer(rData.keyInputDirection, player[clientIndex]);
			//UpdatePlayer(player);
			sData.players[clientIndex].pos = player[clientIndex].pos;

			for (int i = 0; i < 35; i++)
				sData.enemyList[i].isAlived = enemyList[i].isAlived;

			SendData(sData);
		}

	}

	closesocket(client_sock);
	return 0;
}

void SendData(sc_send_struct s_data)
{
	int retval = 0;
	char buf[BUFSIZE];

	for (int i = 0; i < PLAYER_MAX; i++) {
		retval = send(client_sock[i], (char*)&s_data, sizeof(s_data), 0);
		if (retval == SOCKET_ERROR) {
			err_display("SC_Send_data() ");
			return;
		}
	}
}

sc_recv_struct RecvData(SOCKET sock, sc_recv_struct r_data)
{
	int retval = 0;
	retval = recvn(sock, (char*)&r_data, sizeof(r_data), 0);
	if (retval == SOCKET_ERROR) {
		err_display("SC_Recv_data() ");
	}
		return r_data;
}

template <typename T>
void SetPos(T gameObject, int x, int y)
{
	gameObject.Pos.x = x;
	gameObject.Pos.y = y;
}

void MovePlayer(int key, Player& p)
{
	if (key == MOVE_LEFT)
	{
		if (p.pos.x > 0)
			p.pos.x--;
		CheckPlayerByWallCollision(key, p);
		CheckPlayerByEnemyCollision(p);
		CheckPlayerByItemCollision(p);
		CheckPlayerByPlayerCollision(key, p);
	}
	else if (key == MOVE_RIGHT)
	{
		if (p.pos.x < 11)
			p.pos.x++;
		CheckPlayerByWallCollision(key, p);
		CheckPlayerByEnemyCollision(p);
		CheckPlayerByItemCollision(p);
		CheckPlayerByPlayerCollision(key, p);
	}
	else if (key == MOVE_UP)
	{
		if (p.pos.y > 0)
			p.pos.y--;
		CheckPlayerByWallCollision(key, p);
		CheckPlayerByEnemyCollision(p);
		CheckPlayerByItemCollision(p);
		CheckPlayerByPlayerCollision(key, p);
	}
	else if (key == MOVE_DOWN)
	{
		if (p.pos.y < 11)
			p.pos.y++;
		CheckPlayerByWallCollision(key, p);
		CheckPlayerByEnemyCollision(p);
		CheckPlayerByItemCollision(p);
		CheckPlayerByPlayerCollision(key, p);
	}

}

void CheckPlayerByWallCollision(int key, Player& p)
{
	switch (key) {
	case MOVE_LEFT:
		for (int i = 0; i < 35; i++) {
			if (wallList[i].pos.x == p.pos.x && wallList[i].pos.y == p.pos.y) {
				p.pos.x++;
				break;
			}
		}
		break;

	case MOVE_RIGHT:
		for (int i = 0; i < 35; i++) {
			if (wallList[i].pos.x == p.pos.x && wallList[i].pos.y == p.pos.y) {
				p.pos.x--;
				break;
			}
		}
		break;

	case MOVE_UP:
		for (int i = 0; i < 35; i++) {
			if (wallList[i].pos.x == p.pos.x && wallList[i].pos.y == p.pos.y) {
				p.pos.y++;
				break;
			}
		}
		break;

	case MOVE_DOWN:
		for (int i = 0; i < 35; i++) {
			if (wallList[i].pos.x == p.pos.x && wallList[i].pos.y == p.pos.y) {
				p.pos.y--;
				break;
			}
		}
		break;

	default:
		break;
	}

}

void CheckPlayerByEnemyCollision(Player& p)
{
	for (int i = 0; i < 35; i++) {
		if (p.pos.x == enemyList[i].pos.x && p.pos.y == enemyList[i].pos.y) {
			enemyList[i].isAlived = false;
			printf("%d 적 충돌\n", i);
		}
	}
}

void CheckPlayerByItemCollision(Player& p)
{
	for (int i = 0; i < 4; i++) {
		if (p.pos.x == itemList[i].pos.x && p.pos.y == itemList[i].pos.y) {
			itemList[i].isAlived = false;
			printf("%d 아이템 충돌\n", i);
		}
	}
}

void CheckPlayerByPlayerCollision(int key, Player& p)
{
	switch (key) {
	case MOVE_LEFT:
		for (int i = 1; i < PLAYER_MAX; i++) {
			if (player[i].pos.x == p.pos.x && player[i].pos.y == p.pos.y) {
				//if(p==player[i])
				p.pos.x++;
				printf("플레이어 충돌\n");
				break;
			}
		}
		break;

	case MOVE_RIGHT:
		for (int i = 1; i < PLAYER_MAX; i++) {
			if (player[i].pos.x == p.pos.x && player[i].pos.y == p.pos.y) {
				p.pos.x--;
				printf("플레이어 충돌\n");
				break;
			}
		}
		break;

	case MOVE_UP:
		for (int i = 1; i < PLAYER_MAX; i++) {
			if (player[i].pos.x == p.pos.x && player[i].pos.y == p.pos.y) {
				p.pos.y++;
				printf("플레이어 충돌\n");
				break;
			}
		}
		break;

	case MOVE_DOWN:
		for (int i = 1; i < PLAYER_MAX; i++) {
			if (player[i].pos.x == p.pos.x && player[i].pos.y == p.pos.y) {
				p.pos.y--;
				printf("플레이어 충돌\n");
				break;
			}
		}
		break;

	default:
		break;
	}
}

void MakeRank()
{
	for (int i = 0; i < PLAYER_MAX; i++) {
		for (int j = 0; j < PLAYER_MAX - 1 ; j++)	{
			if (player[i].score > player[j].score)
				player[i].rank--;
		}
	}
}

int isGameOver(Player p[])
{
	int cnt = 0;
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (p[i].hp <= 0)
			cnt++;
	}
	if (cnt == 3)
		return GAME_SET;
	else
		return GAME_RUNNING;
}

void UpdatePlayer(Player p[])
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		sData.players[i].pos = p[i].pos;
	}
}

void err_quit(const char* msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void err_display(const char* msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int recvn(SOCKET s, char* buf, int len, int flags) {
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