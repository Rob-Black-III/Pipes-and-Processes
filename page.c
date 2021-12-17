// Rob Black
// rdb5063@g.rit.edu

#include <stdio.h>
#include <stdint.h>

int main()
{

    // Given 32-bit address, find the physical page number, starting address in hex, and offset
    // Assume 4k page size

    uint32_t address, pageNumber, startingAddress, offset;

    static const uint32_t PAGE_SIZE = 4096;

    static const uint32_t testCases[] = {0, 19986, 2147483647, 429496000};

    for (int i = 0; i < 4; i++)
    {

        address = testCases[i];
        printf("Address: %d\n", address);

        // 4k page sizes
        pageNumber = address / PAGE_SIZE; 
        startingAddress = pageNumber * PAGE_SIZE;
        offset = address % PAGE_SIZE;

        printf("Page number: %d\n", pageNumber);
        printf("Starting address: 0x%x\n", startingAddress);
        printf("Offset: %d\n", offset);
        printf("\n");
    }
}