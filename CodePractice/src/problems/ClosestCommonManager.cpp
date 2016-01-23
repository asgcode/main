#include "Employee.h"
#include "stdio.h"

// Build the organization and return the root
Employee* BuildOrganizationTree()
{
    char BillReportee[3][100] = {"Dom", "Samir", "Michael"};
    Employee* Bill = new Employee((char*) "Bill", (char*) NULL, (char**) BillReportee, 3);

    char DomReportee[3][100] = {"Bob", "Peter", "Porter"};
    Employee* Dom = new Employee((char*) "Dom", (char*) NULL, (char**) DomReportee, 3);

    char PeterReportee[3][100] = {"Milton", "Nina"};
    Employee* Peter = new Employee((char*) "Peter", (char*) NULL, (char**) DomReportee, 2);

    return Bill;
}

Employee* FindClosestCommonManager(Employee* root, char* pFirstEmployeeName, char* pSecondEmployeeName)
{
    return NULL;
}

void TestClosestCommonManager()
{
    Employee* root = BuildOrganizationTree();

    if (strcmp(FindClosestCommonManager(root, "Miton", "Nina")->getName(), "Peter") == 0)
    {
        printf("Test One Successfull \n");
    }

    if (strcmp(FindClosestCommonManager(root, "Nina", "Porter")->getName(), "Dom") == 0)
    {
        printf("Test Two Successfull \n");
    }

    if (strcmp(FindClosestCommonManager(root, "Nina", "Samir")->getName(), "Bill") == 0)
    {
        printf("Test Three Successfull \n");
    }

    if (strcmp(FindClosestCommonManager(root, "Peter", "Nina")->getName(), "Peter") == 0)
    {
        printf("Test Three Successfull \n");
    }
}