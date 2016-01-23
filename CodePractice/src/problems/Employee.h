#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "Utils.h"

class Employee
{
    char  m_Name[100];
    char  m_ManagerName[100];
    char* m_ReportNames[100];

    UINT m_NumReportee;

public:
    Employee(char* pName, char* pManagerName, char** ppReportNames, int numReportee)
    {
        assert(pName != NULL);

        strcpy(m_Name, pName);

        if (pManagerName != NULL)
        {
            strcpy(m_ManagerName, pManagerName);
        }
        else
        {
           strcpy(m_ManagerName, "\0");
        }

        m_NumReportee = numReportee;

        for (UINT i = 0; i < m_NumReportee; i++)
        {
            m_ReportNames[i] = (char*) malloc(sizeof(char)*strlen(ppReportNames[i]));
            assert(m_ReportNames[i] != NULL);
        }
    }

    ~Employee()
    {
        for (UINT i = 0; i < m_NumReportee; i++)
        {
            free(m_ReportNames[i]);
        }
    }

    const char* getName(void) const
    {
        return m_Name;
    }

    const char* getManagerName(void) const
    {
        return m_ManagerName;
    }

    char** getReporteeList(void)
    {
        return m_ReportNames;
    }

    const int getNumReportee() const
    {
        return m_NumReportee;
    }

};
