/*
 * Copyright 2017 Akshay Agarwal <agarwal.akshay.akshay8@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "Shuffle.h"


// don't use random_shuffle deprecated in the future...

void 
Randomize(std::vector<int> &arr, int n)
{
	srand ( time(NULL) );

	for(int i = n - 1; i > 0; i--) {
		int j = rand() % (i+1);
		std::swap(arr[i], arr[j]);
	}
}


void
CreateRandomSeq(std::vector<int> &arr, int len)
{
	for(int i = 0; i < len; ++i)
		arr.push_back(i);
	Randomize(arr, len);
}
