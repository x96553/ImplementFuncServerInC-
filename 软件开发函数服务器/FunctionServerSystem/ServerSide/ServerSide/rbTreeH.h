#ifndef _RBTREEH_H_
#define _RBTREEH_H_
#include<iostream>
using namespace std;
//����ö�����ͣ������ֺ�ڽڵ�
enum class RBColor { RED, BLACK };
class RBNode;
//��������
class RBTree {
public:
	RBTree(RBNode* nil);
	~RBTree();
	int DestroyRBTree(RBTree** tree);
	void AddNodeC(RBTree** tree, RBNode* node);
	void ModifyNodeC(RBTree** tree, RBNode* node, unsigned int sizeDLL);
	int BrowseNodeC(RBTree* tree, string funcCmdBuf, RBNode** findNode);
	void DeleteNodeC(RBTree** tree, RBNode** findNode);
private:
	RBNode* root;
	RBNode* nil;
	void AddNodeE(RBTree** tree, RBNode* node);
	void InsertFixUp(RBTree** tree, RBNode* node);
	void LeftRotate(RBTree** tree, RBNode* node);
	void RightRotate(RBTree** tree, RBNode* node);
	RBNode* ModifyNodeE(RBTree** tree, RBNode* node);
	void BrowseNodeE(RBNode* node, string funcCmdBuf, RBNode** findNode);
	void DeleteNodeE(RBTree** tree, RBNode** findNode);
	void TransplantRB(RBTree** tree, RBNode** beReplacedNode, RBNode** replaceNode);
	RBNode* MiniMuxRB(RBTree** tree, RBNode* beReplacedNode);
	void DeleteFixUpRB(RBTree** tree, RBNode** recordNode);
};
//���������ڵ�
class RBNode {
	//������ڵ����Ԫ��
	friend class  RBTree;
public:
	RBNode(char* funcName, unsigned int funcStartPos, unsigned int funcSize);
	~RBNode();
	void DestroyRBNode(RBNode** node, RBNode* nil);
	char* getFuncName();
	unsigned int getfuncStartPos();
	unsigned int getfuncSize();
private:
	char* funcName;
	unsigned int funcStartPos;
	unsigned int funcSize;
	RBColor color;
	RBNode* lchild;
	RBNode* rchild;
	RBNode* father;
};
#endif // !_RBTREEH_H_
