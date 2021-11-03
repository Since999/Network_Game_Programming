#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE    4096
struct Position {
    float x, y;
};
class Player {
public:
    Position pos;
    bool isAlived;
    int hp;
    int score;
    int clientIndex;
    int rank;

};
struct sc_recv_struct {
    int keyInputDirection;
    char playerID[10];

};
struct sc_send_struct {
    Player player[3];
    int gameState;

};
HANDLE ProcessEvent1;
HANDLE ProcessEvent2;
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
void cur(short x, short y) {
    COORD pos = { x, y }; // COORD xy가지는 구조체

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

}


bool check12 = true;
float progress1 = 0;
float progress2 = 0;

int ThID = 0;


DWORD WINAPI ProcessClient1(LPVOID arg)
{
    sc_recv_struct first;
    sc_send_struct second;
    SOCKET client_sock = (SOCKET)arg;
    int retval;
    int wa;
    SOCKADDR_IN clientaddr;
    int addrlen;
    char buf[BUFSIZE + 1];
    while (1)
    {
        wa = WaitForSingleObject(ProcessEvent1, INFINITE);
        if (wa == WAIT_OBJECT_0) break;
           
    }
    
    

    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

 
        while (1) {
            retval = recvn(client_sock, (char*)&first,sizeof(sc_recv_struct), 0);
            if (retval == 0) {
                break;
            }
            else {

               
                    printf("key = %d , ID = %s :  \n",first.keyInputDirection,first.playerID);

                    SetEvent(ProcessEvent2);

            }
            Position pos;
            pos.x = float(first.keyInputDirection);
            pos.y= -float(first.keyInputDirection);
            second.player[0].pos = pos;
            second.gameState = 10;
            retval = send(client_sock, (char*)&second, sizeof(sc_send_struct), 0);
            if (retval == SOCKET_ERROR) {
                err_display("send()");
                break;
            }
            else {


                printf("send! \n");

                SetEvent(ProcessEvent2);

            }

        }




    
   

    closesocket(client_sock);

    return 0;
}

DWORD WINAPI ProcessClient2(LPVOID arg)
{

    SOCKET client_sock = (SOCKET)arg;
    int retval;
    int wa;
    SOCKADDR_IN clientaddr;
    int addrlen;
    char buf[BUFSIZE + 1];
    while (1)
    {
        wa = WaitForSingleObject(ProcessEvent2, INFINITE);
        if (wa == WAIT_OBJECT_0)break;
     
    }

    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

    while (1) {





        char filename[256];
        ZeroMemory(filename, 256);

        retval = recvn(client_sock, (char*)&filename, 50, 0);
        if (retval == SOCKET_ERROR) {

            err_display("recv()");
            break;
        }
        else if (retval == 0)
            break;
        /*
        printf("받을 파일 이름 : %s\n", filename);*/

        int totalbytes;
        recvn(client_sock, (char*)&totalbytes, sizeof(totalbytes), 0);

        /*
        printf("받을 파일 크기 : %d 바이트\n", totalbytes);*/


        FILE* fp = fopen(filename, "wb");

        int numtotal = 0;
        float progress = 0;

        int Bufsize = totalbytes / 100000;

        while (1) {
            retval = recvn(client_sock, buf, Bufsize, 0);


            if (retval == 0) {
                break;
            }
            else {

                fwrite(buf, 1, retval, fp);


                numtotal += retval;
                progress = (float)numtotal / (float)totalbytes * 100;

               
                    progress2 = progress;
                    cur(0, 0);
                    printf("prograss : %.1f  ,  %.1f  \n", progress1, progress2);
                
          
                    SetEvent(ProcessEvent1);

            }

        }




    }
    

    closesocket(client_sock);

    return 0;
}

    /*ProcessEvent1 = CreateEvent(NULL, FALSE, FALSE, NULL);
       if (ProcessEvent1 == NULL) return 1;
       ProcessEvent2 = CreateEvent(NULL, FALSE, TRUE, NULL);
       if (ProcessEvent2 == NULL) return 1;*/

       /*WaitForMultipleObjects(2, &hThread, TRUE, INFINITE);
       CloseHandle(ProcessEvent1);
       CloseHandle(ProcessEvent2);*/
int main(int argc, char* argv[])
{
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
    HANDLE hThread[2];
    ProcessEvent1 = CreateEvent(NULL, FALSE, TRUE, NULL);
    if (ProcessEvent1 == NULL) return 1;
    ProcessEvent2 = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (ProcessEvent2 == NULL) return 1;
    //SetEvent(ProcessEvent1);
    
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
            
        }

       
        // 스레드 생성
        hThread[0] = CreateThread(NULL, 0, ProcessClient1,
            (LPVOID)client_sock, 0, NULL);




        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");

        }


        // 스레드 생성
        hThread[1] = CreateThread(NULL, 0, ProcessClient2,
            (LPVOID)client_sock, 0, NULL);

       
    
    WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
    CloseHandle(ProcessEvent1);
    CloseHandle(ProcessEvent2);
    closesocket(listen_sock);

    WSACleanup();
    return 0;
}
