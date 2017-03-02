
#include "emu/core.h"
#include "emu/console.h"
#include "emu/osd/socket.h"

#if __WIN32__
static WSADATA wsaData;
#endif

void osdExit(std::string reason)
{
	std::cout << "System Shutdown..." << std::endl;
	exit(0);
}

int main(int argc, char **argv)
{
#if __WIN32__
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Winsock 2 API Error Code: %d", WSAGetLastError());
		return 1;
	}
#endif
	Console *con = new Console();

	printf("Welcome to Multi-System Emulator System\n\n");

	con->prompt();
	osdExit("");
	return 0;
}
