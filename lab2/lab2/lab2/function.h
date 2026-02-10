// Name: Nikola Prusac Manahy
// Student ID: C00309098

#pragma once
#include <iostream>

bool isLeapYear(int year)
{
	if (year % 4 == 0)
	{
		if (year % 100 == 0)
		{
			if (year % 400 == 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}

int Reversed(int testNumber)
{
	int reversed = 0;
	while (testNumber > 0)
	{
		reversed = reversed * 10 + testNumber % 10;
		testNumber = testNumber / 10;
	}
	return reversed;
}
bool isAPalindrome(int testNumber)
{
	return testNumber == Reversed(testNumber);
}
bool isAPrimeNumber(int numbertoTest)
{
	if (numbertoTest <= 1)
		return false;

	for (int index = 2; index <= numbertoTest / 2; index++)
	{
		if (numbertoTest % index == 0)
		{
			return false;
		}
	}
	return true; 
}
int input5CharsConvertToInt()
{
	int returnInt = 0;
	char inputChar;
	for (int i = 0; i < 5; i++)
	{
		std::cin >> inputChar;
		//check if its a digit.
		if (inputChar < '0' || inputChar > '9')
		{
			return 0;
		}

		returnInt = returnInt * 10 + (inputChar - '0');
	}
	return returnInt;
}
int convertBinarytoDecimal(int binaryNumber)
{
	int decimal = 0;
	int base = 1;

	while (binaryNumber > 0)
	{
		int lastDigit = binaryNumber % 10;
		decimal = decimal + lastDigit * base;
		base = base * 2;
		binaryNumber = binaryNumber / 10;
	}
	return decimal;
}
void drawRightAngledTriangle()
{
	for (int index = 1; index <= 4; index++)
	{
		for (int j = 0; j < index; j++)
		{
			std::cout << "A";
		}
		std::cout << std::endl;
	}
}
void drawIsocelesTriangle()
{
	int row = 1;
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < row; j++)
		{
			std::cout << 'A';
		}
		std::cout << std::endl;

		if (i < 3)
			row++;
		else
			row--;
	}
}
void drawIsocelesTriangle2()
{

}

int find(int size, int arr[], int toFind)
{
	for (int index = 0; index < size; index++)
	{
		if (arr[index] == toFind)
			return index;
	}
	return -1;           
}
int find2ndLargest(int size, int arr[])
{
	return -1;
}
void copyArraytoArray(int size, int arr1[], int arr2[])
{
	return;
}
bool insertElement(int& size, int& count, int arr[], int elementToInsert, int insertIndex)
{

	return false;
}
bool deleteElement(int& size, int& count, int arr[], int deleteIndex)
{

	return true;
}
int frequencyCount(int size, int arr[], int value)
{

	return 0;
}
int countDuplicates(int size, int arr[])
{

	return 0;
}
void reverse(int size, int arr[])
{
	return;
}
int rotateLeft(int size, int arr[])
{
	return -1;
}
bool twoMovies(int flightLength, int movieLengths[], int size)
{
	return false;
}
int wordCounter(int size, char characters[])
{
	return 0;
}