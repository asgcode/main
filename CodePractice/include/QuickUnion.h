#ifndef QUICK_UNION
#define QUICK_UNION

#include<stdlib.h>

class QuickUnion
{
    int* Id;
    int  size;

public:
    QuickUnion(int n)
    {
       Id = (int*) calloc(n, sizeof(int));

       for (int i = 0; i < n; i++)
       {
           Id[i] = i;
       }

       size = n;
    }

    ~QuickUnion()
    {
        delete Id;
    }

    int root(int x)
    {
        while (Id[x] != x)
        {
            x = Id[x];
        }

        return x;
    }

    void Union(int x, int y)
    {
       int xRoot = root(x);
       int yRoot = root(y);

       Id[xRoot] = yRoot;
    }

    bool Connected(int x, int y)
    {
        return (root(x) == root(y));
    }

};

#endif //QUICK_UNION
