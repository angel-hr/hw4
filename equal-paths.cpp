#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

const int height(const Node* current) {
    if (current == NULL) {return 0;}
    if (current->left == NULL && current->right == NULL) {return 1;}  //if were at the last node
    int left = 1 + height(current->left);
    int right = 1 + height(current->right);
    if (right > left) { left = right;}
    return left;
}


bool equalPaths(Node * root)
{
    if (root == NULL) {return true;}
    if (root->left == NULL && root->right == NULL){ return true; } //if there is only one value
    else if (root->left == NULL && root->right != NULL) {
        if (root->right->left == NULL && root->right->right == NULL){ return true;}
    }
    else if (root->left != NULL && root->right == NULL) {
        if (root->left->left == NULL && root->left->right == NULL){return true;}
    }
    return height(root->left) == height(root->right);
}

