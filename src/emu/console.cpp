
#include "emu/core.h"

void cliPrompt()
{
	char line[256];

	printf("mse> ");
	fflush(stdout);

	scanf("%s", line);
	printf("Entered: %s\n", line);
	printf("Done..\n");
}
