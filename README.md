# Parallel Prefix Sum
Authors: Jonathan Moore (jtm170330), George Lu (gxl170230)

## Install instructions

A. Use the prebuilt binary at cmake-build-debug/parallel_prefix_sum

or 

B. Run cmake in the project root (and make afterwards, if needed) to build from source.

## Parent process

 1. read command line argument
 2. check for argument errors
 3. create "large enough" shared memory segment
 4. read the input array & store it in the shared memory segment
 5. create child processes
 6. wait for all child processes to terminate
 7. read the output from shared memory & write to output file

## Child process

    /*my id - index of the child process (0...m)
    for i<-0 to [logn]-1
    {
	    a:= starting position of the input array for iteration i
	    b:= starting position of the output array for iteration i
	    checkbegin:= starting position of current process's chunk in b
	    checkend:= startingposition of the next chunk in b
	    for j<-chunkbegin to chunkend-1
	    {
		    compute b[j] using prefix sum algorithm
		}
		*wait for all other child processes to finish their iteration
	} */


