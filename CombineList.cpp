struct Node
{
    int data;
    Node* pNext;
};

Node* Combine(Node* pListA, Node* pListB)
{ 
    Node* pCombinedList       = NULL;
    Node* pCombindListCurrent = NULL;
#if 0
    if (pListA != NULL)
    {
      // Point combine list to A
      pCombinedList = pListA;
      
      // iterate all the way to the end of A
      do 
      {
          pCombindListCurrent = pListA;
          pListA = pListA->pNext;
          
      }while (pListA != NULL);
          
    }
    
    if (pListB != NULL)
    {
      if (pCombinedListCurrent != NULL)
      {
          pCombinedListCurrent->Next = pListB;
      }
      else
      {
          pCombinedList = pListB;
      }
    }
#else
     Node* pListAStart  = pListA;
     Node* pListBStart  = pListB;
     Node* pCombinedListCurrentA = NULL;
     Node* pCombinedListCurrentB = NULL;
     
     
     while ((pListA != NULL) || (pListB != NULL)
     {
        pCombinedListCurrentA = pListA;
        pCombinedListCurrentB = pListB;
        
        pListA = pListA->Next;
        
        if (pListA == NULL)
        {
          break;
        }
        
        pListB = pListB->Next;
  
        if (pListB == NULL)
        {
          break;
        }
        
     }
     
     // we reached to end of list A first and A is not empty list
     if ((pListA == NULL) && (pCombinedListCurrentA != NULL))
     {
         pCombinedListCurrentA->Next = pCombinedListCurrentB;
         pCombinedList = pListAStart;
     }
     else if (pCombinedListCurrentB != NULL)
     {
         // we reached to end of list B first and B is not empty list
         pCombinedListCurrentB->Next = pCombinedListCurrentA;
         pCombinedList = pListBStart;
     }
     
#endif    
   return pCombinedList; 
}