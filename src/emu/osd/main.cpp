
#include "emu/core.hpp"
#include "emu/osd/socket.h"

#if __WIN32__
static WSADATA wsaData;
#endif

int main(int argc, char **argv)
{
#if __WIN32__
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Winsock 2 API Error Code: %d", WSAGetLastError());
		return 1;
	}
#endif

	printf("Welcome to Multi-System Emulator System\n\n");

	cliPrompt();
	return 0;
}
