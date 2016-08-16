#include"Bloom_Filter.h"


void Bloom::add(char* url)
{
	unsigned long long hash64 = XXH64(url, strlen(url), 12345);
	unsigned long long hash1 = hash64;
	unsigned long long hash2 = (hash64 >> 32);
	for (int i = 1; i <= 6; i++)
	{
		int hash = (hash1 + (i*hash2))%m;
		set[hash] = 1;
		//cout << hash << endl;
	}
}


bool Bloom::contain(char* url)
{
	unsigned long long hash64 = XXH64(url, strlen(url), 12345);
	unsigned long long hash1 = hash64;
	unsigned long long hash2 = (hash64 >> 32);
	for (int i = 1; i <= 6; i++)
	{
		int hash = (hash1 + (i*hash2)) % m;
		if (!set[hash])return false;
	}

	return true;
}


