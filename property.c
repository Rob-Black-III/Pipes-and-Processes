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

    // Size we expect the array to be. Upper bound.
    int n = 100;
  
    int startingNumber;
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
  
    scanf("%d", &startingNumber);

    // Bounds Check
    if(startingNumber < 0){
        fprintf(stderr, "Integer must be positive.");
        exit(0);
    }


    printf("Input Number Before Fork: %d\n",startingNumber);
    p = fork();
  
    if (p < 0)
    {
        fprintf(stderr, "Failed to create fork.");
        exit(-1);
    }
  
    // Parent process
    else if (p > 0)
    {
        // Declare sequence inside local scope to prove they are different.
        int* sequence = (int*)malloc(n * sizeof(int));
  
        // Don't need to read input anymore, we're done, close it.
        close(inputPipe[0]);
  
        // Write the number to the pipe and close the connection.
        write(inputPipe[1], &startingNumber, sizeof(startingNumber));
        close(inputPipe[1]);
  
        // Wait for the child process to finish...
        wait(NULL);
  
        // Since the child finished, we can close the pipe.
        close(outputPipe[1]);
  
        // Read the sequence one integer at a time, limited by our max array bounds.
        for (int i = 0; i < n; i++){
            read(outputPipe[0], &sequence[i], sizeof(int));
        }

        // Close the output pipe.
        close(outputPipe[0]);

        // Janky print sequence. 
        printf("Sequence in Parent after sent back from child. \n");
        int printIndex = 0;
        while(printIndex < n){
            printf("%d ", sequence[printIndex]);

            // Exit condition
            if(sequence[printIndex] == 1){
                break;
            }
            else{
                printIndex++;
            }  
        }
        printf("\n");

        free(sequence);
    }
  
    // Child process
    else
    {
        // Declare sequence inside local scope to prove they are different.
        int* sequence = (int*)malloc(n * sizeof(int));

        // Don't need the pipe anymore.
        close(inputPipe[1]);
  
        // Read the numbe provided from STDIN.
        int childStartNum;
        read(inputPipe[0], &childStartNum, sizeof(int));

        // Meat and Potatoes. The actual algorithm provided.
        printf("Child process received number: %d\n",childStartNum);
        
        int currentNumber = childStartNum;
        int currentIndex = 1;
        sequence[0] = childStartNum;
        while(currentNumber != 1){
            if(currentNumber % 2 == 0){
                currentNumber = currentNumber/2;
                sequence[currentIndex] = currentNumber;
                currentIndex++;
            }
            else{
                currentNumber = 3 * currentNumber + 1;
                sequence[currentIndex] = currentNumber;
                currentIndex++;
            }
        }

        // Test Print
        printf("Sequence in Child. \n");
        for(int i=0; i < currentIndex && i < n;i++){
            printf("%d ", sequence[i]);
        }
        printf("\n");
  
        // Close the "read" side of the pipes
        close(inputPipe[0]);
        close(outputPipe[0]);
  
        // Write the sequence back to the parent int by int.
        for (int i = 0; i < n; i++){
            write(outputPipe[1], &sequence[i], sizeof(int));
        }

        // Close the pipe.
        close(outputPipe[1]);

        free(sequence);
  
        exit(0);
    }
}