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

	for (int index = 2; index <= numbertoTest; index++)
	{
		if (numbertoTest % index == 0)
		{
			return false;
		}
		return true;
	}
}
int input5CharsConvertToInt()
{
	int returnInt = 0;
	char inputChar;
	for (int i = 0; i < 5; i++)
	{
		std::cin >> inputChar;
		//check if its a digit.
		//do something

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

}
void drawIsocelesTriangle()
{

}
void drawIsocelesTriangle2()
{

}

int find(int size, int arr[], int toFind)
{
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