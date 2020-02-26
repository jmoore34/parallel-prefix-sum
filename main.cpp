#include <iostream>
#include <unistd.h>

using namespace std;
int main() {
    cout << "Hello, World!" << endl;
    int pid = fork();
    if(pid==0){
        cout << "child" << endl;
    } else{
        cout << "parent" << endl;
    }
    //command line argument

    //input file

    //validate inputs and number of elements

    //output file

    //create "large enough" shared memory space

    //create child processes

    return 0;
}
