#include <iostream>


#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cmath>
#include <sys/mman.h>

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

    // Set up shared memory
    // Examples used for reference:
    //  https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_shared_memory.htm
    //  https://www.geeksforgeeks.org/ipc-shared-memory/
    //  https://stackoverflow.com/questions/5656530/how-to-use-shared-memory-with-linux-in-c
    const int numRows = ceil(log2(itemCount))+1; // how many rows of the n elements we will use in shared memory
    const int numInts = numRows * itemCount; // total number of integers in shared memory
    const int memSize = numInts * 4; // the size of shared memory in bytes

    /*key_t key = ftok("arbitrary_key", 12345); // arbitrary key used for setting up shared memory
    int sharedMemoryId = shmget(key, memSize, 0666|IPC_CREAT);
    int* data = (int*) shmat(sharedMemoryId, NULL, 0);
     */
    const int access = PROT_READ | PROT_WRITE; // allow both reading and writing
    const int visibility = MAP_SHARED | MAP_ANONYMOUS; // shared, but visible only to related processes
    int* data = (int*) mmap(NULL, memSize, access, visibility, -1, 0);


    cout << "Shared memory size: " << memSize << endl;

    //set up array by reading input file
    // Examples used for reference:
    //  https://www.tutorialspoint.com/cplusplus/cpp_files_streams.htm
    //  https://www.quora.com/How-does-one-read-input-until-the-end-of-file-end-of-line-in-C++
    ifstream infile = ifstream(inputFileName);
    int index = 0;
    int currentValue;
    while (infile >> currentValue) {
        cout << "v:" << currentValue << endl;

        if (index >= itemCount)
        {
            cerr << "Error: More numbers were in the file than expected." << endl;
            exit(0);
        }


        data[index++] = currentValue;
    }
    cout << "i:" << index << endl;
    if (index != itemCount) // we expect 0 plus n to equal n
    {
        cerr << "Error: Unexpected number of numbers in input file." << endl;
        exit(0);
    }

}
