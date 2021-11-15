
#include "server.h"


#include <vector>
using namespace std;
#define SERVERPORT 9000
#define BUFSIZE    4096

int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;
	
	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
		{

			return SOCKET_ERROR;

		}
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}
void err_quit(char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
    LocalFree(lpMsgBuf);
    exit(1);
}
void err_display(char* msg)
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



Enemy enemyList[35];
Wall List[36];
Item itemList[4];
Player player[3];
sc_recv_struct first;
sc_send_struct second;


void InitWall()
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
void MovePlayer(int key, Player& p)
{
	printf("%s \n", p.playerID);
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
		CheckPlayerByPlayerCollision(key, p);
		CheckPlayerByWallCollision(key, p);
		CheckPlayerByEnemyCollision(p);
		CheckPlayerByItemCollision(p);


	}

}
void CheckPlayerByWallCollision(int key, Player& p)
{
	switch (key)
	{
	case MOVE_LEFT:
		for (int i = 0;i < 36;i++)
		{
			if (List[i].pos.x == p.pos.x && List[i].pos.y == p.pos.y)
			{
				p.pos.x++;
				break;
			}
		}
		break;
	case MOVE_RIGHT:
		for (int i = 0;i < 36;i++)
		{
			if (List[i].pos.x == p.pos.x && List[i].pos.y == p.pos.y)
			{
				p.pos.x--;
				break;
			}
		}
		break;
	case MOVE_UP:
		for (int i = 0;i < 36;i++)
		{
			if (List[i].pos.x == p.pos.x && List[i].pos.y == p.pos.y)
			{
				p.pos.y++;
				break;
			}
		}
		break;
	case MOVE_DOWN:
		for (int i = 0;i < 36;i++)
		{
			if (List[i].pos.x == p.pos.x && List[i].pos.y == p.pos.y)
			{
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
	for (int i = 0;i < 35;i++)
		if (p.pos.x == enemyList[i].pos.x && p.pos.y == enemyList[i].pos.y)
		{
			enemyList[i].isAlived = false;
			printf("%d 충돌\n",i);
			break;
		}

}
void CheckPlayerByItemCollision(Player& p)
{
	for (int i = 0;i < 4;i++)
		if (p.pos.x == itemList[i].pos.x && p.pos.y == itemList[i].pos.y)
		{
			itemList[i].isAlived = false;
			printf("%d 충돌\n", i);
			break;
		}

}
void CheckPlayerByPlayerCollision(int key,Player& p)
{

		switch (key)
		{
		case MOVE_LEFT:
			for (int i = 1;i < 3;i++)
			{

				if (player[i].pos.x == p.pos.x && player[i].pos.y == p.pos.y)
				{
					//if(p==player[i])
					p.pos.x++;
					printf("플레이어 충돌\n");

					break;
				}
			}
			break;
		case MOVE_RIGHT:
			for (int i = 1;i < 3;i++)
			{
				if (player[i].pos.x == p.pos.x && player[i].pos.y == p.pos.y)
				{
					p.pos.x--;
					printf("플레이어 충돌\n");
					break;
				}
			}
			break;
		case MOVE_UP:
			for (int i = 1;i < 3;i++)
			{
				if (player[i].pos.x == p.pos.x && player[i].pos.y == p.pos.y)
				{
					p.pos.y++;
					printf("플레이어 충돌\n");
					break;
				}
			}
			break;
		case MOVE_DOWN:
			for (int i = 1;i < 3;i++)
			{
				if (player[i].pos.x == p.pos.x && player[i].pos.y == p.pos.y)
				{
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
// Client_Thread

DWORD WINAPI ProcessClient(LPVOID arg) {
	SOCKET client_sock = (SOCKET)arg;
	SOCKADDR_IN clientaddr;
	int addrlen;
	int fileNameSize;
	int fileSize;
	int retval;

	


	// 클라이언트 정보
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

	while (1) {
		retval = recvn(client_sock, (char*)&first, first.size, 0);
		if (retval == 0) {
			break;
		}
		strcpy(player[0].playerID, first.playerID);
		
		


		MovePlayer(first.keyInputDirection,player[0]);




		second.players->pos = player->pos;
		second.enemyList->isAlived = enemyList->isAlived;
		second.itemList->isAlived = itemList->isAlived;
		printf("%d\n", enemyList[0].isAlived);


		second.gameState = 10;
		
		retval = send(client_sock, (char*)&second, second.size, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}

	}

	closesocket(client_sock);

	return 0;
}

int main() {
	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

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

	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE hThread;


	

	// 임계 영역 초기화
	InitializeCriticalSection(&cs);
	InitEnemy();
	InitWall();
	InitItem();
	player->pos.x = 0;
	player->pos.y = 0;
	while (1) {
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
	}

	// 임계 영역 종료
	DeleteCriticalSection(&cs);

	closesocket(listen_sock);

	WSACleanup();
	return 0;
}

