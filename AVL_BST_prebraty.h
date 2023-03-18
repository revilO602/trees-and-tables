// AVL tree implementation in C taken from GeeksForGeeks
//insert and other functions: https://www.geeksforgeeks.org/avl-tree-set-1-insertion/
//search function: https://www.geeksforgeeks.org/binary-search-tree-set-1-search-and-insertion/
//I added free function for testing purposes
//I renamed som structs and functions for consistency
#include<stdio.h> 
#include<stdlib.h> 

// An AVL tree node 
 typedef struct AVLNode
{
	int key;
	struct AVLNode* left;
	struct AVLNode* right;
	int height;
}AVLNODE;

 // A utility function to get maximum of two integers
int maxVal(int a, int b);

int height( AVLNODE* N)
{
	if (N == NULL)
		return 0;
	return N->height;
}
// A utility function to get maximum of two integers
int maxVal(int a, int b)
{
	return (a > b) ? a : b;
}

 AVLNODE* newNode(int key)
{
	 AVLNODE* node = ( AVLNODE*)
		malloc(sizeof( AVLNODE));
	node->key = key;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;
	return(node);
}

 AVLNODE* rightRotate( AVLNODE* y)
{
	 AVLNODE* x = y->left;
	 AVLNODE* T2 = x->right;

	x->right = y;
	y->left = T2;

	y->height = maxVal(height(y->left), height(y->right)) + 1;
	x->height = maxVal(height(x->left), height(x->right)) + 1;

	return x;
}

 AVLNODE* leftRotate( AVLNODE* x)
{
	 AVLNODE* y = x->right;
	 AVLNODE* T2 = y->left;

	y->left = x;
	x->right = T2;

	x->height = maxVal(height(x->left), height(x->right)) + 1;
	y->height = maxVal(height(y->left), height(y->right)) + 1;

	return y;
}

int getBalance( AVLNODE* N)
{
	if (N == NULL)
		return 0;
	return height(N->left) - height(N->right);
}

// Recursive function to insert a key in the subtree rooted 
// with node and returns the new root of the subtree. 
AVLNODE* insertAVL(AVLNODE* node, int key)
{
	/* 1.  Perform the normal BST insertion */
	if (node == NULL)
		return(newNode(key));

	if (key < node->key)
		node->left = insertAVL(node->left, key);
	else if (key > node->key)
		node->right = insertAVL(node->right, key);
	else // Equal keys are not allowed in BST 
		return node;

	/* 2. Update height of this ancestor node */
	node->height = 1 + max(height(node->left),
		height(node->right));

	/* 3. Get the balance factor of this ancestor
		  node to check whether this node became
		  unbalanced */
	int balance = getBalance(node);

	// If this node becomes unbalanced, then 
	// there are 4 cases 

	// Left Left Case 
	if (balance > 1 && key < node->left->key)
		return rightRotate(node);

	// Right Right Case 
	if (balance < -1 && key > node->right->key)
		return leftRotate(node);

	// Left Right Case 
	if (balance > 1 && key > node->left->key)
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	// Right Left Case 
	if (balance < -1 && key < node->right->key)
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	/* return the (unchanged) node pointer */
	return node;
}

// C function to search a given key in a given BST 
 int searchAVLTree( AVLNODE* root, int key)
 {
	 // Base Cases: root is null or key is present at root 
	 if (root == NULL || root->key == key)
		 return 1;

	 // Key is greater than root's key 
	 if (root->key < key)
		 return searchAVLTree(root->right, key);

	 // Key is smaller than root's key 
	 return searchAVLTree(root->left, key);
 }

 void freeAVLTree(AVLNODE* root) {
	 if (root != NULL) {
		 freeAVLTree(root->left);
		 freeAVLTree(root->right);
		 free(root);
	 }
 }