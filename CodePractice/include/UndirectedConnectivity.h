#include "Graphs.h"
#include <vector>


using namespace std;

class UndirectedConnectivity
{
    vector<bool> m_Marked;
    vector<int>  m_Ids;

public:
    UndirectedConnectivity(UndirectedGraph graph)
    {


    }

    bool IsConnected(int v, int w);
};