#include "Interpreter.hh"

#include <fstream>

#define LOG(x) std::cout << x << std::endl;

int main(int argc, char** argv) {
    if (argc < 2)
    {
        LOG("Enter the file name");
        std::exit(EXIT_FAILURE);        
    }

    std::ifstream fs (argv[1]);
    Interpreter i;
    std::string s;
    while(!fs.eof()) {
        std::getline(fs, s);
        if(s.size() == 0)   continue;
        i.Evaluate(s);
    }
    
}