#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <signal.h>

#define BADARGS 1 // bad combination of command line arguments
#define BADLSTF 2 // bad list file 
#define BADVALS 3 // no number-of-values located

// VALUES abstract data type
typedef struct valtok {
	char *var; // value
	float prob; // probability of the value
} VALUES; 

float infocont(float x); // function I(x)
float entropy(VALUES *, int); // function H(x)
void onerror(const char *, const int); // error handler
void onsig(int); // SIGSEGV handler

int main(int argc, char *argv[])
{
	
	/* On a bad argument combination */
	if (argc != 2) {
		onerror("*invalid combination of arguments\n\t*usage: entropy <list file>", BADARGS);
	}
	
	/* to handle the sigsegv */
	signal(SIGSEGV, onsig);
	
	FILE *lstf = NULL; // list file
	int vals = 0; // number of values
	int i = 0; // general counter
	
	// try to open the list file
	if ((lstf = fopen(argv[1], "r")) == NULL)
		onerror("*error opening list file\n", BADLSTF);
	
	// try to read the valtok and vals
	if ((fscanf(lstf, "(vars) %d\n", &vals) != 1))
		onerror("unable to locate the number of values", BADVALS);

	// defining values array
	VALUES values[vals];
	
	// reading values into the array
	for (i = 0; i < vals; i++) {
		values[i].var = malloc(50 * sizeof (char));
		fscanf(lstf, "{%s : %f}\n", values[i].var, &values[i].prob);
	}
	
	//FOR DEBUGGING PURPOSES// 
	// for (i = 0; i < vals; i++) {
		// printf("%s %f\n", values[i].var, values[i].prob);
	// }
	//FOR DEBUGGING PURPOSES//
	
	// print the result
	float H = entropy(values, vals);
	printf("entropy is %f\n", H);
		
}

// call "onsig" on SIGSEGV
void onsig(int signo)
{
	fprintf(stderr, "signal caught: %d\nSEGMENTATION FAULT\n", signo);
}

// general error handler
void onerror(const char *errs, const int eno)
{
	printf("error occured:\n\t%s\n\tERROR_CODE: ENOx%d\n", errs, eno);
	exit(eno);
}

// CORE FUNCTION, this calculates the entropy
float entropy(VALUES values[], int len)
{
	short i; // counter
	float entropy = 0.0; // entropy of the values array
	float infocont(float); // information-content I(x) function
	
	// calculate entropy
	for (i = 0; i < len; i++) {
		entropy += values[i].prob * infocont(values[i].prob);
	}
	
	// return calculated entropy 
	return entropy;
}

// information content I(x) function
float infocont(float x) 
{
	// return log2(1/x)
	return (log(1/x) / log(2));
}

