/*
 *  Implementation of FIFO and LRU page replacement algorithm
 *  Please add appropriate level of comments in this file
 */

#include "vmalgorithm.h"


/* Generate an access pattern
 * Example: 3, 2, 2, 1, 1  indicates the page 3, 2, 2, 1, 1 in order
 */
void generateAccessPattern()
{
	int i;
	srand(time(0));
	accessPattern = (int *)malloc(sizeof(int) * AccessPatternLength);
	printf("The randomized Access Pattern: ");
	for (i = 0; i < AccessPatternLength; i++)
	{
		accessPattern[i] = rand() % ReferenceSZ;
		printf("%d ", accessPattern[i]);
	}
	printf("\n");
}


/*
 * Initialize the parameters of simulated memory
 */
void initializePageFrame()
{
	int i;
	memory.PageFrameList = (int *)malloc(sizeof(int) * FrameNR); // dynamic allocated FrameNR frames to be used in memory
	memory.nextToReplaced = 0;									 // indicate the new frame to be replaced as 0
	for (i = 0; i < FrameNR; i++)
	{
		memory.PageFrameList[i] = -1; // initialization to -1 indicating all frames are unused
	}
}


// Print the pages loaded in memory
void printPageFrame()
{
	int i;
	for (i = 0; i < FrameNR; i++)
	{
		printf("%2d ", memory.PageFrameList[i]);
	}
	printf("\n");
}


/*
 *  Print the access pattern in order
 */
void printAccessPattern()
{
	int i;
	printf("The Same Access Pattern: ");
	for (i = 0; i < AccessPatternLength; i++)
	{
		printf("%d ", accessPattern[i]);
	}
	printf("\n");
}


/*
 * Return: number of the page fault using FIFO page replacement algorithm
 */
int FIFO()
{
	int found;
	int numPageFault = 0;

	/* search for the page number in access pattern */

	for(int i = 0; i < AccessPatternLength; i++)
	{
		found = 0;

		for(int j = 0; j < FrameNR; j++)
		{
			if(accessPattern[i] == memory.PageFrameList[j])
			{
				found = 1;
				break;
			}
		}

		/* replace page number when not found in physical address */

		if(found == 0)
		{
			numPageFault++;
			memory.PageFrameList[memory.nextToReplaced] = accessPattern[i];
			memory.nextToReplaced = (memory.nextToReplaced + 1) % FrameNR;
		}
		
		printPageFrame();
	}

	return numPageFault;
}


/*
 * Return: number of the page fault using LRU page replacement algorithm
 */
int LRU()
{
	int found;
	int numPageFault = 0;
	int index;

	/* search for the page number in access pattern */

	for(int i = 0; i < AccessPatternLength; i++)
	{
		int pageNumber = accessPattern[i];
		found = 0;

		for(int j = 0; j < FrameNR; j++)
		{
			if(memory.PageFrameList[j] == pageNumber)
			{
				found = 1;
				break;
			}
		}

		/* requested page non-existent in the frame */

		if(found == 0)
		{
			int status[20], temp;

			for(int k = 0; k < FrameNR; k++)
				status[k] = 0;


			for(int m = 0, temp = i - 1; m < FrameNR - 1; m++, temp--)
			{
				for(int n = 0; n < FrameNR; n++)
				{
					if(memory.PageFrameList[n] == accessPattern[temp])
						status[n] =  1;
				}
			}

			/* insert the page where status is still 0 */
			for(int p = 0; p < FrameNR; p++)
			{
				if(status[p] == 0){
					index = p;					
				}
			}
        	memory.PageFrameList[index] = accessPattern[i]; // place page requested at index location in PageFrameList
        	numPageFault++;	
		}

		printPageFrame();
	}

	return numPageFault;
}