#include<string>

class TreeNode
{
	public:
		TreeNode *left;
		TreeNode *right;
		TreeNode *parent;
		std::string color;
		int id;
		int count;
		TreeNode(){}
		TreeNode(int,int);
};
