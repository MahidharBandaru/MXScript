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
    std::string s;

    std::string str(static_cast<std::stringstream const&>(std::stringstream() << fs.rdbuf()).str()); 

    auto it = str.begin();
    for(auto it = str.begin(); it != str.end(); ++it)
    {   
        char c = *it;
        if(c == '@')
        {
            it++;
            while(it != str.end())
            {
                c = *it;
                if (c == '\n')  break;
                ++it;
            }
            if(it == str.end()) break;
        }
        if(c != '\n')
            s += c;
    }
    s.insert(s.begin(), '{');
    s.insert(s.end(), '}');
    i.Evaluate(s);

    return 0;    
}