#include"rbTreeH.h"
#include<fstream>
#include<iostream>
using namespace std;
//红黑树节点的构造函数
RBNode::RBNode(char* funcName, unsigned int funcStartPos, unsigned int funcSize) :funcName(funcName)
{
	this->funcStartPos = funcStartPos;
	this->funcSize = funcSize;
	this->color = RBColor::RED;
	this->lchild = 0;
	this->rchild = 0;
	this->father = 0;
}
//红黑树节点的析构函数
RBNode::~RBNode()
{
	delete[] this->funcName;
}
//调用红黑树节点的析构函数删除所有红黑树的节点
void RBNode::DestroyRBNode(RBNode** node, RBNode* nil)
{
	RBNode* fathterNode = (*node)->father;
	RBNode* currentNode = (*node);
	if ((*node)->lchild != nil)
		RBNode::DestroyRBNode(&(*node)->lchild, nil);
	if ((*node)->rchild != nil)
		RBNode::DestroyRBNode(&(*node)->rchild, nil);
	if (fathterNode != nil)
	{
		if (fathterNode->lchild == (*node))
			fathterNode->lchild = nil;
		if (fathterNode->rchild == (*node))
			fathterNode->rchild = nil;
	}
	delete currentNode;
}
//获取红黑树节点函数名的函数
char* RBNode::getFuncName()
{
	return funcName;
}
unsigned int RBNode::getfuncStartPos()
{
	return funcStartPos;
}
unsigned int RBNode::getfuncSize()
{
	return funcSize;
}
//红黑树构造函数
RBTree::RBTree(RBNode* nil)
{
	this->root = nil;
	this->nil = nil;
	((this->nil)->color) = RBColor::BLACK;
}
//红黑树析构函数
RBTree::~RBTree()
{
	if (this->root != this->nil)
	{
		this->root->DestroyRBNode(&this->root, this->nil);
		this->root = this->nil;
	}
	if (this->nil)
	{
		delete this->nil;
		this->nil = 0;
		this->root = 0;
	}
}
//调用红黑树的析构函数删除红黑树
int RBTree::DestroyRBTree(RBTree** tree)
{
	if ((*tree))
	{
		delete (*tree);
		(*tree) = 0;
	}
	return 0;
}
//增加红黑树节点函数
void RBTree::AddNodeC(RBTree** tree, RBNode* node)
{
	RBTree::AddNodeE(tree, node);
}
void  RBTree::AddNodeE(RBTree** tree, RBNode* node)
{
	RBNode* father = (*tree)->nil;
	RBNode* current = (*tree)->root;
	while (current != (*tree)->nil)
	{
		father = current;
		if (strcmp(node->funcName, current->funcName) < 0)
			current = current->lchild;
		else
			current = current->rchild;
	}
	node->father = father;
	if (father == (*tree)->nil)
		(*tree)->root = node;
	else if (strcmp(node->funcName, father->funcName) < 0)
		father->lchild = node;
	else
		father->rchild = node;
	node->lchild = (*tree)->nil;
	node->rchild = (*tree)->nil;
	RBTree::InsertFixUp(tree, node);
}
//修改红黑树节点的颜色
void RBTree::InsertFixUp(RBTree** tree, RBNode* node)
{
	while (node->father->color == RBColor::RED)
	{
		if (node->father == node->father->father->lchild)
		{
			RBNode* uncle = node->father->father->rchild;
			if (uncle->color == RBColor::RED)
			{
				node->father->color = RBColor::BLACK;
				uncle->color = RBColor::BLACK;
				node->father->father->color = RBColor::RED;
				node = node->father->father;
			}
			else
			{
				if (node == node->father->rchild)
				{
					node = node->father;
					RBTree::LeftRotate(tree, node);
				}
				node->father->color = RBColor::BLACK;
				node->father->father->color = RBColor::RED;
				RBTree::RightRotate(tree, node->father->father);
			}
		}
		else if (node->father == node->father->father->rchild)
		{
			RBNode* uncle = node->father->father->lchild;
			if (uncle->color == RBColor::RED)
			{
				node->father->color = RBColor::BLACK;
				uncle->color = RBColor::BLACK;
				node->father->father->color = RBColor::RED;
				node = node->father->father;
			}
			else
			{
				if (node == node->father->lchild)
				{
					node = node->father;
					RBTree::RightRotate(tree, node);
				}
				node->father->color = RBColor::BLACK;
				node->father->father->color = RBColor::RED;
				RBTree::LeftRotate(tree, node->father->father);
			}
		}
	}
	(*tree)->root->color = RBColor::BLACK;
}
//左旋红黑树
void RBTree::LeftRotate(RBTree** tree, RBNode* node)
{
	RBNode* rotate = node->rchild;
	node->rchild = rotate->lchild;
	if (rotate->lchild != (*tree)->nil)
		rotate->lchild->father = node;
	rotate->father = node->father;
	if (node->father == (*tree)->nil)
		(*tree)->root = rotate;
	else if (node == node->father->lchild)
		node->father->lchild = rotate;
	else
		node->father->rchild = rotate;
	rotate->lchild = node;
	node->father = rotate;
}
//右旋红黑树
void RBTree::RightRotate(RBTree** tree, RBNode* node)
{
	RBNode* rotate = node->lchild;
	node->lchild = rotate->rchild;
	if (rotate->rchild != (*tree)->nil)
		rotate->rchild->father = node;
	rotate->father = node->father;
	if (node->father == (*tree)->nil)
		(*tree)->root = rotate;
	else if (node == node->father->rchild)
		node->father->rchild = rotate;
	else
		node->father->lchild = rotate;
	rotate->rchild = node;
	node->father = rotate;
}
//修改红黑树节点函数
void RBTree::ModifyNodeC(RBTree** tree, RBNode* node, unsigned int sizeDLL)
{
	RBNode* findModifyNode = RBTree::ModifyNodeE(tree, node);
	findModifyNode->funcSize = sizeDLL;
}
RBNode* RBTree::ModifyNodeE(RBTree** tree, RBNode* node)
{
	RBNode* findModifyNode = (*tree)->root;
	while (findModifyNode != (*tree)->nil && strcmp(findModifyNode->funcName, node->funcName) != 0)
	{
		if (strcmp(node->funcName, findModifyNode->funcName) < 0)
			findModifyNode = findModifyNode->lchild;
		else
			findModifyNode = findModifyNode->rchild;
	}
	return findModifyNode;
}
//浏览红黑树节点函数
int RBTree::BrowseNodeC(RBTree* tree, string funcCmdBuf, RBNode** findNode)
{
	(*findNode) = 0;
	RBTree::BrowseNodeE(tree->root, funcCmdBuf, findNode);
	if ((*findNode))
		return 1;
	return 0;
}
void RBTree::BrowseNodeE(RBNode* node, string funcCmdBuf, RBNode** findNode)
{
	std::string::size_type funcStart = std::string::npos;
	string nodeFunc;
	funcStart = string(node->funcName).find_last_of('.');
	nodeFunc = string(node->funcName).substr(funcStart + 1, strlen(node->funcName) - funcStart - 1);
	if (nodeFunc == funcCmdBuf)
		(*findNode) = node;
	if (node->lchild != this->nil && node->lchild != 0)
		RBTree::BrowseNodeE(node->lchild, funcCmdBuf, findNode);
	if (node->rchild != this->nil && node->rchild != 0)
		RBTree::BrowseNodeE(node->rchild, funcCmdBuf, findNode);
}
//删除红黑树节点函数
void RBTree::DeleteNodeC(RBTree** tree, RBNode** findNode)
{
	RBTree::DeleteNodeE(tree, findNode);
	if ((*findNode)->father != (*tree)->nil)
		(*findNode)->father = (*tree)->nil;
	if ((*findNode)->lchild != (*tree)->nil)
		(*findNode)->lchild = (*tree)->nil;
	if ((*findNode)->rchild != (*tree)->nil)
		(*findNode)->rchild = (*tree)->nil;
}
void RBTree::DeleteNodeE(RBTree** tree, RBNode** findNode)
{
	//定义变量
	RBNode* recordNode = 0;
	RBNode* replaceNode = (*findNode);
	//保存被替换节点颜色
	RBColor replaceNodeOriginalColor = replaceNode->color;
	//对被删除节点有无孩子及有几个孩子情况进行判断
	if ((*findNode)->lchild = (*tree)->nil)
	{
		recordNode = (*findNode)->rchild;
		RBTree::TransplantRB(tree, findNode, &(*findNode)->rchild);
	}
	else if ((*findNode)->rchild = (*tree)->nil)
	{
		recordNode = (*findNode)->lchild;
		RBTree::TransplantRB(tree, findNode, &(*findNode)->lchild);
	}
	else if ((*findNode)->lchild != (*tree)->nil && (*findNode)->rchild != (*tree)->nil)
	{
		replaceNode = RBTree::MiniMuxRB(tree, (*findNode)->rchild);
		replaceNodeOriginalColor = replaceNode->color;
		recordNode = replaceNode->rchild;
		if (replaceNode->father == (*findNode))
			recordNode->father = replaceNode;
		else
		{
			RBTree::TransplantRB(tree, &replaceNode, &replaceNode->rchild);
			replaceNode->rchild = (*findNode)->rchild;
			replaceNode->rchild->father = replaceNode;
		}
		RBTree::TransplantRB(tree, findNode, &replaceNode);
		replaceNode->lchild = (*findNode)->lchild;
		replaceNode->lchild->father = replaceNode;
		replaceNode->color = (*findNode)->color;
	}
	if (replaceNodeOriginalColor == RBColor::BLACK)
		RBTree::DeleteFixUpRB(tree, &recordNode);
}
//移动红黑树的子树
void RBTree::TransplantRB(RBTree** tree, RBNode** beReplacedNode, RBNode** replaceNode)
{
	if ((*beReplacedNode)->father == (*tree)->nil)
		(*tree)->root = (*replaceNode);
	else if ((*beReplacedNode) == (*beReplacedNode)->father->lchild)
		(*beReplacedNode)->father->lchild = (*replaceNode);
	else if ((*beReplacedNode) == (*beReplacedNode)->father->rchild)
		(*beReplacedNode)->father->rchild = (*replaceNode);
	(*replaceNode)->father = (*beReplacedNode)->father;
}
//查找树或子树中的最小值
RBNode* RBTree::MiniMuxRB(RBTree** tree, RBNode* beReplacedNode)
{
	RBNode* MiniMuxNode = beReplacedNode;
	while (MiniMuxNode->lchild != (*tree)->nil)
		MiniMuxNode = MiniMuxNode->lchild;
	return MiniMuxNode;
}
//对删除后的红黑树进行旋转红黑树重新平衡
void RBTree::DeleteFixUpRB(RBTree** tree, RBNode** recordNode)
{
	while ((*recordNode) != (*tree)->root && (*recordNode)->color == RBColor::BLACK)
	{
		if ((*recordNode) == (*recordNode)->father->lchild)
		{
			RBNode* brotherNode = (*recordNode)->father->rchild;
			if (brotherNode->color == RBColor::RED)
			{
				brotherNode->color = RBColor::BLACK;
				(*recordNode)->father->color = RBColor::RED;
				RBTree::LeftRotate(tree, (*recordNode)->father);
				brotherNode = (*recordNode)->father->rchild;
			}
			if (brotherNode->lchild->color == RBColor::BLACK && brotherNode->rchild->color == RBColor::BLACK)
			{
				brotherNode->color = RBColor::RED;
				(*recordNode) = (*recordNode)->father;
			}
			else if (brotherNode->rchild->color == RBColor::BLACK)
			{
				brotherNode->lchild->color == RBColor::BLACK;
				brotherNode->color = RBColor::RED;
				RBTree::RightRotate(tree, brotherNode);
				brotherNode = (*recordNode)->father->rchild;
			}
			brotherNode->color = (*recordNode)->father->color;
			(*recordNode)->father->color = RBColor::BLACK;
			brotherNode->rchild->color = RBColor::BLACK;
			RBTree::LeftRotate(tree, (*recordNode)->father);
			(*recordNode) = (*tree)->root;
		}
		else if ((*recordNode) == (*recordNode)->father->rchild)
		{
			RBNode* brotherNode = (*recordNode)->father->lchild;
			if (brotherNode->color == RBColor::RED)
			{
				brotherNode->color = RBColor::BLACK;
				(*recordNode)->father->color = RBColor::RED;
				RBTree::RightRotate(tree, (*recordNode)->father);
				brotherNode = (*recordNode)->father->lchild;
			}
			if (brotherNode->rchild->color == RBColor::BLACK && brotherNode->lchild->color == RBColor::BLACK)
			{
				brotherNode->color = RBColor::RED;
				(*recordNode) = (*recordNode)->father;
			}
			else if (brotherNode->lchild->color == RBColor::BLACK)
			{
				brotherNode->rchild->color == RBColor::BLACK;
				brotherNode->color = RBColor::RED;
				RBTree::LeftRotate(tree, brotherNode);
				brotherNode = (*recordNode)->father->lchild;
			}
			brotherNode->color = (*recordNode)->father->color;
			(*recordNode)->father->color = RBColor::BLACK;
			brotherNode->lchild->color = RBColor::BLACK;
			RBTree::RightRotate(tree, (*recordNode)->father);
			(*recordNode) = (*tree)->root;
		}
	}
	(*recordNode)->color = RBColor::BLACK;
}
