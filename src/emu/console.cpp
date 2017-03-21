
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
: cdev(dev), sdev(nullptr)
{
}

Console::~Console()
{
}

int Console::executeCommand(args_t &args)
{
	const Command *cmds;

	// Process commands by using current system device
	if (sdev != nullptr && (cmds = sdev->getCommandTable()) != nullptr) {
		for (int idx = 0; cmds[idx].name; idx++) {
//    		std::cout << "Command: " << cmd.name << std::endl;
			if (cmds[idx].name == args[0])
				return cmds[idx].execute(this, sdev, args);
		}
	}

	// Process commands by using console device
	if ((cmds = cdev->getCommandTable()) != nullptr) {
		for (int idx = 0; cmds[idx].name; idx++) {
//    		std::cout << "Command: " << cmd.name << std::endl;
			if (cmds[idx].name == args[0])
				return cmds[idx].execute(this, cdev, args);
		}
	}

	// command not found as default
	return CMD_NOTFOUND;
}

void Console::prompt()
{
	std::string cmdLine;
    args_t args;
    const Command *cmds;
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

    	rc = executeCommand(args);
    	if (rc == CMD_NOTFOUND) {
    		std::cerr << cdev->getName() <<  ": Unknown command " << args[0] << std::endl;
    		rc = CMD_OK;
    	}
    }
}
