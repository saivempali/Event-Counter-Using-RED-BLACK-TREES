#include<iostream>
#include<string>
#include<fstream>
#include<ctime>

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

//==================== INORDER ======================================

void RBT::inorder(TreeNode *root)
{
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
				string temp = parent->color;
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
				string temp = parent->color;
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
	string y_original_color = y->color;
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
		TreeNode *newEvent = new TreeNode(theID,1);
		insert(newEvent);
		TreeNode *prevEvent = inorderPredecessor(newEvent);
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
	}
}

int total = 0;

//================== IN RANGE HELPER ===========================

void RBT::InRangeHelper(TreeNode* root, int id1, int id2)
{
	if(root->count == -1)
		return;

	if(id1 < root->id)
		InRangeHelper(root->left,id1,id2);

	if(id1 <= root->id && id2 >= root->id)
	{
		total += root->count;
	}

	if(id2 > root->id)
		InRangeHelper(root->right,id1,id2);
	
}

//======================= IN RANGE =============================

void RBT::InRange(int ID1, int ID2)
{
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

//===================== BUILD TREE =========================

void RBT::buildTree(int id, int count)
{
	TreeNode *newNode = new TreeNode(id,count);
	rightmost->right = newNode;
	newNode->parent = rightmost;
	rightmost = newNode;
	fixInsert(newNode);
}


int main(int argc, char *argv[])
{
//	clock_t start, end, finalend;
//    	start = clock();
	RBT tree;
	int numNodes;
	string input_file = argv[1];
	const char* input = input_file.c_str();
	ifstream read_file;
	
	read_file.open(input);
	if(read_file.is_open())
	{
		string line = "";
		string id_str = "";
		string count_str = "";	
		getline(read_file, line);
		size_t sz;
		numNodes = stoi(line,&sz);
		for(int loopIndex=0; loopIndex < numNodes; loopIndex++)
		{
			getline(read_file, line);
			int id,count;
			int i=0;
			while(line[i] != ' ')
			{
				id_str = id_str + line[i];
				i++;
			}
			id = stoi(id_str,&sz);
			id_str = "";

			i++;
			while(i < line.length())
			{
				count_str = count_str + line[i];
				i++;
			}
			count = stoi(count_str, &sz);
			count_str = "";
			tree.buildTree(id,count);
		}
		
		getline(cin,line);
		cout<<line<<endl;
		while(line != "quit")
		{
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

			getline(cin,line);	
		}
	}

	else
	{
		cout<<"Error : File open failed\n";
	}
	
//	end = clock();
	
//	cout << "Tree insert time = " << (double(end - start) / CLOCKS_PER_SEC) << " seconds" << '\n';
//	cout << "Tree traversal time = " << (double(finalend - end) / CLOCKS_PER_SEC) << " seconds" << '\n';
	

	return 0;
}
