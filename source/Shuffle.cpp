/*
 * Copyright 2017 Akshay Agarwal <agarwal.akshay.akshay8@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "Shuffle.h"


void 
Swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}


void 
Randomize(int* arr, int n)
{
	srand ( time(NULL) );

	for(int i = n - 1; i > 0; i--) {
		int j = rand() % (i+1);
		Swap(&arr[i], &arr[j]);
	}
}


void
CreateRandomSeq(int* arr, int len)
{
	for(int i = 0; i < len; ++i)
		arr[i] = i;
	Randomize(arr, len);
}
