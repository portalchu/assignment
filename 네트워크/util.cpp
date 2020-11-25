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

	//3. WSABUF : rwbuf ���� �����Ʈ ������ ���ΰ��� ����
	rwbuf.len = MAXBUF - 1;

	//4. Handler : �Լ� ������ ���� (������ ó���Լ� ����)
	handle = &Session::ReadProcess;

	//5. WSARecv ȣ��
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

	//3. WSABUF : swbuf ���� �����Ʈ �۽��� ���ΰ��� ����
	swbuf.len = cb;

	//4. Handler : �Լ� ������ ���� (�۽��� ó���Լ� ����)
	handle = &Session::WriteProcess;

	//5. WSASendȣ��
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

//���� ��, ó��
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
	CopyMemory(sbuf, rbuf, cbt); //���Ź����� ������ �۽Ź��ۿ� ����
	PacketSend(cbt); //���ڼ���
}

//�۽� ��, ó��
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