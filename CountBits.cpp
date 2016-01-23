#include <iostream>
using namespace std;

int countBits(unsigned int test)
{
    int m, bits = 0;
	
    unsigned int mask = 1;
	
    for (m = 0; test != 0; m++)
	{
	    if ((test & mask) != 0)
	    {
	        bits++;
	    }
	    test = test >> 1;
	}
	
	printf ("number of iterations %d \n",  m);
	
	return bits;
}

int CountBitsTwo (unsigned int test)
{
    
}

int main() {
	//code
	int i = 0;
	int j = ~i;
	
	int k = -2;
	
	printf ("~ ouptut is %d \n", ~i);
	
	unsigned int test = k;
	
	int bits = countBits(test);
	
	printf ("number of bits %d \n", bits);
	
	return 0;
}