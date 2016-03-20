#include<iostream>
#include<string>
#include<fstream>
#include<ctime>
//#include "TreeNode.h"
//#include "RBT.h"


using namespace std;

//=============== TREENODE CLASS ==================================
class TreeNode
{
	public:
		TreeNode *left;
		TreeNode *right;
		TreeNode *parent;
		string color;
		int id;
		int count;
		TreeNode(){}
		TreeNode(int,int);
};

TreeNode::TreeNode(int id, int count)
{
	left = right = parent = NULL;
	color = "RED";
	this->id = id;
	this->count = count;
}

//==================== RED BLACK TREE ==============================
class RBT
{
	public:
		TreeNode *root;
	
		RBT(){cout<<"RBT constructor\n";root = NULL;}
		void insert(TreeNode*);
		void del(TreeNode*);
		void fixInsert(TreeNode*);
		void LeftRotate(TreeNode*);
		void RightRotate(TreeNode*);
		void transplant(TreeNode*.TreeNode*);
};

//==================== INORDER ======================================

void inorder(TreeNode *root)
{
	if(root == NULL)
		return;

	inorder(root->left);
	cout<<root->id<<"  "<<root->color<<endl;
	inorder(root->right);
}

//==================== INSERT =======================================
void RBT::insert(TreeNode *node)
{
	TreeNode *y = NULL;
	TreeNode *x = root;

	while(x != NULL)
	{
		y = x;
		if(node->id < x->id)
			x = x->left;

		else
			x = x->right;
	}
	node->parent = y;
	
	if(y == NULL)
		root = node;

	else if(node->id < y->id)
		y->left = node;

	else
		y->right = node;

	//cout<<"After inserting "<<node->id<<endl;
	//inorder(root);
	fixInsert(node);
	//cout<<"After fixing "<<node->id<<endl;
	//inorder(root);
	//cout<<root->id<<endl;
}

//===================== LEFT ROTATE ======================================

void RBT::LeftRotate(TreeNode* x)
{

	//cout<<"inside left rotate\n"<<root->id<<endl<<x->id<<endl;
	TreeNode *y = x->right;
	x->right = y->left;
	if(y->left != NULL)
		y->left->parent = x;

	y->parent = x->parent;
	if(x->parent == NULL)
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
	if(x->right != NULL)
		x->right->parent = y;

	x->parent = y->parent;
	
	if(y->parent == NULL)
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
	TreeNode *parent = NULL;
	TreeNode *grandparent = NULL;

	while(node != root && node->color != "BLACK" && node->parent->color == "RED")
	{
		//cout<<"enterd while\n";
		parent = node->parent;
		grandparent = node->parent->parent;

		//cout<<node->id<<"  "<<parent->id<<"  "<<grandparent->id<<endl;
		if(parent == grandparent->left)
		{
			//cout<<"entered main if\n";
			TreeNode *uncle = grandparent->right;

			if(uncle != NULL && uncle->color == "RED")	
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
				string temp = parent->color;
				parent->color = grandparent->color;
				grandparent->color = temp;
				node = parent;
			}
		}

		else
		{

			//cout<<"enterd else\n";
			TreeNode *uncle = grandparent->left;

			if(uncle != NULL && uncle->color == "RED")	
			{
				//cout<<"entered if in else\n";
				grandparent->color = "RED";
				parent->color = "BLACK";
				uncle->color = "BLACK";
				node = grandparent;
				
			}

			else
			{
				//cout<<"entered else in else\n";
				if(node == parent->left)
				{
					RightRotate(parent);
					node = parent;
					parent = node->parent;
				}
				//cout<<parent->color<<","<<parent->id<<"  "<<grandparent->color<<","<<grandparent->id<<endl;
				LeftRotate(grandparent);
				//cout<<parent->color<<","<<parent->id<<"  "<<grandparent->color<<","<<grandparent->id<<endl;
				string temp = parent->color;
				parent->color = grandparent->color;
				grandparent->color = temp;
				//cout<<parent->color<<","<<parent->id<<"  "<<grandparent->color<<","<<grandparent->id<<endl;
				node = parent;
			
			}
		}
	}

	//cout<<root->id<<endl;
	root->color = "BLACK";
}

//===================== TRANSPLANT ==================================

void RBT::transplant(TreeNode* u, TreeNode* v)
{
	if(u->parent == NULL)
		root = v;
	
	else if(u == u->parent->left)
		u->parent->left = v;

	else
		u->parent->right = v;
}


//===================== DELETE ======================================

void RBT::del(TreeNode* node)
{
	
}











int main(int argc, char *argv[])
{
	//cout<<"start\n";
	//TreeNode temp;
	
	 clock_t start, end, finalend;

    	start = clock();
    	

	RBT tree;

	int numNodes;

	string input_file = argv[1];
	//cout<<input_file<<endl;

	const char* input = input_file.c_str();

	ifstream read_file;
	
	
	read_file.open(input);
	if(read_file.is_open())
	{
		//cout<<"file opened\n";
		string line = "";
		string id_str = "";
		string count_str = "";	
		getline(read_file, line);
		while(getline(read_file, line))
		{
			//cout<<line<<endl;
			
			int id,count;
			int i=0;
			while(line[i] != ' ')
			{
				id_str = id_str + line[i];
				i++;
			}
			size_t sz;
			id = stoi(id_str,&sz);
			id_str = "";
			//cout<<id<<endl;

			i++;
			while(i < line.length())
			{
				count_str = count_str + line[i];
				i++;
			}
			count = stoi(count_str, &sz);
			count_str = "";
			//cout<<count<<endl;

			TreeNode *node = new TreeNode(id,count);
			//cout<<node->id<<endl;
			tree.insert(node);
			
		}
	}

	else
	{
		cout<<"Error : File open failed\n";
	}

	
	    end = clock();
	
	
	/*inorder(tree.root);
	TreeNode *node = new TreeNode(4,1);
	tree.insert(node);
	cout<<"===========================================\n";*/
	//inorder(tree.root);
	//cout<<endl;

	cout << "Tree insert time = " << (double(end - start) / CLOCKS_PER_SEC) << " seconds" << '\n';
	cout << "Tree traversal time = " << (double(finalend - end) / CLOCKS_PER_SEC) << " seconds" << '\n';
	

	return 0;
}
