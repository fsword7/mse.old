//
//  commands.cpp
//  mse
//
//  Created by Timothy Stark on 10/28/16.
//  Copyright © 2016 Timothy Stark. All rights reserved.
//

#include "emu/core.h"
#include "emu/console.h"

int cmdShutdown()
{
	// Shutdown system
	return CMD_SHUTDOWN;
}

Command cmdGeneral[2] = {
	{ "exit", "", cmdShutdown },
	{ "quit", "", cmdShutdown }
};
