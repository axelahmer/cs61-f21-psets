#include <cstring>
#include <cstdio>

//c++ libs
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

int main(int argc, char* argv[]){
    // minimal std lib:
    // selection sort argv
    // for (int i = 1; i < argc-1; i++)
    // {
    //     int best = i;
    //     for (int j = i+1; j < argc; j++)
    //     {
    //         if (strcmp(argv[best],argv[j]) > 0)
    //         {
    //             best  = j;
    //         }
    //     }
    //     // swap
    //     if (best != i)
    //     {
    //         char* temp = argv[i];
    //         argv[i] = argv[best];
    //         argv[best] = temp;
    //     }
        
    // }
    // for (int i = 1; i < argc; i++)
    // {
    //     fprintf(stdout,"%s\n",argv[i]);
    // }
    
    // use of std lib:
    // create a vector of the arguments as C++ strings
    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
        args.push_back(argv[i]);
    }

    // sort the vector’s contents
    std::sort(args.begin(), args.end());

    // print the vector’s contents
    for (size_t i = 0; i != args.size(); ++i) {
        std::cout << args[i] << '\n';
    }


}