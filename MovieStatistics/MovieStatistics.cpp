// MovieStatistics.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <time.h>
using namespace std;

bool TryParse(string toParse, int &value);	//input validation module
int *MakeArray(int size);	//module to allocate memory for an array
int GetMode(int arrayptr[], int size);	//module to retrieve the mode of an int array
int GetHighest(int[], int);	//module to retrieve the highest value of an int array, used with the GetMode module
double GetMedian(int arrayptr[], int size);	//module to retrieve the median of an int array
double GetAverage(int[], int);	//module to retrieve the average of an int array
static void Sort(int[], int);	//sorting algorithm for an int array
void FillArray(int empty[], int);	//module to fill an int array with random numbers

int main()
{
	int size = 10;	//default size
	//variable declarations
	int arrayMode;
	double arrayMedian;
	double arrayAverage;
	string input;
	//random number seeding if array is autofilled
	unsigned seed = time(0);
	srand(seed);
	bool valid = false;
	while (!valid)	//comment out this loop to use the default size
	{
		cout << "\nEnter the number of students surveyed: ";		
		getline(cin, input);
		if (TryParse(input, size) && size > 0)	//validate user input
			valid = true;
		else
			cout << "\nPlease enter only a positive integer for the number of students.";
	}
	int *arrayPtr = nullptr;	//creating pointer for the primary array
	arrayPtr = MakeArray(size);	//call the module to allocate space for the primary array
	for (int i = 0; i < size; i++)	//loop to fill the array with user values
	{
		valid = false;
		while (!valid)
		{
			int moviesSeen;
			cout << "\nEnter the number of movies seen by student " << i + 1 << ": ";
			getline(cin, input);
			if (TryParse(input, moviesSeen) && moviesSeen > 0)	//validate user input
			{
				valid = true;
				*(arrayPtr + i) = moviesSeen;	//assign user input to the array element
			}
			else
				cout << "\nPlease enter only positive integers for the number of movies seen by the student.";
		}
	}
	//FillArray(arrayPtr, size);  //comment out the above loop to auto fill the array with random numbers
	arrayMode = GetMode(arrayPtr, size);	//get the array mode
	if (arrayMode == -1)	
		cout << "\nThis array has no Mode.";	//output if array has no mode
	else
		cout << "\nThe array Mode is " << arrayMode << endl;
	arrayMedian = GetMedian(arrayPtr, size);
	cout << "\nThe array Median is " << arrayMedian << endl;
	arrayAverage = GetAverage(arrayPtr, size);
	cout << "\nThe array Average is " << arrayAverage << endl;
	cin.get();	//pause the program
    return 0;
}

int *MakeArray(int size)
{
	int * arrayptr = nullptr;
	arrayptr = new int[size];	//allocate memory for an array
	return arrayptr;
}

void FillArray(int empty[], int size)
{
	const int MAX = 10;	//random number bounds
	const int MIN = 1;
	for (int i = 0; i < size; i++)	//loop to fill the passed in array with random ints
	{
		*(empty + i) = (rand() % ((MAX - MIN) + 1)) + MIN;
		//cout << empty[i] << " ";	
	}
}

int GetHighest(int findHighest[], int size)
{
	int highest = 0;
	for (int i = 0; i < size; i++)
	{
		if (findHighest[i] > highest)
			highest = findHighest[i];
	}
	return highest + 1;	//add one because an array size is returned
}

int GetMode(int arrayptr[], int size)
{
	int mode = -1;	//flag value we use if there is no mode
	int highest = GetHighest(arrayptr, size);	//get the highest value to use as a frequency array
	int * frequency = nullptr;
	frequency = new int[highest];	//allocate memory for the array
	for (int i = 0; i < highest; i++)	//initialize the frequency array
	{
		*(frequency + i) = 0;
	}
	for (int i = 0; i < size; i++)
	{
		frequency[*(arrayptr + i)]++;	//increment the subscript in the frequency array that matches our primary array
	}
	int curr = 0, prev = 0;	//tracking variables
	for (int i = 0; i < highest; i++)	//loop the frequency array to determine the highest value
	{
		curr = *(frequency + i);	//assign curr to the element we want to inspect
		if (curr > prev)	//check it against the previous highest frequency count
		{
			prev = curr;	//if curr count is higher, set prev to the new highest count
			mode = i;	//set mode to the highest count, using the frequency array subscript to determine the value
		}
		else if (curr == prev)	//if current highest frequency matches previous highest, reset mode flag
		{
			mode = -1;
		}
	}
	delete[] frequency;	//release frequency array memory
	return mode;
}

double GetMedian(int arrayptr[], int size)
{
	double median;
	int medianElement = size / 2;	//determine the median element, rounding down is ideal for odd size array subscripts
	Sort(arrayptr, size);	//ensure the array is sorted
	//for (int i = 0; i < size; i++)
	//	cout << arrayptr[i] <<  " ";
	if (size % 2 == 0)	//determine if the array size is even
	{
		median = (double)(*(arrayptr + medianElement -1) + *(arrayptr + medianElement)) / 2;	//average the two middle values
	}
	else
	{
		median = *(arrayptr + medianElement);
	}
	return median;
}

double GetAverage(int arrayptr[], int size)
{
	int total = 0;
	double average;
	for (int i = 0; i < size; i++)
		total += *(arrayptr + i);
	average = (double)total / size;	//casting the int total so the remainder is included
	return average;
}

bool TryParse(string toParse, int &value)	//parsing validation function for integers
{
	try
	{
		stoi(toParse);
		value = stoi(toParse);
		return true;
	}
	catch (exception ex)
	{
		return false;
	}
}

static void Sort(int old[], int size)	//radix sort
{
	int i, j;
	int * temp = nullptr;
	temp = new int[size];	//create a temporary array to hold bits with value 1 while sorting
	for (int shift = 31; shift > -1; shift--)	//loop through the 32 bits of the int
	{
		j = 0;
		for (i = 0; i < size; i++)
		{
			bool move = (old[i] << shift) >= 0;	//bit shift the int to determine if the current analyzed bit is a 0 or 1
			if (shift == 0 ? !move : move)	//determine if the array element needs to be moved in the primary array or to the temp array
				old[i - j] = old[i];	//move the element to the appropriate open spot in the primary array if bit is 0
			else			
				temp[j++] = old[i];		//move the element to the temp array is the bit is 1			
		}
		int tempPos = 0;	//counter variable
		for (int l = i - j; l < size; l++)	//fill the end of the primary array with the values in the temp array
		{
			old[i - j + tempPos] = temp[tempPos++];	
		}
	}
	delete[] temp;	//release memory of the temp array
}