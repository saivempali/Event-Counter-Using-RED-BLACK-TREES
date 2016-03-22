#include<string>
#include "TreeNode.h"

class RBT
{
	public:
		TreeNode *root;
		TreeNode *nil;
		TreeNode *rightmost;
	
		RBT()
		{
			nil = new TreeNode;
			nil->left = nil->right = nil->parent = nil;
			nil->id = -1;
			nil->count = -1;
			nil->color = "BLACK";
			root = nil;
			rightmost = root;
		}
		void buildTree(int,int);
		void insert(TreeNode*);
		void del(int);
		void delHelper(TreeNode*);
		void fixInsert(TreeNode*);
		void fixDelete(TreeNode*);
		void LeftRotate(TreeNode*);
		void RightRotate(TreeNode*);
		void transplant(TreeNode*,TreeNode*);
		void inorder(TreeNode*);
		
		
		void Increase(int,int);
		void Reduce(int,int);
		void Count(int);
		void InRange(int,int);
		void Next(int);
		void Previous(int);
		void InRangeHelper(TreeNode*,int,int);

		TreeNode* findNode(int);
		TreeNode* inorderSuccessor(TreeNode*);
		TreeNode* inorderPredecessor(TreeNode*);
};

// TREE NODE CONSTRUCTOR:
TreeNode::TreeNode(int id, int count)
{
	RBT rbt;
	left = right = parent = rbt.nil;
	color = "RED";
	this->id = id;
	this->count = count;
}

//==================== FIND NODE ====================================

TreeNode* RBT::findNode(int val)
{
	TreeNode *temp = root;
	while(temp->count != -1)	
	{
		if(temp->id == val)
		{
			return temp;
		}

		else if(val < temp->id)
			temp = temp->left;

		else
			temp = temp->right;
	}
	
	return nil;
}

//==================== INSERT =======================================
void RBT::insert(TreeNode *node)
{

	
	TreeNode *y = nil;
	TreeNode *x = root;

	while(x->count != -1)
	{
		y = x;
		if(node->id < x->id)
			x = x->left;

		else
			x = x->right;
	}
	node->parent = y;
	
	if(y == nil)
		root = node;

	else if(node->id < y->id)
		y->left = node;

	else
		y->right = node;

	fixInsert(node);
}

//===================== LEFT ROTATE ======================================

void RBT::LeftRotate(TreeNode* x)
{

	//cout<<"inside left rotate\n"<<root->id<<endl<<x->id<<endl;
	TreeNode *y = x->right;
	x->right = y->left;
	if(y->left != nil)
		y->left->parent = x;

	y->parent = x->parent;
	if(x->parent == nil)
		root = y;

	else if(x == x->parent->left)	
		x->parent->left = y;
	
	else
		x->parent->right = y;

	y->left = x;
	x->parent = y;
}

//====================== RIGHT ROTATE =======================================

void RBT::RightRotate(TreeNode* y)
{
	TreeNode* x = y->left;
	y->left = x->right;
	if(x->right != nil)
		x->right->parent = y;

	x->parent = y->parent;
	
	if(y->parent == nil)
		root = x;

	else if(y == y->parent->left)
		y->parent->left = x;

	else
		y->parent->right = x;

	x->right = y;
	y->parent = x;
}

//===================== FIX INSERT =======================================

void RBT::fixInsert(TreeNode *node)
{
	TreeNode *parent = nil;
	TreeNode *grandparent = nil;

	while(node->parent->color == "RED")
	{
		parent = node->parent;
		grandparent = node->parent->parent;

		if(parent == grandparent->left)
		{
			TreeNode *uncle = grandparent->right;

			if(uncle != nil && uncle->color == "RED")	
			{
				grandparent->color = "RED";
				parent->color = "BLACK";
				uncle->color = "BLACK";
				node = grandparent;
			}
			
			else
			{
				if(parent == parent->right)
				{
					LeftRotate(parent);
					node = parent;
					parent = node->parent;
				}

				RightRotate(grandparent);
				std::string temp = parent->color;
				parent->color = grandparent->color;
				grandparent->color = temp;
				node = parent;
			}
		}

		else
		{
			TreeNode *uncle = grandparent->left;

			if(uncle != nil && uncle->color == "RED")	
			{
				grandparent->color = "RED";
				parent->color = "BLACK";
				uncle->color = "BLACK";
				node = grandparent;	
			}

			else
			{
				if(node == parent->left)
				{
					RightRotate(parent);
					node = parent;
					parent = node->parent;
				}
				
				LeftRotate(grandparent);
				std::string temp = parent->color;
				parent->color = grandparent->color;
				grandparent->color = temp;
				node = parent;
			}
		}
	}
	root->color = "BLACK";
}

//===================== TRANSPLANT ==================================

void RBT::transplant(TreeNode* u, TreeNode* v)
{
	if(u->parent->count == -1)
	{
		root = v;
	}
	
	else if(u == u->parent->left)
	{
		u->parent->left = v;
	}

	else
	{
		u->parent->right = v;
	}
	v->parent = u->parent;
}

//===================== INORDER SUCCESSOR ===========================

TreeNode* RBT::inorderSuccessor(TreeNode* z)
{
	if(z->right->count == -1)	
	{
		TreeNode *p = z->parent;
		while(p->count != -1 && z == p->right)
		{
			z = p;
			p = p->parent;
		}
		return p;
	}

	TreeNode *temp = z->right;
	while(temp->left->count != -1)
	{
		temp = temp->left;
	}
	return temp;
}

TreeNode* RBT::inorderPredecessor(TreeNode* z)
{
	if(z->left->count == -1)
	{
		TreeNode *p = z->parent;
		while(p->count != -1 && z == p->left)
		{
			z = p;
			p = p->parent;
		}
		return p;
	}

	TreeNode *temp = z->left;
	while(temp->right->count != -1)
		temp = temp->right;

	return temp;
}

//===================== DELETE ======================================

void RBT::del(int val)
{
	TreeNode* temp = root;

	while(temp->id != val)		
	{
		if(val < temp->id)
			temp  = temp->left;

		else
			temp = temp->right;
	}

	delHelper(temp);
}

void RBT::delHelper(TreeNode* z)
{
	TreeNode* y = z;
	TreeNode* x;
	std::string y_original_color = y->color;
	if(z->left->count == -1)
	{
		x = z->right;
		transplant(z,z->right);
	}

	else if(z->right == nil)
	{
		x = z->left;
		transplant(z,z->left);
	}

	// CASE : More than 1 child
	else
	{
		y = inorderSuccessor(z);
		y_original_color = y->color;
		x = y->right;
		if(y->parent == z)
		{
			x->parent = y;
		}

		else
		{
			transplant(y,y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		
		transplant(z,y);
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;
	}

	if(y_original_color == "BLACK")
	{
		fixDelete(x);
	}
}

//========================== FIX DELETE ============================

void RBT::fixDelete(TreeNode* x)
{
	TreeNode* sibling_x;
	while(x != root && x->color == "BLACK")
	{
		if(x == x->parent->left)
		{
			sibling_x = x->parent->right;
			if(sibling_x->color == "RED")
			{
				sibling_x->color = "BLACK";
				x->parent->color = "RED";
				LeftRotate(x->parent);
				sibling_x = x->parent->right;
			}

			if(sibling_x->left->color == "BLACK" && sibling_x->right->color == "BLACK")
			{
				sibling_x->color = "RED";
				x = x->parent;
			}

			else 
			{
				if(sibling_x->right->color == "BLACK")
				{
					sibling_x->left->color == "BLACK";
					sibling_x->color == "RED";
					RightRotate(sibling_x);
					sibling_x = x->parent->right;
				}

				sibling_x->color = x->parent->color;
				x->parent->color = "BLACK";
				sibling_x->right->color = "BLACK";
				LeftRotate(x->parent);
				x = root;
			}
		}

		else
		{
			sibling_x = x->parent->left;
			if(sibling_x->color == "RED")			
			{
				sibling_x->color = "BLACK";
				x->parent->color = "RED";
				RightRotate(x->parent);
				sibling_x = x->parent->left;
			}

			if(sibling_x->right->color == "BLACK" && sibling_x->left->color == "BLACK")
			{
				sibling_x->color = "RED";
				x = x->parent;
			}

			else
			{
				if(sibling_x->left->color == "BLACK")
				{
					sibling_x->right->color = "BLACK";
					sibling_x->color = "RED";
					LeftRotate(sibling_x);
					sibling_x = x->parent->left;
				}

				sibling_x->color = x->parent->color;
				x->parent->color = "BLACK";
				sibling_x->left->color = "BLACK";
				RightRotate(x->parent);
				x = root;
			}
		}
	}
	x->color = "BLACK";
}

