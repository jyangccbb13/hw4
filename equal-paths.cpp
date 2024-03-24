#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

int checkPaths(Node* root);

bool equalPaths(Node * root)
{
    // Add your code below
    if (!root)
    {
        return true;
    }
    else return checkPaths(root) != -1;


}

int checkPaths(Node* root)
{
    if (!(root->right) && !(root->left))
    {
        return 1;
    }
    else
    {
        int Ldepth = 0, Rdepth = 0;
        if (root->left)
        {
            Ldepth = checkPaths(root->left);
            if (Ldepth == -1)
            {
                return -1;
            }
        }

        if (root->right)
        {
            Rdepth = checkPaths(root->right);
            if (Rdepth == -1)
            {
                return -1;
            }
        }

        if (!(root->right) || !(root->left))
        {
            return max(Ldepth, Rdepth) + 1;
        }

        if (Ldepth == Rdepth)
        {
            return Ldepth + 1;
        }
    }
}

