#include <stdio.h>
#include <stdlib.h>
#include<time.h> 

//F1. Move
// when called, it moves position by value
int move(int value, int *position, int L)			
{
	*position += value;

	if (*position > L)		//if the position goes past L, it moves it to 1
	{
		*position -= L;
	}
	if (*position < 1)		// if it moves below 1, it moves it to L
	{
		*position += L;
	}
	


}

//F2. Check
// checks all positions have been visited. if they have it returns 1.
int check(int *pointer, int L) {		// the pointer that is entered is the going to be visited[0]
	
	int visitedall = 0; //used to keep track if it has visited it all
	for (int i = 0; i < L; i++) {	//when the atom visits a site, the visited[site] =1.
		visitedall += *pointer;	//if all sites have been visited then the total sum of value in array will be ==L
		pointer++;
	}
	if (visitedall == L)
		return 1;
	else
		return 0;
}

//F3. Random Number generator(given)
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
	int upperLimit;				//upper L limit
	int printSites;				//options for printing data
	int seedLowerLimit;	
	int seedUpperLimit;
	int seedcounter = 0;
	int L;						//Number of sites
	int position;				//current position
	int count;					//keeps track of how many hops
	int hasItVisitedAllSites;	//used as a true/false statement
	int history[20000];			//saves order of visit.
	FILE*fp;					//file that it prints into
	//1. Assign Variables
	
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
	if (seedLowerLimit ==0) {
		srand(time(NULL));
		seedLowerLimit = (rand() % 99) + 1;
		
	}
	seedcounter = seedLowerLimit;
	seedUpperLimit = seedLowerLimit + repeats;


	//choice between 2 types of data output. 
	printf("Print the path the particle walks?\n");
	printf("1 for yes, 2 to print only how many hops for it to visit every site\n");
	scanf("%d", &printSites);

	if (1 > printSites || printSites > 2) {
		printf("Error: Enter 1 or 2\n");
		exit(0);
	}


	

	//2. Setting up file and printing headers
	
	//data.txt made to write to
	fp = fopen("data.txt", "w");

	//option 1, printing headers to file
	if (printSites == 1) {
		fprintf(fp, "L\tseeds\tHopsTovisit\tPath");	
	}
	
	//option 2, printing headers to file
	if (printSites == 2) {
		fprintf(fp, "Seed:%d\n", seedcounter);
		fprintf(fp, "L\tHops to visit every site");					
	}

	//setting up loading bar for user
	int loading;
	int loadingtotal = (upperLimit+1-L)*(seedUpperLimit+1-seedLowerLimit);

	//3. Main Loops

	for (L; L <= upperLimit; L++) {
		
		//loading bar
		printf("calculating %d/%d\n", loading, loadingtotal);	
		
		//option 2, 1st column printing to file which is L value
		if (printSites == 2) {
			fprintf(fp, "\n%d", L);
		}

		for (int seed = seedLowerLimit; seed <= seedUpperLimit; seed++) {
			

			//setting up seed
			//for print 1, to prevent particle walking same path when seed is repeated
			//a seed counter makes sure seed isnt repeated.
			if (printSites == 2) {
				srandom(seedcounter);
				seedcounter++;
			}
			else
			{
				srandom(seed);
			}

			//loading
			loading++;

			//option 1, printing first 2 columns to file. These are L and seed values.  
			if (printSites == 1) {					
				fprintf(fp, "\n%d\t%d", L, seed);
			}

			//3.1 Resetting Variables
			position = 1;
			count = 0;
			hasItVisitedAllSites = 0;		//sets it as "false" to start with. used in while loop as a condition.
			history[count] = position;		//start position stored in history[0] 
			int visited[L];					//visited array will be used to check if it has visited a certain site before.
			int *visitedPointer = visited;	// C doesn't allow an array as an input variable for a function, so pointer used instead

			//assign all values in visited to 0.(Not done by default)
			for (int j = 0; j < L; j++) {	
				visited[j] = 0;
			}

			visited[position - 1] = 1;		//The start position has already been visited

			//3.2 Random number and move
			//after all sites have been visited, check fucntion change hasItVisitedAllsites to 1. 
			while (hasItVisitedAllSites == 0)	
			{

				//random number generated(0 or 1). moves -1 if number generated is 0, and +1 if 1.
				if (randomInt(2) == 1)
				{
					move(1, &position, L);						//move function called.
				}
				else
				{
					move(-1, &position, L);						//same as above except moves -1.
				}
				
				//3.3 Check and print
				count++;						//after each move, counter increases. This is number of hops
				visited[position - 1] = 1;		//adds "TRUE" to current position in visited array.
				hasItVisitedAllSites = check(visitedPointer, L);	//check returns 1 if all sites visited.
				
				
				if (hasItVisitedAllSites == 1) {			
					
					fprintf(fp, "\t%d", count); //after all sites have been visited. the number of hops is printed.

				}

				//option 1
				if (printSites == 1) {
					history[count] = position; //stores current position in array named history
				}
			}

			//option 1
			//print all sites that have been visited in order
			if (printSites == 1) {				
				for (int k = 0; k < count + 1; k++) {
					fprintf(fp, "\t%d", history[k]); 
				}

			}
		}
	}
	fclose(fp);		//closes data.txt
	printf("calculating %d/%d\n", loading, loadingtotal);
	printf("done. look for file inside the folder which this program lies. File name is data.txt\n");
	printf("seed:%d\n", seedLowerLimit);
}