#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> 

// F1. Move

// when called, it moves position by given value
int move(int *positionPointer, int value, int L)			
{

	*positionPointer += value;

	if (*positionPointer > L)		//if the position goes past L, it moves to 1
	{
		*positionPointer -= L;
	}
	if (*positionPointer < 1)		// if position moves below 1, it moves to L
	{
		*positionPointer += L;
	}
	

}

// F2. Random Number generator(given)

// declaration of a function that returns a random integer
// in the range 0...(max-1)	
// note random() is a built-in C function
// the % operator divides by max and takes the remainder
int randomInt(int max)
{
	return (random() % max);
}


void
main()
{
	//0. Variables
	int L;						//number of sites in each axis
	int upperLimit;				//upper L limit
	int seedLowerLimit;
	int seedUpperLimit;
	int seedcounter=0;
	int count;					//keeps track of how many hop
	int visitedall;				//used to keep track if it has visited it all
	FILE*fp;					//file that it prints into
	int hasItVisitedAllSites;	//used as a true/false statement
	int x = 0;					//xyz stored in position. for clarity
	int y = 1;
	int z = 2;
	int position[3];			//current position

	// 1. Assigning Variables

	//Setting up L upper and lower limit. if invalid value entered, it exits.
	printf("Enter L lower limit\n");
	scanf("%d", &L);

	if (2 > L) {
		printf("Error: Enter valid lower limit\n");
		exit(0);
	}

	printf("Enter upper limit \n");
	scanf("%d", &upperLimit);


	if (upperLimit < L) {
		printf("Error: Enter value bigger or equal to L\n");
		exit(0);
	}


	

	//Setting up seed upper and lower limit. if invalid value entered, it exits.
	int repeats;
	printf("Number of random walk repeats. Value must be 0 or greater\n");
	scanf("%d", &repeats);
	
	if (0 > repeats) {
		printf("Error: repeats must be 0 or greater\n");
		exit(0);
	}

	printf("Enter seed, or enter 0 to pick a random seed\n");
	scanf("%d", &seedLowerLimit);
	
	if (0 > seedLowerLimit) {
		printf("Error: seed must be greater or equal to 0\n");
		exit(0);
	}
	if (0 == seedLowerLimit) {
		srand(time(NULL));
		seedLowerLimit = (rand() % 99) + 1;
	}
	seedcounter = seedLowerLimit;
	seedUpperLimit = seedLowerLimit + repeats;


	// 2.File and print headers

	// makes a data.txt file to write to
	fp = fopen("data.txt", "w");
	
	// headings written to file
	fprintf(fp, "Seed:%d \nL\tHops to visit every site",seedcounter);					
	
	// prints seed number on top
		

	// 3. Loops
		
	for (L; L <= upperLimit; L++) {
		
		//loading bar for user
		printf("calculating L %d, upper limit is %d\n", L, upperLimit);		

		//first column
		fprintf(fp, "\n%d\t", L);	
		
		//number of sites that are available
		int totalNoSites = pow(L, 3);

		for (int seed = seedLowerLimit; seed <= seedUpperLimit; seed++) {
			
			// 3.1 resetting variables
			srandom(seedcounter);
			
			// start position
			position[x] = 1;	//x
			position[y] = 1;	//y
			position[z] = 1;	//z
			
			// printf("(%d,%d,%d) start L:%d Seed:%d\n", position[x], position[y], position[z],L,seedcounter); // use to see if code is working correctly.
			//there is another print statement below for checking as well(around line 195)
			seedcounter++;
			visitedall = 1;	//start position has already been visited.
			count = 0;		//hops so far
			hasItVisitedAllSites = 0;	//sets it as "false". This is a check for the while function to allow it to exit loop.
			int visited[L][L][L];

			for (int j = 0; j < L; j++) {	//sets all array value to 0. prevents errors. Not done by default.
				for (int k = 0; k < L; k++) {
					for (int t = 0; t < L; t++) {
						visited[j][k][t] = 0;
					}
				}
			}
			
			visited[position[x] - 1][position[y] - 1][position[z] - 1] = 1; // start site has been visited


			// 3.2 Random number and Move
			
			while (hasItVisitedAllSites == 0)	
			{	
				//3.3 Moving the position
				switch (randomInt(6)) {

				case 0:
				{
					move(position + x, 1, L);						
					break;

				}
				case 1:
				{
					move(position + x, -1, L);						
					break;

				}
				case 2:
				{
					move(position + y, 1, L);						
					break;

				}
				case 3:
				{
					move(position + y, -1, L);					
					break;

				}
				case 4:
				{
					move(position + z, 1, L);		//position + z forwards memory location(pointer) of position[z] to function
					break;
				}
				case 5:
				{
					move(position + z, -1, L);		
					break;
				}

				}
				
				count++;	//number of hops
				//printf("(%d,%d,%d)\n", position[x], position[y], position[z]);  //check used to see if program works.
				
				//3.3 Check
				//if site not registered as visited, add 1 to visitedall and changes visited site to true(1)
				if (visited[position[x] - 1][position[y] - 1][position[z] - 1] == 0) {
					visitedall++;
					visited[position[x] - 1][position[y] - 1][position[z] - 1] = 1;
					if (visitedall == totalNoSites) {
						hasItVisitedAllSites = 1; //this allows it to exit while loop
						fprintf(fp, "%d\t", count); //prints count to file
					}
				}

			}
			

		}
	}
	fclose(fp);		//closes data.txt
	printf("done. look for file inside the folder which this program lies. File name is data.txt\n");
	printf("seed:%d\n", seedLowerLimit);
}