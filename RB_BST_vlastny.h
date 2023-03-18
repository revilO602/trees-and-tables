// Oliver Leontiev
// DSA, Zadanie 2, 2020
// my own red-black BST
//#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct RBNode {
	int key;
	struct RBNode* parent;
	struct RBNode* left_child;
	struct RBNode* right_child;
	bool red;
}RBNODE;

RBNODE* addNodeRB(RBNODE* parent, int new_key) {
	RBNODE* new_node;
	if ((new_node = (RBNODE*)malloc(sizeof(RBNODE))) == NULL)return NULL;
	new_node->left_child = NULL;
	new_node->right_child = NULL;
	new_node->parent = parent;
	new_node->key = new_key;
	new_node->red = true;
	return new_node;
}
//returns what side of its parent is node (left = 0 / right = 1)
int whichChildRB(RBNODE* node) {
	if (node == node->parent->left_child)
		return 0;
	return 1;
}

void rotateLeftRB(RBNODE** root, RBNODE* node) { // 
	RBNODE* p = node->parent;
	RBNODE* rc = node->right_child;
	int node_side;
	if (node != *root)node_side = whichChildRB(node);
	node->right_child = rc->left_child;
	if (rc->left_child != NULL) rc->left_child->parent = node;
	rc->left_child = node;
	node->parent = rc;
	rc->parent = p;
	if (node == *root)
		* root = rc;
	else if (node_side == 0)
		p->left_child = rc;
	else p->right_child = rc;
}

void rotateRightRB(RBNODE** root, RBNODE* node) {
	RBNODE* p = node->parent;
	RBNODE* lc = node->left_child;
	int node_side;
	if (node != *root)node_side = whichChildRB(node);
	node->left_child = lc->right_child;
	if (lc->right_child != NULL) lc->right_child->parent = node;
	lc->right_child = node;
	node->parent = lc;
	lc->parent = p;
	if (node == *root)
		* root = lc;
	else if (node_side == 0)
		p->left_child = lc;
	else p->right_child = lc;
}
//regular BST insert
RBNODE* preInsertRB(RBNODE** root, int new_key) {
	RBNODE* x = *root;
	RBNODE* p = NULL;
	while (x != NULL) {
		p = x;
		if (new_key < x->key)
			x = x->left_child;
		else if (new_key > x->key)
			x = x->right_child;
		else return NULL;
	}
	if (p == NULL)
		x = *root = addNodeRB(NULL, new_key);
	else if (new_key < p->key)
		x = p->left_child = addNodeRB(p, new_key);
	else x = p->right_child = addNodeRB(p, new_key);
	return x;
}
// return null if no sibling
RBNODE* getSiblingRB(RBNODE* node) { 
	RBNODE* p = node->parent;
	if (p->left_child == node)
		return p->right_child;
	else return p->left_child;
}
//recolors up the tree until possible within the rules of RB BST
RBNODE* recolorRB(RBNODE* node) {
	RBNODE* p = node->parent;
	RBNODE* grand_parent;
	RBNODE* p_sib = getSiblingRB(p);
	while (p != NULL) {
		grand_parent = p->parent;
		p->red = false;
		p_sib->red = false;
		grand_parent->red = true;
		node = grand_parent;
		p = node->parent;
		if (p != NULL) {
			if (!p->red)
				return NULL;
			p_sib = getSiblingRB(p);
			if (p_sib == NULL || !p_sib->red)
				return node;
		}
	}
	node->red = false;
	return NULL;
}
//performs single or double rotation, within RB BST rules
void reshapeRB(RBNODE** root, RBNODE* node) {
	RBNODE* p = node->parent;
	RBNODE* grand_parent = p->parent;
	RBNODE* tmp;
	int node_side = whichChildRB(node);
	int p_side = whichChildRB(p);

	if (node_side != p_side) { // first rotation
		if (p_side == 0)
			rotateLeftRB(root, p);
		else rotateRightRB(root, p);
		tmp = node;
		node = p;
		p = tmp;
	}
	node_side = whichChildRB(node);
	p->red = false;
	grand_parent->red = true;
	if (node_side == 0) // second rotation
		rotateRightRB(root, grand_parent);
	else rotateLeftRB(root, grand_parent);
}

void insertRBTree(RBNODE** root, int new_key) {
	RBNODE* new_node;
	RBNODE* p;
	RBNODE* p_sib;
	new_node = preInsertRB(root, new_key);
	if (new_node == NULL)return;
	p = new_node->parent;
	if (*root == new_node) {
		new_node->red = false;
		return;
	}
	p = new_node->parent;
	if (p->red) {
		p_sib = getSiblingRB(p);
		if (p_sib != NULL && p_sib->red)
			new_node = recolorRB(new_node);
		if (new_node != NULL) {
			reshapeRB(root, new_node);
		}
	}
}

int searchRBTree(RBNODE* root, int key) {
	RBNODE* akt = root;
	while (akt != NULL) {
		if (akt->key == key)
			return 1;
		if (key < akt->key)
			akt = akt->left_child;
		else if (key > akt->key)
			akt = akt->right_child;
	}
	return 0;
}

void freeRBTree(RBNODE* root) {
	if (root != NULL) {
		freeRBTree(root->left_child);
		freeRBTree(root->right_child);
		free(root);
	}
}
