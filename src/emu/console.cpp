
#include "emu/core.h"
#include "emu/console.h"

int cmdShutdown(Console *, args_t &)
{
	// Shutdown system
	return CMD_SHUTDOWN;
}

// General commands table
Command cmdGeneral[3] = {
	{ "create", "", cmdCreate },
	{ "exit", "", cmdShutdown },
	{ "quit", "", cmdShutdown }
};

std::vector<std::string> split(std::string const &line)
{
    std::istringstream buffer(line);
    std::vector<std::string> ret;
    
    std::copy(std::istream_iterator<std::string>(buffer),
              std::istream_iterator<std::string>(),
              std::back_inserter(ret));
    return ret;
}

Console::Console(appCore *_app)
: app(_app)
{
}

Console::~Console()
{
}

void Console::prompt()
{
    std::string cmdLine;
    std::vector<std::string> args;
    int rc = CMD_OK;
    
    while(rc == CMD_OK) {
    	std::cout << "mse> ";
    	std::getline(std::cin, cmdLine);
//    	std::cout << "Entered line: " << cmdLine << std::endl;
    
    	args = split(cmdLine);
//    	for (auto &&word : args)
//    		printf("Arg: %s\n", word.c_str());
    
    	if (args.size() == 0)
    		continue;

    	rc = CMD_NOTFOUND; // default
    	for (auto &&cmd : cmdGeneral) {
//    		std::cout << "Command: " << cmd.name << std::endl;
    		if (cmd.name == args[0]) {
    			rc = cmd.execute(this, args);
    			continue;
    		}
    	}

    	// Command not found
    	if (rc == CMD_NOTFOUND) {
    		std::cout << "Command '" << args[0] << "': Not found" << std::endl;
    		rc = CMD_OK;
    	}
    };
}
