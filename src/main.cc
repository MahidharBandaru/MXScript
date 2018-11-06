#include "Interpreter.hh"

#include <fstream>
#include <iterator>
#include <sstream>

#define LOG(x) std::cout << x << std::endl;

int main(int argc, char** argv) {
    if (argc < 2)
    {
        LOG("Enter the file name");
        std::exit(EXIT_FAILURE);        
    }

    std::ifstream fs (argv[1]);
    Interpreter i;

    std::string str(static_cast<std::stringstream const&>(std::stringstream() << fs.rdbuf()).str()); 

    std::string s (str.begin(), str.end());
    
    i.Evaluate(s);

    return 0;    
}