#include <iostream>
#include <fstream>
#include <cstring>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct node
{
    int key;
    node* left;
    node* right;
};
//helper funciton to find min value node in the subtree rooted at 'current'
//used in a delete where the node has two children and we need to 
//restructure the subtree or entire tree
node* getMinimumKey(node* current)
{
    while(current->left != NULL)
    {
        current = current->left;
    }
    return current;
}
// A utility function to do
// inorder traversal of BST
void inorder(node* root)
{
    if (root != NULL) {
        inorder(root->left);
        std::cout << root->key <<",";
        inorder(root->right);
    }
    //std::cout << std::endl; //for format purposes so it looks nice and organized
}
//iterative function to search in the subtree rooted at 'current' and set its parent.
//current and parent are passed by reference
void searchKey(node*& current, int key, node*& parent)
{
    while(current != NULL && current->key != key)
    {
        parent = current; // update the parent to the current node
        if (key < current->key)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
        
    }
}
bool searchNoRecursion(node* leaf, int key){
    bool found=false;
    
    //use a while loop and not recursion. Leaf is being changed
    //each and every time to point to the left or right, going down
    //the tree.
    while (leaf && !found){ 
        if (key==leaf->key){
            found=true;
        }else if (key < leaf->key)
            leaf = leaf->left; //replaces recursion
        else
            leaf = leaf->right;
    }
    return found;
}

//insert operations
//recursively insert, tail rescursive, no return ops, ideal, nothing to do on return of recursion
//make connection on insert
void insert(node *& leaf, int item){
    if (!leaf){// Insertion place found.
        leaf = new node;
        leaf->right = NULL;
        leaf->left = NULL;
        leaf->key = item;
    }
    else if (item < leaf->key)
        insert(leaf->left, item) ;    //Insert in left subtree.
    else if (item > leaf->key)
        insert(leaf->right, item) ;   //Insert in right subtree.	
}

//delete operations
void deleteNode(node*& root, int number)//char* number)
{
    //search for the node with the value
    //BinaryTree *leaf; // just a pointer to a node to use during taversal to try to find our 'key'

    node* parent = NULL;//pointer to store the parent of the current node
    node* current = root;//starting with the root node
    bool booleanValForFoundInSearch = searchNoRecursion(current,number);
    std::cout << "Found: " << booleanValForFoundInSearch << std::endl;
    if(booleanValForFoundInSearch == true)
    {
        searchKey(current, number, parent); // search key in the BST And set its parent pointer

        //no children-- it is a leaf node
            if(current->left == NULL && current->right == NULL)
            {
                if(current != root)//if the node to be deleted is NOT a root node, the set its parent left or right child to NULL
                {
                    if(parent->left == current)
                    {
                        parent->left = NULL;
                    }
                    else
                    {
                        parent->right = NULL;
                    }   
                }
                else // if the tree has only a root node, set it to null
                {
                    root = NULL;
                }
                delete(current); //deallocate memory
            }
        //two children
            else if(current->left && current->right)
            {
                node* successor = getMinimumKey(current->right);//find its inorder successor node
                int successorValue = successor->key; // store successor value
                //recursively delete the successor.
                //successor will have at most one child (the right child)
                char buffer[40];
                deleteNode(root, (char)successor->key); //had to typecast it to char*
                //itoa(successor->key,buffer,10));
                current->key = successorValue; // copy value of the successor to the current node
            }

        //one child
            else
            {
                node* child;
                //choosing a child
                if(current->left)
                {
                    child = current->left;
                }
                else
                {
                    child = current->right;
                }
                //if the node to be deleted is NOT a root node,
                //set its parent to its child
                if(current != root)
                {
                    if(current == parent->left)
                    {
                        parent->left = child;
                    }
                    else
                    {
                        parent->right = child;
                    }
                }
                //if the node to be deleted IS ROOT NODE, set the root to child
                else
                {
                    root = child;
                }
                delete(current); //deallocate the memory
            }
        
    }

}
//read from file
//line by line insert into tree
//delete when read delete
//print out data at the end
int main()
{
    FILE *fp;
    char line[255]; // line of chars
    int number[20];
    char deleteWord[] = "delete";
    const char fileName[9] = "data.txt";
    fp = fopen(fileName, "r");
    node* root;
    while(fscanf(fp, "%s", line) != EOF)
    {
        std::cout << "line is now: " << line << std::endl;
        if(strcmp(line, deleteWord) == 0) //read "delete"
        {
            fscanf(fp, "%s", line); // read one more to get value of node we want to delete
            std::cout << "line is now: " << line << std::endl;
            deleteNode(root, atoi(line));
        }
        else
        {
            
            insert(root, atoi(line));//typecasted line to an integer because insert uses comparisons
        }
    }

    //std::cout << "Opened " << fileName << std::endl;
    fclose(fp); //close the file
    //std::cout << "here\n";
    inorder(root); // print tree LVR
    std::cout << std::endl;
    return 0;
}