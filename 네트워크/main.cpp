#pragma comment (lib,"ws2_32.lib")
#include "over1.h"
#include <process.h>

using std::cout;
using std::endl;

#define MAXBUF 80

void main()
{
	Server s(8000);
	_beginthreadex(NULL, 0, &ProActor, (LPVOID*)&s, 0, NULL);

	SOCKET csock;
	SOCKADDR_IN caddr;
	int addrlen = sizeof(caddr);

	LPSession ses;
	while (1)
	{
		csock = accept(s.listen_sock, (SOCKADDR*)&caddr, &addrlen);
		if (csock == INVALID_SOCKET)
		{
			cout << "accept error" << endl;
			return -1;
		}
		//Session 생성 후 바로 서버의 AddSession 함수 실행
		ses = s.AddSession(new Session(&s, csock, &caddr));
		ses->PacketRecv();


	}


}
