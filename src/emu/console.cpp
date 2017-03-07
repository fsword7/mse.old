
#include "emu/core.h"
#include "emu/devsys.h"
#include "emu/console.h"

std::vector<std::string> split(std::string const &line)
{
    std::istringstream buffer(line);
    std::vector<std::string> ret;
    
    std::copy(std::istream_iterator<std::string>(buffer),
              std::istream_iterator<std::string>(),
              std::back_inserter(ret));
    return ret;
}

Console::Console(Device *dev)
: root(dev)
{
}

Console::~Console()
{
}

void Console::prompt()
{
    std::string cmdLine;
    std::vector<std::string> args;
    Driver *drv;
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

    	// Get driver table for commands
    	drv = root->getDriver();
    	rc = CMD_NOTFOUND; // default

    	// Process commands by using current device
    	for (int idx = 0; drv->Commands[idx].name; idx++) {
    		const Command *cmd = &drv->Commands[idx];
//    		std::cout << "Command: " << cmd.name << std::endl;
    		if (cmd->name == args[0]) {
    			rc = cmd->execute(this, root, args);
    			continue;
    		}
    	}

    	// Command not found
    	if (rc == CMD_NOTFOUND) {
    		std::cout << drv->drvName <<  ": Unknown command " << args[0] << std::endl;
    		rc = CMD_OK;
    	}
    };
}
