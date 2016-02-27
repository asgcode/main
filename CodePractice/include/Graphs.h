#include <vector>

using namespace std;

typedef vector<int> VINT;
typedef unsigned int UINT;

class UndirectedGraph
{
    UINT     m_Vertices;
    UINT     m_Edges;
    VINT*    m_Adj;

public:

    UndirectedGraph(int* inBuf)
    {
        UINT inCount = 0;

        m_Vertices = inBuf[inCount++];
        m_Edges    = inBuf[inCount++];

        m_Adj = new VINT[m_Vertices];

        for (UINT i = 0; i < m_Edges; i++)
        {
            m_Adj[inBuf[inCount]].push_back(inBuf[inCount+1]);
            m_Adj[inBuf[inCount+1]].push_back(inBuf[inCount]);
            inCount += 2;
        }
    }

    ~UndirectedGraph()
    {
            delete[] m_Adj;
    }

    void PrintEdges()
    {
        for (UINT i = 0; i < m_Vertices; i++)
        {
            for (VINT::iterator it = m_Adj[i].begin(); it != m_Adj[i].end(); ++it)
            {
                printf("Edge from %d to %d \r\n", i, *it);
            }
        }
    }

    VINT* GetGraphAdjList()
    {
        return m_Adj;
    }
};