#include<iostream>
#include<string>
#include<fstream>
#include<ctime>
#include "RedBlackTree.h"

using namespace std;

//==================== INORDER ======================================

void RBT::inorder(TreeNode *root)
{
	if(root->count == -1)
		return;

	inorder(root->left);
	cout<<root->id<<"  "<<root->count<<"  "<<root->color<<endl;
	inorder(root->right);
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
	
	return 0;
}
