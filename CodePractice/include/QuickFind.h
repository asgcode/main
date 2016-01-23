#ifndef QUICK_FIND
#define QUICK_FIND

#include<stdlib.h>

class QuickFind
{
    int* Id;
    int  size;

public:
    QuickFind(int n)
    {
       Id = (int*) calloc(n, sizeof(int));

       for (int i = 0; i < n; i++)
       {
           Id[i] = i;
       }

       size = n;
    }

    ~QuickFind()
    {
        delete Id;
    }

    void Union(int x, int y)
    {
       int xNode = Id[x];
       int yNode = Id[y];

       for (int i = 0; i < size; i++)
       {
           if (Id[i] == xNode)
           {
               Id[i] = yNode;
           }
       }
    }

    inline bool Connected(int x, int y)
    {
        return (Id[x] == Id[y]);
    }

};

#endif //QUICK_FIND
