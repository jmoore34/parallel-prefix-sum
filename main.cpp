#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <sys/mman.h>
#include <thread>
#include <boost/thread.hpp>
#include <boost/thread/barrier.hpp>

using namespace std;

int itemCount;
int *data;

int *getArr(int iteration) {
    return data + iteration * itemCount;
}

void doWork(int id, int iterations, int numThreads) {
    boost::barrier rendezvous(numThreads);
    for (int iteration = 0; iteration < iterations; ++iteration) {

        int blockSize = itemCount / numThreads;
        int blockStart = blockSize * id;
        int blockEnd = (id == numThreads - 1) ? itemCount : blockSize * (id +
                                                                         1); // non-inclusive block end; if last section, go until last item
        for (int k = blockStart; k < blockEnd; ++k) {
            int distance = (1 << iteration); // compute 2^(iteration) to determine how far left element is
            int add = 0;
            if (k - distance >= 0)
                add = getArr(iteration)[k - distance];

            getArr(iteration + 1)[k] = add + getArr(iteration)[k];
        }
        //debug for checking each child processes' start and end index of for all iterations
        //cout << "Iteration: " << iteration << " ID: " << myIndex << " Start: " << blockStart << " End: " << blockEnd << endl;

        rendezvous.wait();
    }

}


int main(int argc, char **argv) {
    // Input validation
    if (argc != 5) { // 4 arguments + path
        cout << "Error: Expected 4 arguments." << endl;
        exit(1);
    }

    // Parse commandline arguments
    itemCount = stoi(argv[1]);
    string inputFileName = argv[2];
    string outputFileName = argv[3];
    int numThreads = stoi(argv[4]);

    if (numThreads <= 0) {
        cerr << "Number of threads must be at least 1." << endl;
        exit(0);
    }

    if (itemCount <= 0) {
        cerr << "Number of items must be at least 1." << endl;
        exit(0);
    }

    // Set up shared memory
    // Examples used for reference:
    //  https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_shared_memory.htm
    //  https://www.geeksforgeeks.org/ipc-shared-memory/
    //  https://stackoverflow.com/questions/5656530/how-to-use-shared-memory-with-linux-in-c
    const int ITERATIONS = ceil(log2(itemCount));
    const int numRows = ITERATIONS + 1; // how many rows of the n elements we will use in shared memory
    const int numInts = numRows * itemCount; // total number of integers in shared memory
    const int memSize = numInts * 4; // the size of shared memory in bytes

    const int access = PROT_READ | PROT_WRITE; // allow both reading and writing
    const int visibility = MAP_SHARED | MAP_ANONYMOUS; // shared, but visible only to related processes
    data = (int *) mmap(NULL, memSize, access, visibility, -1, 0);

    //debug for checking created shared memory size
    //cout << "Shared memory size: " << memSize << endl;

    //set up array by reading input file
    // Examples used for reference:
    //  https://www.tutorialspoint.com/cplusplus/cpp_files_streams.htm
    //  https://www.quora.com/How-does-one-read-input-until-the-end-of-file-end-of-line-in-C++
    ifstream infile = ifstream(inputFileName);
    int index = 0;
    int currentValue;
    while (infile >> currentValue) {
        //debug for checking currentValue from input file
        //cout << "v:" << currentValue << endl;

        if (index >= itemCount) {
            cerr << "Error: More numbers were in the file than expected." << endl;
            exit(0);
        }


        data[index++] = currentValue;
    }
    //debug for checking index value
    //cout << "i:" << index << endl;
    if (index != itemCount) // we expect 0 plus n to equal n
    {
        cerr << "Error: Invalid input file." << endl;
        exit(0);
    }

    const int PARENT = -1;
    int myIndex = PARENT; // Process index-- which process am I (-1 is parent)


    std::thread workers[numThreads];
    for (int i = 0; i < numThreads; ++i) {
        workers[i] = std::thread(doWork, i, ITERATIONS, numThreads);
    }


    for (int i = 0; i < numThreads; ++i) {
        workers[i].join();
    }


    ofstream outfile(outputFileName);
    for (int i = 0; i < itemCount; ++i) {
        outfile << getArr(ITERATIONS)[i] << " ";
    }

    /*for (int i=0; i<itemCount; ++i)
    {
        cout << getArr(ITERATIONS)[i] << " ";
    }*/
}
