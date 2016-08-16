#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H
#include<iostream>
#include<string.h>
#include<vector>
#include<algorithm>
#include"xxhash.h"
#include<bitset>
using namespace std;



class Bloom
{
	const static unsigned long int n = 1000000;
	const static unsigned long int m = 8142363;
	int k = 6;
	bitset<m>set;
public:
	Bloom()
	{
		
	}

	void add(char* url);
	bool contain(char* url);

};


















#endif