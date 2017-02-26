
#include <emu/core.h>

std::vector<std::string> split(std::string const &line)
{
    std::istringstream buffer(line);
    std::vector<std::string> ret;
    
    std::copy(std::istream_iterator<std::string>(buffer),
              std::istream_iterator<std::string>(),
              std::back_inserter(ret));
    return ret;
}

void cliPrompt()
{
    std::string cmdLine;
    std::vector<std::string> args;
    
    std::cout << "mse> ";
    std::getline(std::cin, cmdLine);
    std::cout << "Entered line: " << cmdLine << std::endl;
    
    args = split(cmdLine);
    for (auto &&word : args)
        printf("Arg: %s\n", word.c_str());
    
    printf("Done..\n");
}
