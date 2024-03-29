#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

bool checkPaths(Node* root, int depth, int& leaves);

bool equalPaths(Node * root)
{

int leaves = -1;
return checkPaths(root, 0, leaves);
}

bool checkPaths(Node* root, int depth, int& leaves)
{
    if (root == NULL)
    {
        return true;
    }

    if((root->left == NULL) && root->right == NULL)
    {
        if (leaves == -1)
        {
            leaves = depth;
        }
        else if (leaves != depth)
        {   
        return false;
        }
    return true;

    }
    return checkPaths(root->left, depth + 1, leaves) && checkPaths(root->right, depth + 1, leaves);

}

