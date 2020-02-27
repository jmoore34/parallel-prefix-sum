#include <iostream>


#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>

using namespace std;

int main(int argc, char** argv) {
    // Input validation
    if (argc != 5) { // 4 arguments + path
        cout << "Error: Expected 4 arguments." << endl;
        exit(1);
    }

    // Parse commandline arguments
    int itemCount = stoi(argv[1]);
    string inputFileName = argv[2];
    string outputFileName = argv[3];
    int numProcesses = stoi(argv[4]);

}
