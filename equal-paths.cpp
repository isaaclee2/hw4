#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int getSubtreeHeight(Node* node){
    if(node == nullptr){
        return 0;
    }

    int height_left = getSubtreeHeight(node->left);
    int height_right = getSubtreeHeight(node->right);

    int max = -1000;
    if(height_left > height_right){
        max = height_left;
    }
    else{
        max = height_right;
    }
    return max + 1;
}


bool equalPaths(Node * root)
{
    // Add your code below

    // empty tree
    if(root == nullptr){
        return true;
    }
    // leaf 
    if(root->left == nullptr && root->right == nullptr){
        return true;
    }
    // only one child
    if(root->left == nullptr){
        return equalPaths(root->right);
    }
    if(root->right == nullptr){
        return equalPaths(root->left);
    }
    // both children
    if(equalPaths(root->left) == false || equalPaths(root->right) == false){
        return false;
    }



    int height_left = getSubtreeHeight(root->left);
    int height_right = getSubtreeHeight(root->right);

    if(height_left == height_right){
        return true;
    }
    else{
        return false;
    }
}