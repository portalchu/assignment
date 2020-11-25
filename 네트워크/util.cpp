#include "over1.h"

#define Lock(a) EnterCriticalSection(&a)
#define Unlock(a) LeaveCriticalSection(&a)

//ctor
Session::Session(LPServer ser, SOCKET s, SOCKADDR_IN* addr)
{
	sock = s;
	event = WSACreateEvent();
	swbuf.buf = sbuf;
	rwbuf.buf = rbuf;
	CopyMemory(&caddr, addr, sizeof(caddr));
	server = ser;
}

bool Session::PacketRecv()
{
	//1. WSAOVERLAPPED init
	memset(&over, 0, sizeof(over)); // zero initialize
	//2. WSAOVERLAPPED hEvent <- EVENT object handle
	over.hEvent = event;

	//3. WSABUF : rwbuf 에서 몇바이트 수신할 것인가를 결정
	rwbuf.len = MAXBUF - 1;

	//4. Handler : 함수 포인터 결정 (수신후 처리함수 결정)
	handle = &Session::ReadProcess;

	//5. WSARecv 호출
	DWORD cbt;
	DWORD flag = 0;
	int ret = WSARecv(sock, &rwbuf, 1, &cbt, &flag, &over, NULL);
	if (ret == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSA_IO_PENDING)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool Session::PacketSend(DWORD cb)
{
	//1. WSAOVERLAPPED init
	memset(&over, 0, sizeof(over)); // zero initialize
	//2. WSAOVERLAPPED hEvent <- EVENT object handle
	over.hEvent = event;

	//3. WSABUF : swbuf 에서 몇바이트 송신할 것인가를 결정
	swbuf.len = cb;

	//4. Handler : 함수 포인터 결정 (송신후 처리함수 결정)
	handle = &Session::WriteProcess;

	//5. WSASend호출
	DWORD cbt;
	int ret = WSASend(sock, &swbuf, 1, &cbt, 0, &over, NULL);
	if (ret == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSA_IO_PENDING)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return true;
}

//수신 후, 처리
void Session::ReadProcess(DWORD cbt)
{
	if (cbt == 0)
	{
		cout << "Graceful Closing case" << endl;
		server->CloseSession(this);
		return;
	}

	rbuf[cbt] = '\0';
	cout << "[Client]" << rbuf << endl;
	CopyMemory(sbuf, rbuf, cbt); //수신버퍼의 내용을 송신버퍼에 복사
	PacketSend(cbt); //에코서비스
}

//송신 후, 처리
void Session::WriteProcess(DWORD cbt)
{
	PacketRecv();
}

Server::Server(unsigned short port)
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2,2), &wsa))
	{
		cout << "WSAStart up error" << endl;
		exit(-1);
	}

	listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listen_sock == INVALID_SOCKET)
	{
		cout << "socket error" << endl;
		exit(-1);
	}

	SOCKADDR_IN saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(listen_sock, (SOCKADDR*)&saddr,sizeof(saddr)))
	{
		cout << "bind error" << endl;
		exit(-1);
	}

	if (listen(listen_sock, SOMAXCONN))
	{
		cout << "listen error" << endl;
		exit(-1);
	}

	InitializeCriticalSection(&cs);
	noc = 0;
}

LPSession Server::AddSession(LPSession ses)
{
	Lock(cs);
	sarr.push_back(ses);
	earr.push_back(ses->event);
	noc++;
	Unlock(cs);
	return ses;
}

void Server::CloseSession(LPSession ses)
{
	SOCKET s = ses->sock;
	WSAEVENT e = ses->event;

	Lock(cs);
	earr.erase(std::find(earr.begin(), earr.end(), ses->event));
	sarr.erase(std::find(sarr.begin(), sarr.end(), ses));
	noc--;
	Unlock(cs);

	closesocket(s);
	WSACloseEvent(e);
	delete ses;
}