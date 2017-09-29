#include <iostream>
#include <assert.h>

struct Node {
    int value;
    Node *left;
    Node *right;
};
struct BinaryTree {
public:
    BinaryTree():root(nullptr)
    {}

    void push_data(int value)
    {
        Node *pnode = new Node({value, nullptr, nullptr});
        push_data_aux(root, pnode);
    }
    void preorder_print()
    {
        preorder_print_aux(root);
        std::cout << std::endl;
    }
private:
    Node *root;
    void preorder_print_aux(Node *root)
    {
         if(!root)   
         {
            return;
         }
         preorder_print_aux(root->left);
         std::cout << root->value << " ";
         preorder_print_aux(root->right);
    }
    void push_data_aux(Node *& root, Node *pnode)
    {
        assert(pnode != nullptr);

        if(!root)
        {
            root = pnode;
            return;
        }
        if(pnode->value < root->value)
        {
            if( root->left == nullptr)
            {
                root->left = pnode;
            }
            else
            {
                push_data_aux(root->left, pnode);
            }
        }
        else
        {
            if ( root->right == nullptr)
            {
                root->right = pnode;
            }
            else
            {
                push_data_aux(root->right, pnode);
            }
        }
    }
};
int main(int argc, char** argv)
{
    int array[] = {1, 3, 14, 32, 22, 31, 98, 5, 54, 23, 2}; 
    BinaryTree btree;
    for(auto value : array)
    {
        btree.push_data(value);
    }
    btree.preorder_print();
    return 0;
}
