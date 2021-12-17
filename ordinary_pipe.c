/**
 *  Rob Black
 *  Operating Systems
 *  Homework 2
**/

#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

void main(){
  
    // Two sides of the first pipe (send and receive input number)
    int inputPipe[2];

    // Two sides of the second pipe (send and receive output sequence)
    int outputPipe[2];

    pid_t p;
  
    if(pipe(inputPipe) == -1)
    {
        fprintf(stderr, "Pipe Creation Failed");
        exit(-1);
    }
    if(pipe(outputPipe) == -1)
    {
        fprintf(stderr, "Pipe Creation Failed");
        exit(-1);
    }

    p = fork();
  
    if (p < 0)
    {
        fprintf(stderr, "Failed to create fork.");
        exit(-1);
    }
  
    // Parent process
    else if (p > 0)
    {
        // Receive buffer for the response message.
        char messageToReceiveBuffer[100];
        char* messageToSend = "Hello Child Process";
  
        // Don't need to read input anymore, we're done, close it.
        close(inputPipe[0]);
  
        // Write the number to the pipe and close the connection.
        write(inputPipe[1], messageToSend, strlen(messageToSend)+1);
        close(inputPipe[1]);
  
        // Wait for the child process to finish...
        wait(NULL);
  
        // Since the child finished, we can close the pipe.
        close(outputPipe[1]);

        read(outputPipe[0],messageToReceiveBuffer,100);
        printf("Parent received: %s\n",messageToReceiveBuffer);

        // Close the output pipe.
        close(outputPipe[0]);
    }
  
    // Child process
    else
    {
        // Don't need the pipe anymore.
        close(inputPipe[1]);

        // Response
        char* messageToSend = "Hello Parent Process";
  
        // Make a buffer to read the string from the parent.
        char messageToReceiveBuffer[100];
        read(inputPipe[0], messageToReceiveBuffer,100);

        printf("Child received: %s\n",messageToReceiveBuffer);

        // Write the response message to the pipe to send to the parent.
        write(outputPipe[1],messageToSend,strlen(messageToSend)+1);
  
        // Close the "read" side of the pipes
        close(inputPipe[0]);
        close(outputPipe[0]);

        // Close the pipe.
        close(outputPipe[1]);
  
        exit(0);
    }
}