
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

void Console::prompt()
{
    std::string cmdLine;
    std::vector<std::string> args;
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

    	// Process commands by using current device
    	cmds = cdev->getCommandTable();
    	if (cmds != nullptr) {
    		rc = CMD_NOTFOUND;
    		for (int idx = 0; cmds[idx].name; idx++) {
//    			std::cout << "Command: " << cmd.name << std::endl;
    			if (cmds[idx].name == args[0]) {
    				rc = cmds[idx].execute(this, cdev, args);
    				break;
    			}
    		}
        	if (rc == CMD_NOTFOUND) {
        		std::cerr << cdev->getName() <<  ": Unknown command " << args[0] << std::endl;
        		rc = CMD_OK;
        	}
    	} else
       		std::cerr << cdev->getName() << ": Command table not found." << std::endl;
    }
}
