#include<stdio.h>
#include<stdlib.h>


#include <queue>

using namespace std;

class bst_tree
{
private:
    struct tree_node
    {
        struct tree_node *left_node;
        struct tree_node *right_node;
        int key;
        
        tree_node() : left_node(NULL), right_node(NULL), key(0)
        {  }
    };
    
    tree_node *m_root;
    int m_test;
    
public:
    bst_tree(): m_root(NULL) { printf("%d ", m_test); }
    
    bool insert(int key)
    {
       // printf("%d ", m_test);
        if (NULL == m_root) {
            m_root = new tree_node;
            if (NULL != m_root) {
                m_root->key = key;
                return true;
            }
            else {
                return false;
            }
        }
       
        tree_node *current_node = m_root;
        for (;;) {
            if (key <= current_node->key) {
                if (NULL == current_node->left_node) {
                    current_node->left_node = new tree_node;
                    if (NULL == current_node->left_node) {
                        return false;
                    }
                    else {
                        current_node->left_node->key = key;
                        return true;
                    }
                }
                else {
                    current_node = current_node->left_node;
                }
            }
            else {
                if (NULL == current_node->right_node) {
                    current_node->right_node = new tree_node;
                    if (NULL == current_node->right_node) {
                        return false;
                    }
                    else {
                        current_node->right_node->key = key;
                        return true;
                    }
                }
                else {
                    current_node = current_node->right_node;
                }
            }
        }
    }
    
    void print_level_order()
    {
    
        queue<tree_node *> q;        
        q.push(m_root);
        
        
        while(!q.empty()){
        
            tree_node * curr = q.front();
            
            printf("%d ", curr->key);
            
            q.pop();
            
            if(curr->left_node!=NULL) q.push(curr->left_node);
            if(curr->right_node!=NULL) q.push(curr->right_node);  
         }    
            
    }
};

int main()
{
    
    bst_tree t;
  
    t.insert(5);
    t.insert(3);
    t.insert(7);
    t.insert(2);
    t.insert(4);
    t.insert(6);
    t.insert(8);
    t.print_level_order();   
    return 0;
}