
#include "emu/emucore.h"
#include "emu/commands.h"

std::vector<std::string> split(std::string const &line)
{
    std::istringstream buffer(line);
    std::vector<std::string> ret;
    
    std::copy(std::istream_iterator<std::string>(buffer),
              std::istream_iterator<std::string>(),
              std::back_inserter(ret));
    return ret;
}

console_base::console_base()
//: cdev(dev), sdev(nullptr)
{
}

console_base::~console_base()
{
}


void console_base::printf(const char *format, ...) const
{
	char     out[2048];
	va_list  args;

	va_start(args, format);

	vsprintf(out, format, args);
	fflush(stdout);

	std::cout << out;

	// All done, release arguments.
	va_end(args);
}

void console_base::prompt()
{
	command_handler cmd;
	std::string cmdLine;
    args_t args;
    cmdStatus st = cmdOk;
    
    while(st == cmdOk) {
    	std::cout << "mse> ";
    	std::getline(std::cin, cmdLine);
//    	std::cout << "Entered line: " << cmdLine << std::endl;
    
    	st = cmd.execute(this, cmdLine);
    	if (st == cmdInvalid) {
    		std::cerr << "Invalid command" << std::endl;
    		st = cmdOk;
    	}
    }
}

void console_base::script(std::string fname)
{
//	std::ifstream in;
//	std::string cmdLine;
//	args_t args;
//	int    lineno = 0;
//	int    rc;

//	in.exceptions(std::ifstream::failbit | std::ifstream::badbit);

//	try {
//		in.open(fname);
//
//		while(!in.eof()) {
//			std::getline(in, cmdLine);
//			lineno++;
//
//			if (cmdLine[0] == '#' || cmdLine[0] == ';')
//				continue;
//
//			args = split(cmdLine);
//			if (args.size() == 0)
//				continue;
//
//			rc = executeCommand(args);
//			if (rc != CMD_OK) {
//				std::cout.flush();
//				std::cerr << lineno << ": " << cmdLine << std::endl;
//
//				switch (rc) {
//				case CMD_NOTFOUND:
//					std::cerr << cdev->getName() << ": Unknown command "
//							  << args[0] << " at line " << lineno << std::endl;
//					break;
//
//				default:
//					std::cerr << cdev->getName() << ": Command error at line "
//							  << lineno << std::endl;
//					break;
//				}
//			}
//		}
//
//		in.close();
//	}
//
//	catch (std::system_error& e) {
//		std::cout.flush();
//		std::cerr << "File '" << fname << "' error: " << e.code().message() << std::endl;
//		return;
//	}
}
