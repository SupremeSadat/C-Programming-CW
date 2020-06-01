#include<stdio.h>
#include<math.h>
#include<stdlib.h>

double roundtosigfig(double number, int sigfig) { // rounds to signigicant figures.
	double N;
	if (number == 0.0) {        // log10 of 0 is impossible. so this checks before preceeding
		return 0.0;
	}
	N = log10(fabs(number));    // log 10 of negative number is also impossible. must first take absoulte 
	N = floor(N);               // eg log10(1234567) = 6.09-->floor(6.09)=6
	number = number * pow(10, (sigfig - N - 1));   // this is to make the number sigfig long before the decimal. eg 12345.567
	number = round(number);             // removes everything past the decimal. eg 12345.000
	number = number * pow(10, -(sigfig - N - 1)); // restores it back to 12345000
	return number;
}

double funct(double x) //this is equation we are intergrating
{
	double a;
	if (x == 0)
	{
		a = 0;
		return a;
	}
	a = sin(x) / sqrt(x);
	return a;
}

void main()
{

	double y1, yn;		// yn = f(xn)
	int TrapsA_Unit;	//number of trapaziods you want per a unit.
	double n;			//number of points
	double counter;		//counter = n, used to count down. could also change code to count up. makes no difference.
	double height;		//total height. this is y2 +y3+y4..+y(n-1)
	double Xn;			//last x value
	double deltaX;		//base of traps
	double answer;		// this is the answer calculated the intergral from 0 to Xn
	double X1;			// first x value. i set it as 0 later.
	double Average;		//because how this graph oscillates, i can take x1, and x1+pi and take there average to find the solution for this equation.
	int sigfig;

	Average = 0;

	printf("Welcome!!\nThis is my program to work out the definite integral of Sin(x)/sqrt(x).\n");
	printf("This program uses the Trapezoidal rule to work out the intergral.\n");
	printf("Because this intergral oscillates, it would take very large values of x to \nget close to the definite intergral.\n");
	printf("But because I know it oscillates at a period of 2 pi, I can take 2 integrals \npi apart to get very close to the definite integral.\n");


	printf("And that's what my code does!\n\nLets start by entering the value you wish to integrate to.\n");
	scanf("%lf", &Xn);

	if (Xn < 0.01) {
		printf("Xn is too small or negative which is invalid.\n");
		exit(0);
	}

	printf("Now how many trapezoids would you like to make per a unit. \neg Entering 10 would give trapazoids 0.1 in base (x axis).\n");
	scanf("%d", &TrapsA_Unit);
	if (TrapsA_Unit < 1) {
		printf("Thats too small!(<1) Need atleast 1 Trapeziod.\n");
		exit(0);
	}

	printf("How many sigfigs?\n");
	scanf("%d", &sigfig);
	if (sigfig < 1) {
		printf("sigfigs must be more than 1.\n");
		exit(0);
	}
	if (sigfig > 8) {
		printf("sigfigs must be less then 8.\n");
		exit(0);
	}




	X1 = 0;						//This is x1. Its simply to future proof code. could of just not included it. Or ask user for input.


	for (int i = 0; i < 2; i++) // does this part 2 times to get 2 integral pi apart
	{




		n = Xn * TrapsA_Unit;	//n is the number of trapezium PER A UNIT.
		
		height = 0;				//height is value set as 0 to prevent it calling memory location as it is called before setting a value.
		deltaX = (Xn-X1) / n;
		counter = X1 + deltaX;	//counter counts up until it reaches Xn
		
		y1 = funct(X1);			//y1 is zero in this case. just future proofing code.
		yn = funct(Xn);			//last y value calculated

		do						//this part calculates total height.
		{
			height += 2 * funct(counter+X1);		//This is adding the height of all the trapeziums
			counter += deltaX;
		} while (counter < Xn);


		if (TrapsA_Unit == 1) {
			height = 0;
		}

		answer = (deltaX / 2) * (y1 + yn + height); // This completes the Trapezoidal rule equation


		printf("The integral from 0 to %g is %g\n", Xn, roundtosigfig(answer,sigfig));

		Average = Average + answer;

		if (Xn < 3.14159265) {
			printf("The integral you have chosen to integrate to is less then pi. Average not used.\n");
			exit(0);
		}

		Xn += 3.14159265;
	}


	Average = Average * 0.5; // take average of 2 points pi apart to get correct answer

	
	printf("The answer is heading towards the value %g\n", roundtosigfig(Average, sigfig));

}
