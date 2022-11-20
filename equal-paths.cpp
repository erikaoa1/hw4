#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

int nodeLengthChecker(Node* node);


bool equalPaths(Node * root)
{
    int length;

    if (root == NULL) return true; 
    
    else if (root->left == nullptr && root->right != nullptr){
        length = nodeLengthChecker(root->right);
    }


    else if (root->right == nullptr && root->left != nullptr){
        length = nodeLengthChecker(root->left);
    }

    
    else{
        length = nodeLengthChecker(root);
    }

    if(length == -1){
        return false;
    } else return true;


}

int nodeLengthChecker(Node* node){

    if (node->left == NULL && node->right == NULL){ //this child is a leaf node
        return 1;
    }
    
    if (node->left == NULL){
        return 1 + nodeLengthChecker(node->right);
    }
    else if (node->right == NULL){
        return 1 + nodeLengthChecker(node->left);
    }
    else {
        int left = 1 + nodeLengthChecker(node->left);
        int right = 1 + nodeLengthChecker(node->right);
        
        if (left == right){
        return left;
        } else return -1;
    }
}

