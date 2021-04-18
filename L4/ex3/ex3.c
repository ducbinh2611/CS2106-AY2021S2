/*************************************
* Lab 4 Exercise 3
* Name: Ngo Duc Binh
* Student Id: A0180061U
* Lab Group: B01
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mmalloc.h"
#include <stdbool.h>
#include <stdint.h>
/**********************************************************
 * This is a "global" structure storing heap information
 * The "static" modifier restrict the structure to be
 *  visible only to functions in this file
 *********************************************************/
static heapMetaInfo hmi;


/**********************************************************
 * Quality of life helper functions / macros
 *********************************************************/
#define powOf2(E) (1 << E)

int internalFragmentation = 0;

unsigned int log2Ceiling( unsigned int N )
/**********************************************************
 * Find the smallest S, such that 2^S >= N
 * S is returned
 *********************************************************/
{
    unsigned int s = 0, pOf2 = 1;

    while( pOf2 < N){
        pOf2 <<= 1;
        s++;
    }

    return s;
}


unsigned int log2Floor( unsigned int N )
/**********************************************************
 * Find the largest S, such that 2^S <= N
 * S is returned
 *********************************************************/
{
    unsigned int s = 0, pOf2 = 1;

    while( pOf2 <= N){
        pOf2 <<= 1;
        s++;
    }

    return s-1;
}

unsigned int buddyOf( unsigned int addr, unsigned int lvl )
/**********************************************************
 * Return the buddy address of address (addr) at level (lvl)
 *********************************************************/
{
    unsigned int mask = 0xFFFFFFFF << lvl;
    unsigned int buddyBit = 0x0001 << lvl;

    return (addr & mask) ^ buddyBit;
}

partInfo* buildPartitionInfo(unsigned int offset)
/**********************************************************
 * Allocate a new partInfo structure and initialize the fields
 *********************************************************/
{
    partInfo *piPtr;

    piPtr = (partInfo*) malloc(sizeof(partInfo));

    piPtr->offset = offset;
	piPtr->nextPart = NULL;

    //Buddy system's partition size is implicit
	//piPtr->size = size;

    //All available partition in buddy system is implicitly free
	//piPtr->status = FREE;

    return piPtr;
}

void printPartitionList(partInfo* piPtr)
/**********************************************************
 * Print a partition linked list
 *********************************************************/
{
	partInfo* current;
    int count = 1;
	
	for ( current = piPtr; current != NULL; 
		current = current->nextPart){
        if (count % 8 == 0){
            printf("\t");
        }
		printf("[+%5d] ", current->offset);
        count++;
        if (count % 8 == 0){
            printf("\n");
        }
	}
    printf("\n");
}

void printHeapMetaInfo()
/**********************************************************
 * Print Heap Internal Bookkeeping Information
 *********************************************************/
{
    int i;

	printf("\nHeap Meta Info:\n");
	printf("===============\n");
	printf("Total Size = %d bytes\n", hmi.totalSize);
	printf("Start Address = %p\n", hmi.base);

    for (i = hmi.maxIdx; i >=0; i--){
        printf("A[%d]: ", i);
        printPartitionList(hmi.A[i] );
    }

}

void printHeap()
/**********************************************************
 * Print the content of the entire Heap 
 *********************************************************/
{
    //Included as last debugging mechanism.
    //Print the entire heap regions as integer values.

    int* array;
    int size, i;
    
    size = hmi.totalSize / sizeof(int);
    array = (int*)hmi.base;

    for ( i = 0; i < size; i++){
        if (i % 4 == 0){
            printf("[+%5d] |", i);
        }
        printf("%8d",array[i]);
        if ((i+1) % 4 == 0){
            printf("\n");
        }
    }
}

void printHeapStatistic()
/**********************************************************
 * Print Heap Usage Statistics
 *********************************************************/
{
    //TODO: Task 4. Calculate and report the various statistics

    printf("\nHeap Usage Statistics:\n");
    printf("======================\n");

    printf("Total Space: %d bytes\n", hmi.totalSize);

   //Remember to preserve the message format!
    int numOfFreePartitions = 0;
    int freeSize = 0;
    int numOfLevel = hmi.maxIdx;
    for (int i = 0; i <= numOfLevel; i++) {
        partInfo* currLevel = hmi.A[i];
        while (currLevel != NULL) {
            numOfFreePartitions++;
            freeSize += powOf2(i);
            currLevel = currLevel->nextPart;
        }
    }
    printf("Total Free Partitions: %d\n", numOfFreePartitions);
    printf("Total Free Size: %d bytes\n", freeSize);

    printf("Total Internal Fragmentation: %d bytes\n", internalFragmentation);
}

void addPartitionAtLevel( unsigned int lvl, unsigned int offset )
/**********************************************************
 * There is just a suggested approach. You are free NOT to use this.
 *    This function adds a new free partition with "offset" at hmi.A[lvl]
 *    If buddy is found, recursively (or repeatedly) perform merging and insert
 *      at higher level
 *********************************************************/
{
    partInfo* addedPartition = buildPartitionInfo(offset);
    partInfo* curr = hmi.A[lvl];
    partInfo* prev = curr;
    // if the current level is empty or the first block already has greater offset 
    // then the addedPartition will be the new first block of the linked list
    if (curr == NULL || curr->offset > offset) {
        prev = NULL;
    } else {
        while (curr != NULL && curr->offset < offset) {
            prev = curr;
            curr = curr->nextPart;
        }
    }
  
    // merging of buddies
    unsigned int addressOfAddedPartition = offset;
    unsigned int addressOfBuddy = buddyOf(addressOfAddedPartition, lvl);
    bool foundBuddy = false;
    partInfo* currentPartition = hmi.A[lvl];
    partInfo* prevPartition = currentPartition; // the partition before buddy partition in the current level
    partInfo* buddyPartition;
    // find whether the buddy of the added partition is free
    // buddy has to be in the same level as the addedPartition
    while (currentPartition != NULL && !foundBuddy) {
        unsigned int addressOfCurrentPartition = currentPartition->offset;
        if (addressOfCurrentPartition == addressOfBuddy) {
            foundBuddy = true;
            buddyPartition = currentPartition;
        } else {
            prevPartition = currentPartition;
            currentPartition = currentPartition->nextPart;
        }
    }

   if (foundBuddy) {
        unsigned int newOffset;
        if (buddyPartition->offset < addedPartition->offset) {
            newOffset = buddyPartition->offset;
        } else {
            newOffset = addedPartition->offset;
        }
        // remove the buddy from the current level
        // if buddy partition is the first block of the linked list
        if (prevPartition == buddyPartition) {
            hmi.A[lvl] = prevPartition->nextPart;
        } else {
            prevPartition->nextPart = buddyPartition->nextPart;
        }
        free(buddyPartition);
        free(addedPartition);
        // recursively merge upwards if a buddy is found
        addPartitionAtLevel(lvl + 1, newOffset);
    } else {
        // if the buddy partition is not free then only need to add the addedPartition to the current level
        // if addedPartition is not the first block of the linked list
        if (prev != NULL) {
            partInfo* temp = prev->nextPart;
            prev->nextPart = addedPartition;
            addedPartition->nextPart = temp;
        } else {
            // if the current level is empty, simply add the new partition there
            // else append the current link list as nextPart of the addedPartition
            if (curr == NULL){
                hmi.A[lvl] = addedPartition;
            } else {
                addedPartition->nextPart = hmi.A[lvl];
                hmi.A[lvl] = addedPartition;
            }
        }

    }
}

partInfo* removePartitionAtLevel(unsigned int lvl)
/**********************************************************
 * There is just a suggested approach. You are free NOT to use this.
 *    This function remove a free partition at hmi.A[lvl]
 *    Perform the "upstream" search if this lvl is empty AND perform
 *      the repeated split from higher level back to this level.
 * 
 * Return NULL if cannot find such partition (i.e. cannot sastify request)
 * Return the Partition Structure if found.
 *********************************************************/
{   
    // If can find a free block at this level, allocate that block
    if (hmi.A[(int)lvl] != NULL) {
        partInfo* returnedPartition = hmi.A[(int) lvl];
        hmi.A[(int)lvl] = returnedPartition->nextPart;
        return returnedPartition;
    } else {
        // find the smallest R from (lvl + 1) to maxIdx such that A[R] has a free block
        unsigned int nextLevel = lvl + 1;
        bool isFound = false;
        while (!isFound) {
            if (nextLevel > hmi.maxIdx) {
                return NULL;
            }
            if (hmi.A[(int) nextLevel] != NULL) {
                isFound = true;
            } else {
                nextLevel += 1;
            }
        }

        // now need to split from R - 1 to lvl
        unsigned int currentLevel = nextLevel;
        while ((int) currentLevel > lvl) {
            int offset = hmi.A[(int) currentLevel]->offset;
            hmi.A[(int) currentLevel] = hmi.A[(int) currentLevel]->nextPart;
            unsigned int lowerLevel = currentLevel - 1;
            int buddyOffset = offset + (int) powOf2(lowerLevel);
            partInfo* first = buildPartitionInfo((unsigned int)offset);
            partInfo* second = buildPartitionInfo((unsigned int) buddyOffset);
            first->nextPart = second;
            second->nextPart = hmi.A[(int) currentLevel - 1];
            hmi.A[(int) currentLevel - 1] = first;
            currentLevel--;
        }
        return removePartitionAtLevel(lvl);


    }
    return NULL; 
}

int setupHeap(int initialSize)
/**********************************************************
 * Setup a heap with "initialSize" bytes
 *********************************************************/
{
	void* base;

	base = sbrk(0);
	if(	sbrk(initialSize) == (void*)-1){
		printf("Cannot set break! Behavior undefined!\n");
		return 0;
	}

    hmi.base = base;

	hmi.totalSize = initialSize;
    hmi.internalFragTotal = 0;
	
    //TODO: Task 1. Setup the rest of the bookkeeping info:
    //       hmi.A <= an array of partition linked list
    //       hmi.maxIdx <= the largest index for hmi.A[]
    //
    int maxIdx = (int) log2Ceiling((unsigned int)initialSize);       
    hmi.A = (partInfo**) malloc(sizeof(partInfo*) * (maxIdx + 1));   //change this!
    hmi.maxIdx = maxIdx; //change this!
    // Initialize an empty link list from hmi.A[0] to hmi.A[maxIdx - 1]
    for (int i = 0; i < maxIdx; i++) {
        hmi.A[i] = NULL;
    }
    partInfo* initialFreePartition = buildPartitionInfo(0);
    hmi.A[maxIdx] = initialFreePartition;
    return 1;
}


void* mymalloc(int size)
/**********************************************************
 * Mimic the normal "malloc()":
 *    Attempt to allocate a piece of free heap of (size) bytes
 *    Return the memory addres of this free memory if successful
 *    Return NULL otherwise 
 *********************************************************/
{
    //TODO: Task 2. Implement the allocation using buddy allocator
    // size <= 2^level
    unsigned int level = log2Ceiling((unsigned int) size);
    if (level > hmi.maxIdx) {
        return NULL;
    }

    partInfo* blockAllocated = removePartitionAtLevel(level);
    // if found a block
    if (blockAllocated != NULL) {
        internalFragmentation += powOf2(level) - size;
        unsigned int offset = blockAllocated->offset;
        free(blockAllocated);
        return (void*)hmi.base + offset;
    }
    return NULL;

}

void myfree(void* address, int size)
/**********************************************************
 * Mimic the normal "free()":
 *    Attempt to free a previously allocated memory space
 *********************************************************/
{
    //TODO: Task 3. Implement the de allocation using buddy allocator
    unsigned int level = log2Ceiling((unsigned int) size);
    if (level > hmi.maxIdx) {
        return;
    }
    internalFragmentation -= powOf2(level) - size;
    addPartitionAtLevel(level, address - hmi.base);
    

}
