#ifndef TEST_CASES
#define TEST_CASES

#include <stdio.h>

#include "Arrays.h"
#include "BitsMani.h"
#include "Graphs.h"
#include "MultiThreading.h"
#include "MyString.h"
#include "QuickFind.h"
#include "QuickSort.h"
#include "QuickUnion.h"
#include "Sorting.h"
#include "Stack.h"
#include "Utils.h"


void TestQuickFind(void)
{
    QuickFind* quickFindObj = new QuickFind(10);

    quickFindObj->Union(1,4);
    quickFindObj->Union(4,5);
    quickFindObj->Union(5,7);
    quickFindObj->Union(7,8);
    quickFindObj->Union(8,9);
    quickFindObj->Union(0,2);
    quickFindObj->Union(0,3);

    printf("Is 2 & 3 connected %d \n", quickFindObj->Connected(2,3));
    printf("Is 3 & 4 connected %d \n", quickFindObj->Connected(3,4));
    printf("Is 5 & 6 connected %d \n", quickFindObj->Connected(5,6));
    printf("Is 1 & 9 connected %d \n", quickFindObj->Connected(1,9));

    delete quickFindObj;
}

void TestQuickUnion(void)
{
    QuickUnion* quickUnionObj = new QuickUnion(10);

    quickUnionObj->Union(1,4);
    quickUnionObj->Union(4,5);
    quickUnionObj->Union(5,7);
    quickUnionObj->Union(7,8);
    quickUnionObj->Union(8,9);
    quickUnionObj->Union(0,2);
    quickUnionObj->Union(0,3);

    printf("Is 2 & 3 connected %d \n", quickUnionObj->Connected(2,3));
    printf("Is 3 & 4 connected %d \n", quickUnionObj->Connected(3,4));
    printf("Is 5 & 6 connected %d \n", quickUnionObj->Connected(5,6));
    printf("Is 1 & 9 connected %d \n", quickUnionObj->Connected(1,9));

    delete quickUnionObj;
}

void TestStack()
{
    Stack stackObject;

    Node* a = new Node("Amit");
    Node* b = new Node("Bansal");
    Node* c = new Node("9961");

    stackObject.push(a);
    stackObject.push(b);
    stackObject.push(c);

    printf("%s \n", stackObject.pop()->string);
    printf("%s \n", stackObject.pop()->string);
    printf("%s \n", stackObject.pop()->string);

    delete a;
    delete b;
    delete c;
}

void TestQuickSort()
{
    int arr[] = {2, 3, 8, 5, 6};
    int size  = (sizeof(arr)/sizeof(arr[0]));

    PrintArray(arr, size);

    QuickSort(arr, 0, (size -1));

    PrintArray(arr, size);
}

void TestMultiThreading()
{
    //printOddEvenBusyWait();
    PrintOddEvenNoBusyWait();
    //printOddEvenWithoutSharedMemWithSemaphore();
}

void TestStringToInteger()
{
    printf("result %d \n", StringToInt("-229878973"));
}

void TestNonRepeatingChar()
{
    printf("First Non repeating char is %c \n",findFirstNonRepeatingChar("HulkIronManHulki"));
}

void TestReverseStringWords()
{
    char str[100] = "This is Amit";
    reverseStringWords(str);
    printf("Reverse string is %s \n",str);
}

void TestRepeatNBy2()
{
    int arr[] = {3, 3, 3, 3, 5, 6, 4, 4};
    int size  = (sizeof(arr)/sizeof(arr[0]));

    PrintArray(arr, size);
    printf("TestRepeatNBy2 %d \n", FindNElementRepeatNBy2Times(arr, size));
}

void TestPrintSetRange()
{
    PrintSetRange(6390);
}

void TestGiveOnceOnlyNumber()
{
    int arr[] = {3, 3, 3, 1, 4, 4, 4};
    int size  = (sizeof(arr)/sizeof(arr[0]));

    printf("TestGiveOnceOnlyNumber %d \n", GiveOnceOnlyNumber(arr, size));
}

void TestResetBit()
{

}
void TestPrint32Best()
{
    //PrintBest32("test1.bin", "test1.out");
}

void TestPerformShellSort()
{
    int arr[] = {2, 3, 1, 8, -10, 16, 15, 14, 19};
    int size  = (sizeof(arr)/sizeof(arr[0]));

    PerformShellSort(arr, size);
    PrintArray(arr, size);

}

void TestUnDirectedGraph()
{
    int arr[] = {5, 5, 4, 2, 2, 1, 3, 1, 3, 0, 4, 3};

    UndirectedGraph unGraph(arr);
    unGraph.PrintEdges();

}

#endif
