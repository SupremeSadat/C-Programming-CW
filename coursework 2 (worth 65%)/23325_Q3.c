#include <stdio.h>
#include <stdlib.h>
#include<time.h> 
// F1. Random number generator

// declaration of a function that returns a random integer
// in the range 0...(max-1)	
// note random() is a built-in C function
// the % operator divides by max and takes the remainder
int randomInt(int max)
{
	return (random() % max);
}

// F2. Move Function
// position modelled as a 2d array where position[0] is A(possible cluster positions are 1 to A)
// position[1] is B(possible B positions are 1 to B). The particle can walk to any other B position inside its current cluster. 
// But to go to another cluster(A) it must first be at 1 or B. It can only move 1 cluster up(from B) or down(from 1). 

int move(int* positionPointerA, int value, int A, int B)
{

	int* positionPointerB = positionPointerA + 1;

	//if current position is 1, 0 will represent a change in cluster.
	//all the other positions cannot get 0, so 1 is added to the value.
	if (*positionPointerB != 1) {
		value++;
	}
	//if the value is equal to the position, 1 is added to the value. prevents it hopping to same site.
	if (value >= *positionPointerB) {
		value++;

	}

	*positionPointerB = value; //the particle has now moved to value

	// if position is at B+1, then it has to move 1 cluster up.
	if (*positionPointerB == (B + 1)) {
		*positionPointerB = 1;
		*positionPointerA = *positionPointerA + 1;
		//if has moved past the last cluster, it will move back to first cluster
		if (*positionPointerA == (A + 1)) {
			*positionPointerA = 1;
		}
	}

	// if it has moved to zero, it will move down a cluster
	if (*positionPointerB == 0) {
		*positionPointerB = B;
		*positionPointerA = *positionPointerA - 1;
		//if it has moved down cluster 1, it will loop back to cluster A.
		if (*positionPointerA == 0) {
			*positionPointerA = A;
		}

	}

}

void
main()
{

	int A;				//clusters
	int B;				//sites in each cluster
	int ALimit;
	int BLimit;
	int Blower;
	int printSites;		//used for data print style
	int seedUpperLimit;	//seed for random number generator
	int seedLowerLimit;
	int randomseed;
	int history[200000][2]; //used to print walk
	double Average = 0.0;	//average hops to visit every sites
	int position[2];
	int possibleJumpPos;
	int vistedAll = 1;
	int hasItVisitedAll = 0;
	int count = 0;
	long double total = 0.0;
	//1. assigning Variables

	//setting up A
	printf("Enter A\n");
	scanf("%d", &A);
	if (1 > A) {
		printf("Error: 1 is the minimum value for A\n");
		exit(0);
	}

	printf("Enter A upper Limit\n");
	scanf("%d", &ALimit);
	if (A > ALimit) {
		printf("Error: Enter value bigger or equal to A\n");
		exit(0);
	}


	// setting up B
	printf("Enter B\n");
	scanf("%d", &Blower);
	if (2 > Blower) {
		printf("Error: 2 is the minimum value for B\n");
		exit(0);
	}

	printf("Enter B upper limit\n");
	scanf("%d", &BLimit);
	if (B > BLimit) {
		printf("Error: Enter value bigger or equal to B\n");
		exit(0);
	}

	//setting up seed
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
		printf("Error: seed must be greater or equal to 1.\n 0 for random seed.\n");
		exit(0);
	}
	if (seedLowerLimit == 0) {
		srand(time(NULL));
		seedLowerLimit = (rand()% 99) + 1;
	}
	randomseed = seedLowerLimit;
	seedUpperLimit = seedLowerLimit + repeats;


	//choice between 3 types of data output. 
	printf("Print the Path the particle takes\n");
	printf("Or print a table of A and B against seed\n");
	printf("Or print the average number of hops on a table of A against B\n");
	printf("1: for sites, 2: A,B against seeds, 3: A against B averaged\n");
	printf("For option 3, the seed will not be repeated.\n");
	scanf("%d", &printSites);


	if (1 > printSites || printSites > 3) {
		printf("Error: Enter 1 or 2 or 3\n");
		exit(0);
	}

	// 2. Setting up File

	FILE* fp;
	fp = fopen("data.txt", "w");

	if (printSites == 1) {
		fprintf(fp, "seeds\tA\tB\tHopsToVisitAll\tSites\n");
	}

	if (printSites == 2) {
		fprintf(fp, "\t\tSeed\nA\tB");
		for (int i = seedLowerLimit; i <= seedUpperLimit; i++) {
			fprintf(fp, "\t%d", i);
		}
	}

	if (printSites == 3) {
		fprintf(fp, "Seed:%d\n", seedLowerLimit);
		fprintf(fp, "Avg\tB\nA");
		for (int i = Blower; i <= BLimit; i++) {
			fprintf(fp, "\t%d", i);
		}
	}

	//loading setup and visited check setup
	int loading = 0;
	int AlowerLimit = A;
	int visited[ALimit][BLimit];

	//3. Loops for calculating and printing
	for (A; A <= ALimit; A++) {

		// option 3 1st column
		if (printSites == 3) {
			fprintf(fp, "\n%d", A);
		}

		for (B = Blower; B <= BLimit; B++) {

			//loading
			printf("%d/%d done. on A:%d, B:%d\n", loading, (ALimit - AlowerLimit + 1) * (BLimit - Blower + 1) * (repeats + 1), A, B);

			//option 3 resetting average 
			if (printSites == 3) {
				Average = 0.0;
			}

			//option 2 1st column
			if (printSites == 2) {
				fprintf(fp, "\n%d\t%d", A, B);
			}

			for (int seed = seedLowerLimit; seed <= seedUpperLimit; seed++) {
				int totalNoSites = A * B;

				//3a) resetting variables

				//option 3, seed is not repeated as it skews data
				if (printSites == 3) {
					srandom(randomseed);
					randomseed++;
				}
				else {
					srandom(seed);
				}

				position[0] = 1;	//A
				position[1] = 1;	//B
				loading++;
				for (int i = 0; i < A; i++) {
					for (int j = 0; j < B; j++) {
						visited[i][j] = 0;
					}
				}

				visited[position[0] - 1][position[1] - 1] = 1;
				vistedAll = 1;
				hasItVisitedAll = 0;
				count = 0;

				history[count][0] = position[0];
				history[count][1] = position[1];

				//option 1 first 3 column
				if (printSites == 1) {
					fprintf(fp, "\n%d\t%d\t%d", seed, A, B);
				}

				//3b. random number

				while (hasItVisitedAll == 0) {
					//calculating the number of possible sites it can jump to.
					//position 1 and B can jump to other clusters which has 1 more possibility, when compared to other B sites
					if (position[1] == 1 || position[1] == B) {
						possibleJumpPos = B;
					}
					else
					{
						possibleJumpPos = B - 1;
					}

					//3c. call move function
					move(position, randomInt(possibleJumpPos), A, B);

					count++;	//1 hop done

					//option 1, saves the position in history 
					if (printSites == 1) {
						history[count][0] = position[0];
						history[count][1] = position[1];
					}

					// 3d. check and print
					// if not visited site yet, add one and check if it has visited all sites.
					if (visited[position[0] - 1][position[1] - 1] == 0) {
						vistedAll++;
						visited[position[0] - 1][position[1] - 1] = 1;
						//option 1,2 if visited all sites, print hops on data.txt.
						if (vistedAll == totalNoSites) {
							hasItVisitedAll = 1;
							if (printSites != 3) {
								fprintf(fp, "\t%d", count);
							}
							if (printSites == 3) {
								Average += count;				
							}
						}
					}
				}

				//option 1, print all the sites it visited
				if (printSites == 1) {
					for (int j = 0; j <= count; j++) {
						fprintf(fp, "\t(%d,%d)", history[j][0], history[j][1]);
					}
				}

			}
			//option 3, find average and print that
			if (printSites == 3) {
				total = Average / (repeats + 1);
				fprintf(fp, "\t%Lf", total);
			}
		}
	}
	fclose(fp);		//closes data.txt
	printf("%d/%d done.\n", loading, (ALimit - AlowerLimit + 1) * (BLimit - Blower + 1) * (repeats + 1));
	printf("done. look for file inside the folder which this program lies. File name is data.txt\n");
	printf("seed:%d\n", seedLowerLimit);
	//loading

}