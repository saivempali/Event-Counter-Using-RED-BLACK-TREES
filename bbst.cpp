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



//==================== RED BLACK TREE ==============================
class RBT
{
	public:
		TreeNode *root;
		TreeNode* nil;
	
		RBT()
		{
			//cout<<"RBT constructor\n";
			nil = new TreeNode;
			nil->left = nil->right = nil->parent = nil;
			nil->id = -1;
			nil->count = -1;
			nil->color = "BLACK";
			root = nil;
		}
		void insert(TreeNode*);
		void del(int);
		void delHelper(TreeNode*);
		void fixInsert(TreeNode*);
		void fixDelete(TreeNode*);
		void LeftRotate(TreeNode*);
		void RightRotate(TreeNode*);
		void transplant(TreeNode*,TreeNode*);
		void inorder(TreeNode*);
		TreeNode* findNode(int);
		TreeNode* inorderSuccessor(TreeNode*);
		TreeNode* inorderPredecessor(TreeNode*);
		void Increase(int,int);
		void Reduce(int,int);
		void Count(int);
		void InRange(int,int);
		void Next(int);
		void Previous(int);
		void InRangeHelper(TreeNode*,int,int);
};

TreeNode::TreeNode(int id, int count)
{
	RBT rbt;
	left = right = parent = rbt.nil;
	color = "RED";
	this->id = id;
	this->count = count;
}

//==================== INORDER ======================================

void RBT::inorder(TreeNode *root)
{

	//cout<<"inside inorder\n";
	if(root->count == -1)
		return;

	inorder(root->left);
	cout<<root->id<<"  "<<root->count<<"  "<<root->color<<endl;
	inorder(root->right);
}

//==================== FIND NODE ====================================

TreeNode* RBT::findNode(int val)
{
	TreeNode *temp = root;
	while(temp->count != -1)	
	{
		if(temp->id == val)
		{
			//cout<<"val found\n";
			return temp;
		}

		else if(val < temp->id)
			temp = temp->left;

		else
			temp = temp->right;
	}
	//cout<<"val not found\n";
	return nil;
}

//==================== INSERT =======================================
void RBT::insert(TreeNode *node)
{

	//cout<<"inside insert\n";
	TreeNode *y = nil;
	TreeNode *x = root;

	while(x->count != -1)
	{
		//cout<<"while in insert\n";
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

	//cout<<"After inserting "<<node->id<<endl;
	//inorder(root);
	//cout<<root->id<<"  "<<root->color<<endl;
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

	//cout<<"inside fix insert\n"<<node->id<<endl;
	TreeNode *parent = nil;
	TreeNode *grandparent = nil;
	
	//cout<<"before while\n"<<node->parent->color<<endl;

	while(node->parent->color == "RED")
	{
		//cout<<"enterd while\n";
		parent = node->parent;
		grandparent = node->parent->parent;

		//cout<<node->id<<"  "<<parent->id<<"  "<<grandparent->id<<endl;
		if(parent == grandparent->left)
		{
			//cout<<"entered main if\n";
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

			if(uncle != nil && uncle->color == "RED")	
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
	//cout<<root->color<<endl;
}

//===================== TRANSPLANT ==================================

void RBT::transplant(TreeNode* u, TreeNode* v)
{

	//cout<<u->id<<"  "<<v->id<<endl;
	//cout<<"Inside transplant\n";
	if(u->parent->count == -1)
	{
		//cout<<"modifying root inside transplant\n";
		root = v;
	}
	
	else if(u == u->parent->left)
	{
		//cout<<"CASE : left inside transplant\n"<<u->parent->id<<endl;
		u->parent->left = v;
	}

	else
	{
		//cout<<"CASE : right inside transplant\n";
		u->parent->right = v;
	}

	
	v->parent = u->parent;
	//cout<<v->id<<"  "<<u->parent->id<<endl;
	
}

//===================== INORDER SUCCESSOR ===========================

TreeNode* RBT::inorderSuccessor(TreeNode* z)
{
	//cout<<"Inside inorderSuccessor\n"<<z->id<<"  "<<z->right->id<<endl;
	
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
	//cout<<"temp = "<<temp->id<<endl;
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

	//cout<<"Inside del\n";
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

	//cout<<"Inside delHelper\n"<<z->left->id<<"\n";
	TreeNode* y = z;
	TreeNode* x;
	string y_original_color = y->color;

	// Checking for the 0 or 1 child case
	if(z->left->count == -1)
	{
		//cout<<"Checking for the 0 or 1 child case in IF\n";
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
		//cout<<"CASE : More than 1 child\n";
		y = inorderSuccessor(z);
		//cout<<"successor = "<<y->id<<endl;
		y_original_color = y->color;
		x = y->right;
		if(y->parent == z)
		{
			//cout<<"entered if in else\n";
			//if(x != NULL)
			x->parent = y;
			//else
			//	cout<<"x is null\n";
		}

		else
		{
			//cout<<"calling trasnplant => y,y->right = "<<y->id<<","<<y->right->id<<endl;
			transplant(y,y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		//cout<<"calling transplant in more than 1 child after else\n";
		transplant(z,y);
		//cout<<"after transplant\n";
		y->left = z->left;
		//cout<<y->left->id<<endl;
		y->left->parent = y;
		//cout<<y->left->parent->id<<endl;
		y->color = z->color;
	}

	//cout<<"I'm here\n"<<y_original_color<<endl;
	if(y_original_color == "BLACK")
	{
		//cout<<"calling fix delete\n";
		fixDelete(x);
	}
	//cout<<"end of del\n";
	
}

//========================== FIX DELETE ============================

void RBT::fixDelete(TreeNode* x)
{

	//cout<<"entered fixDelete\n"<<x->id<<endl;
	TreeNode* sibling_x;
	while(x != root && x->color == "BLACK")
	{

		//cout<<"inside while in fix delete\n";
		if(x == x->parent->left)
		{
			//cout<<"x is to the left\n";
			sibling_x = x->parent->right;
			if(sibling_x->color == "RED")
			{
				//cout<<"sibling is red\n";
				sibling_x->color = "BLACK";
				x->parent->color = "RED";
				LeftRotate(x->parent);
				sibling_x = x->parent->right;
			}

			if(sibling_x->left->color == "BLACK" && sibling_x->right->color == "BLACK")
			{
				//cout<<"INSIDE IF\n";
				sibling_x->color = "RED";
				x = x->parent;
			}

			else 
			{

				//cout<<"INSIDE ELSE\n";
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
	
//	if(x != NULL)
	x->color = "BLACK";
}

//======================== INCREASE ===============================

void RBT::Increase(int theID, int m)
{
	TreeNode *event = findNode(theID);

	if(event->count == -1)
	{
		TreeNode *newEvent = new TreeNode(theID,m);
		insert(newEvent);
		cout<<m<<endl;
	}

	else
	{
		event->count += m;
		cout<<event->count<<endl;
	}
	
}

//================= REDUCE =========================

void RBT::Reduce(int theID, int m)
{
	TreeNode *event = findNode(theID);
	if(event->count == -1)
	{
		cout<<"0\n";
	}

	else
	{
		event->count -= m;
		if(event->count <= 0)
		{
			del(theID);
			cout<<"0\n";
		}

		else
			cout<<event->count<<endl;
	}	
}

//================== COUNT ===============================

void RBT::Count(int theID)
{
	TreeNode* event = findNode(theID);

	if(event->count == -1)
		cout<<"0\n";

	else
		cout<<event->count<<endl;
}

//=================== NEXT ==============================

void RBT::Next(int theID)
{
	TreeNode *currentEvent = findNode(theID);
	if(currentEvent->count == -1)
	{
		TreeNode *newEvent = new TreeNode(theID,1);
		insert(newEvent);	
		TreeNode *nextEvent = inorderSuccessor(newEvent);
		if(nextEvent->count == -1)
			cout<<"0 0\n";

		else
			cout<<nextEvent->id<<" "<<nextEvent->count<<endl;

		del(theID);
	}

	else
	{
		TreeNode *nextEvent = inorderSuccessor(currentEvent);
		if(nextEvent->count == -1)
			cout<<"0 0\n";

		else
			cout<<nextEvent->id<<" "<<nextEvent->count<<endl;
	}
}

//================== PREVIOUS =============================

void RBT::Previous(int theID)
{
	TreeNode *curEvent = findNode(theID);
	if(curEvent->count == -1)
	{
		//cout<<"not there\n";
		TreeNode *newEvent = new TreeNode(theID,1);
		insert(newEvent);
		//inorder(root);
		TreeNode *prevEvent = inorderPredecessor(newEvent);
		//cout<<prevEvent->id<<endl;
		if(prevEvent->count == -1)
			cout<<"0 0\n";

		else
			cout<<prevEvent->id<<" "<<prevEvent->count<<endl;
		
		del(theID);
	}

	else
	{

		TreeNode *prevEvent = inorderPredecessor(curEvent);
		if(prevEvent->count == -1)
			cout<<"0 0\n";

		else
			cout<<prevEvent->id<<" "<<prevEvent->count<<endl;
		
	
		/*if(curEvent->left->count == -1)
		{
			if(curEvent == curEvent->parent->right)
				cout<<curEvent->parent->id<<" "<<curEvent->parent->count<<endl;

			else
			{
				if(curEvent->id < root->id)
					cout<<"0 0\n";

				else if(curEvent->id == root->id)
					cout<<"0 0\n";

				else
				{
					if(curEvent->parent == curEvent->parent->parent->left)
						cout<<root->id<<" "<<root->count<<endl;

					else if(curEvent->parent == curEvent->parent->parent->right)
						cout<<curEvent->parent->parent->id<<" "<<curEvent->parent->parent->count<<endl;
				}
			}
		}

		else
		{
			TreeNode *t = curEvent->left;
			while(t->right->count != -1)
				t = t->right;

			cout<<t->id<<" "<<t->count<<endl;
		}*/

	}
}

int total = 0;

void RBT::InRangeHelper(TreeNode* root, int id1, int id2)
{

	//cout<<total<<endl;
	if(root->count == -1)
		return;

	if(id1 < root->id)
		InRangeHelper(root->left,id1,id2);

	if(id1 <= root->id && id2 >= root->id)
	{
		//cout<<total<<"  "<<root->count<<endl;
		total += root->count;
	}

	if(id2 > root->id)
		InRangeHelper(root->right,id1,id2);
	
}

//======================= IN RANGE =============================

void RBT::InRange(int ID1, int ID2)
{
	//cout<<ID1<<" "<<ID2<<endl;
	if(ID1 == ID2)
	{
		TreeNode *event = findNode(ID1);
		cout<<event->count<<endl;
	}

	else if(ID1 < ID2)
	{
		total = 0;
		InRangeHelper(root,ID1,ID2);
		cout<<total<<endl;
	}

	
}


int main(int argc, char *argv[])
{
	clock_t start, end, finalend;
    	start = clock();
	RBT tree;
	int numNodes;
	string input_file = argv[1];
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
		size_t sz;
		numNodes = stoi(line,&sz);
		//cout<<numNodes<<endl;
		for(int loopIndex=0; loopIndex < numNodes; loopIndex++)
		{
			getline(read_file, line);
			//cout<<line<<endl;	
			int id,count;
			int i=0;
			while(line[i] != ' ')
			{
				id_str = id_str + line[i];
				i++;
			}
			//size_t sz;
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
			TreeNode *node = new TreeNode(id,count);
			tree.insert(node);	
		}

		//TreeNode *node = new TreeNode(150,1);
		//tree.insert(node);
		//TreeNode *t = tree.findNode(150);
		//tree.Previous(150);
		//cout<<t->parent->parent->parent->parent->id<<endl;
		
		//tree.inorder(tree.root);
		getline(read_file,line);
		while(line != "quit")
		{
			//cout<<line<<endl;
			string cmd = "";
			string arg1 = "";
			string arg2 = "";
			
			int i=0;
			while(line[i] != ' ')
			{
				cmd += line[i];
				i++;
			}

			if(cmd == "increase")
			{
				i++;
				while(line[i] != ' ')
				{
					arg1 += line[i];
					i++;
				}
				int arg_1 = stoi(arg1,&sz);
				i++;
				while(i < line.length())
				{
					arg2 += line[i];
					i++;
				}
				int arg_2 = stoi(arg2,&sz);
				tree.Increase(arg_1,arg_2);
			}
			else if(cmd == "reduce")
			{
				i++;
				while(line[i] != ' ')
				{
					arg1 += line[i];
					i++;
				}
				int arg_1 = stoi(arg1,&sz);
				i++;
				while(i < line.length())
				{
					arg2 += line[i];
					i++;
				}
				int arg_2 = stoi(arg2,&sz);
				tree.Reduce(arg_1,arg_2);
			}
			else if(cmd == "count")
			{
				i++;
				while(i < line.length())
				{
					arg1 += line[i];
					i++;
				}
				int arg_1 = stoi(arg1,&sz);
				tree.Count(arg_1);
			}
			else if(cmd == "inrange")
			{
				i++;
				while(line[i] != ' ')
				{
					arg1 += line[i];
					i++;
				}
				int arg_1 = stoi(arg1,&sz);
				i++;
				while(i < line.length())
				{
					arg2 += line[i];
					i++;
				}
				int arg_2 = stoi(arg2,&sz);
				tree.InRange(arg_1,arg_2);
			}
			else if(cmd == "next")
			{
				i++;
				while(i < line.length())
				{
					arg1 += line[i];
					i++;
				}
				int arg_1 = stoi(arg1,&sz);
				tree.Next(arg_1);
			}
			else if(cmd == "previous")
			{
				i++;
				while(i < line.length())
				{
					arg1 += line[i];
					i++;
				}
				int arg_1 = stoi(arg1,&sz);
				tree.Previous(arg_1);
			}
			else
				cout<<"Invalid command\n";

			getline(read_file,line);
		}
	}

	else
	{
		cout<<"Error : File open failed\n";
	}
	tree.inorder(tree.root);
	
	end = clock();
	
	cout << "Tree insert time = " << (double(end - start) / CLOCKS_PER_SEC) << " seconds" << '\n';
	cout << "Tree traversal time = " << (double(finalend - end) / CLOCKS_PER_SEC) << " seconds" << '\n';
	

	return 0;
}
