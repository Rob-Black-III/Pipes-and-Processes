/**
 *  Rob Black
 *  Operating Systems
 *  Homework 2
**/

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <unistd.h>


void main(int argc, char* argv[]){
    if(argc == 3){
        //printf("Input filepath: %s\n", argv[1]);
        //printf("Output filepath: %s\n", argv[2]);
        copyFile(argv[1],argv[2]);
    }
}

int copyFile(char* sourcePath, char* destinationPath){

    // Declare FDs
    int inputFileDescriptor, outputFileDescriptor;

    // Return value
    int copyStatus;

    // For fstat. We need a buffer declared
    struct stat fstatBuffer = {0};
    off_t offsetBytesNum = 0;

    // https://pubs.opengroup.org/onlinepubs/9699919799.2018edition/functions/open.html
    inputFileDescriptor = open(sourcePath, O_RDONLY);

    if(inputFileDescriptor == -1){
        return -1;
    }

    outputFileDescriptor = creat(destinationPath, 0644);

    if(outputFileDescriptor == -1){
        return -1;
    }

    // We need the buffer size to no how much to copy for later.
    // https://pubs.opengroup.org/onlinepubs/009696699/functions/fstat.html
    fstat(inputFileDescriptor, &fstatBuffer);

    // https://man7.org/linux/man-pages/man2/sendfile.2.html
    copyStatus = sendfile(outputFileDescriptor, inputFileDescriptor, offsetBytesNum, fstatBuffer.st_size);

    // Close the FD's
    close(inputFileDescriptor);
    close(outputFileDescriptor);

    return copyStatus;
}