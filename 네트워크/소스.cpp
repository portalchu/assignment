#pragma comment (lib,"ws2_32.lib")

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <iostream>

using std::cout;
using std::endl;

#define MAXBUF 80

void err_display(const char *);

int insert_addr(const char *, PVOID);
void Print_IP(PVOID);
void main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET s_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s_sock == INVALID_SOCKET)
	{
		err_display("socket error");
		return;
	}

	SOCKADDR_IN saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(s_sock, (SOCKADDR *)&saddr, sizeof(saddr)))
	{
		err_display("bind error");
		return;
	}

	if (listen(s_sock, SOMAXCONN))
	{
		err_display("listen error");
		return;
	}

	char combuf[MAXBUF];
	int recvlen;
	SOCKADDR_IN  caddr;
	int addrlen = sizeof(caddr);

	SOCKET c_sock = accept(s_sock, (SOCKADDR *)&caddr, &addrlen);
	if (c_sock == INVALID_SOCKET)
	{
		err_display("accept error");
		return;
	}

	Print_IP(&caddr.sin_addr);
	cout << "(" << ntohs(caddr.sin_port) << ") Client Connection Success" << endl;
	while (1)
	{
		recvlen = recv(c_sock, combuf, MAXBUF, 0);
		if (recvlen == 0)
		{
			cout << "Normal Good-bye client case" << endl;
			break;
		}
		if (recvlen == SOCKET_ERROR)
		{
			err_display("recv error");
			break;
		}
		combuf[recvlen] = '\0';
		cout << "[From Client] " << combuf << endl;
		send(c_sock, combuf, recvlen, 0);
	}

	closesocket(s_sock);
	WSACleanup();

}

void Print_IP(PVOID a)
{
	char IPaddr[MAXBUF];
	if (!InetNtop(AF_INET, a, IPaddr, MAXBUF))
	{
		err_display("Print_IP error");
		return;
	}

	cout << "[" << IPaddr << "]";
}

int insert_addr(const char *ipstr, PVOID a)
{
	if (InetPton(AF_INET, (PCSTR)ipstr, a) != 1)
	{
		err_display("insert_addr error");
		return -1;
	}
	return 0;
}

void err_display(const char *mes)
{
	char * ebuf;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&ebuf, 0, NULL);
	cout << "[" << mes << "] " << ebuf << endl;
	LocalFree(ebuf);
}

