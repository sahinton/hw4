#include "equal-paths.h"
#include <iostream>
using namespace std;


// You may add any prototypes of helper functions here
int rightHeight(Node* root); 
int leftHeight(Node* root);

int leftHeight(Node* root){ //return left subtree height
	if (root == nullptr){
		return 0;
	}
	return 1 + leftHeight(root->left);

}
int rightHeight(Node* root){ //return right subtree height
	if (root == nullptr){
		return 0;
	}
	return 1 + rightHeight(root->right);

}
bool equalPaths(Node * root)
{
    // Add your code below
		cout << leftHeight(root) << " " << rightHeight(root) << endl;
		if (leftHeight(root) == rightHeight(root)){
			return true;
		}
		else if(leftHeight(root) == 2 && rightHeight(root) == 1){
			return true;
		}
		else if(leftHeight(root) == 1 && rightHeight(root) == 2){
			return true;
		}
		else{
			return false;
		}
		
}

