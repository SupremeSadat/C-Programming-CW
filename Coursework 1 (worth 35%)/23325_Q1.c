#include<stdio.h>
#include<math.h>
#include<stdlib.h>

double func(double r) { // this is f(r)
	double a;
	double M = 5.974E24; //Mass of Earth
	double m = 7.348E22; //Mass of Moon
	double R = 3.844E8;	 //distance from Earth to Moon
	double w = 2.662E-6; //angular velocity
	double G = 6.674E-11; // gravitational constant
	a = -(pow(w, 2) * r) + ((G * M) / pow(r, 2)) - ((G * m) / pow((R - r), 2)); // equation made in part 1(a)
	return a;
}


// This is the secant function. takes 4(x1,x2,y1,y2) values and outputs x3
double secant(double x1, double x2, double y1, double y2) {
	double a;
	a = x2 - (y2 / ((y2 - y1) / (x2 - x1)));        // secant equation,
	return a;
}


double roundtosigfig(double number, int sigfig) {   // rounds to signigicant figures.
	double N;
	if (number == 0.0) {                            // log10 of 0 is impossible. so this checks before preceeding
		return 0.0;
	}
	N = log10(fabs(number));                        // log 10 of negative number is also impossible. must first take absoulte 
	N = floor(N);                                   // eg log10(1234567) = 6.09-->floor(6.09)=6 prevents rounding error
	number = number * pow(10, (sigfig - N - 1));    // this is to make the number "sigfig long" before the decimal. eg 12345.567
	number = round(number);                         // removes everything past the decimal. eg 12345.000
	number = number * pow(10, -(sigfig - N - 1));   // restores it back to 12345000
	return number;
}






void main()
{

	double x1, x2, y1, y2, x3, dummyx1, dummyx2;
	int n = 2;// n is the counter for x.


	printf("Welcome! \nThis code uses the secant method to calculate the Lagrangian point\nPlease enter the value for x1 \n");
	scanf("%lf", &x1);
	printf("Good! Now enter the value of x2 \n");
	scanf("%lf", &x2);


	if (x1 == x2 || fabs(x1) < 0.05 || fabs(x2) < 0.05) {
		printf("x1 and x2 must be different! And not ZERO or too close to ZERO!!   (and a an actual number)\n");
		exit(0);
	}




	y1 = func(x1);
	y2 = func(x2);
	printf("The 1 x is %.6gm and the y is %.6g\n", x1, y1);
	printf("The 2 x is %.6gm and the y is %.6g\n", x2, y2);



	do {

		x3 = secant(x1, x2, y1, y2);
		n++;                            //this is the number of x values.




		x1 = x2;                        // now x3 --> x2 and x2--->x1 so can loop
		y1 = y2;
		x2 = x3;
		y2 = func(x2);
		printf("The %d x is %.6gm and the y value is %.5g\n", n, x3, y2);


		// fail safe incase secant leads to infinite loop or dead end.
		if (n > 1001) {
			printf("loop has exceeded limit of x1000\nTry another x1 and x2 value.\n");
			exit(0);
		}


		dummyx1 = roundtosigfig(x1, 7); //this is done so program doesnt excessivly keep going untill all 15 digits are equal.
		dummyx2 = roundtosigfig(x2, 7); //we are only trying to find answer to 5 sigfigs, or 7 sigfig checks are good enough.
	} while (dummyx1 != dummyx2);

	//printf("The dummyx2 is %lfm \n",dummyx2); // kept for debugging
	//printf("The dummyx1 is %lfm \n",dummyx1);


	printf("The value of the lagrange point is %.5gm From Earth, in the direction from the Earth to the Moon\n", x3);


	if (n < 5) { // This checks number of iterations. if it is low it maybe that the x1 or x2 = Legrange point. or x1 or x2 is about = 0 or just too big
		printf("\nThe number of iterations is low, maybe try changing the starting values.\nIf the value is too close to 0, the secant method can fail due to rounding error.\nOr maybe you already entered the correct value for the root!\nOr x1 or x2 could be too big!\n");
		exit(0);
	}
}