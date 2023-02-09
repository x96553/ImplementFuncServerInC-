#include"serverClientLib.h"
#include<iostream>
#include<process.h>
#include<fstream>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
//定义请求队列节点构造函数
RequestNode::RequestNode(unsigned int paramNum, char* FuncName)
{
	//对类的成员变量赋值
	this->FuncName = new char[255];
	std::strncpy(this->FuncName, FuncName, 255);
	this->paramNum = paramNum;
	this->paramType = new char[paramNum];
	this->paramLength = new unsigned long[paramNum];
	this->paramContent = new void* [paramNum];
	this->next = 0;
}
//定义请求队列节点析构函数
RequestNode::~RequestNode()
{
	if (this->next)
		delete this->next;
	for (unsigned int paramCounter = 0; paramCounter < this->paramNum; paramCounter++)
	{
		if (*(this->paramContent + paramCounter))
			delete[] * (this->paramContent + paramCounter);
	}
	if (this->paramContent)
		delete[] this->paramContent;
	if (this->paramLength)
		delete[] this->paramLength;
	if (this->paramType)
		delete[] this->paramType;
	if (this->FuncName)
		delete[] this->FuncName;
}
//初始化请求节点
void RequestNode::initializeRequestNode(struct ClientSocket* clientSocket)
{
	for (unsigned int paramCounter = 0; paramCounter < this->paramNum; paramCounter++)
	{
		//发送参数类型
		if (recv(clientSocket->Socket, &this->paramType[paramCounter], sizeof(char), 0) == SOCKET_ERROR)
			ServerClient::ErrorHandling("发送paramType失败！");
		//发送参数长度
		if (recv(clientSocket->Socket, (char*)&this->paramLength[paramCounter], sizeof(unsigned long), 0) == SOCKET_ERROR)
			ServerClient::ErrorHandling("发送paramLength失败！");
		//发送参数内容
		(*(this->paramContent + paramCounter)) = new char[this->paramLength[paramCounter]];
		if (recv(clientSocket->Socket, (char*)this->paramContent[paramCounter], this->paramLength[paramCounter], 0) == SOCKET_ERROR)
			ServerClient::ErrorHandling("发送paramContent失败！");
	}
}
//获取请求队列节点的数据成员
char* RequestNode::getFuncName()
{
	return this->FuncName;
}
unsigned int RequestNode::getParamNum()
{
	return this->paramNum;
}
char* RequestNode::getParamType()
{
	return this->paramType;
}
unsigned long* RequestNode::getParamLength()
{
	return this->paramLength;
}
void** RequestNode::getParamContent()
{
	return this->paramContent;
}
RequestNode* RequestNode::getNext()
{
	return this->next;
}
void RequestNode::setNext(RequestNode* next)
{
	this->next = next;
}
//定义请求队列构造函数
RequestQueue::RequestQueue()
{
	this->front = this->rear = 0;
	this->sizeRequestQueue = 0;
}
//定义请求队列析构函数
RequestQueue::~RequestQueue()
{
	if (this->front)
		delete this->front;
	if (this->rear)
		this->rear = 0;
}
//请求队列节点入队
void RequestQueue::pushRequestNode(RequestNode* requestNode)
{
	if (this->sizeRequestQueue == 0)
		this->front = requestNode;
	else
		(this->rear)->setNext(requestNode);
	this->rear = requestNode;
	this->sizeRequestQueue++;
}
//请求队列节点出队
RequestNode* RequestQueue::popRequestNode()
{
	if (this->sizeRequestQueue >= 1)
	{
		RequestNode* popRequestNode = this->front;
		this->front = (this->front)->getNext();
		popRequestNode->setNext(0);
		this->sizeRequestQueue--;
		return popRequestNode;
	}
	return 0;
}
//获取请求队列的大小
unsigned int RequestQueue::getSizeRequestQueue()
{
	return this->sizeRequestQueue;
}
//定义结果队列节点构造函数
ResultNode::ResultNode(unsigned int sizeReturnResult, void* returnResult)
{
	this->sizeReturnResult = sizeReturnResult;
	this->returnResult = new char[this->sizeReturnResult];
	std::strncpy((char*)this->returnResult, (char*)returnResult, this->sizeReturnResult);
	this->next = 0;
}
//定义结果队列节点析构函数
ResultNode::~ResultNode()
{
	if (this->next)
		delete this->next;
	if (this->returnResult)
		delete[] this->returnResult;
}
//获取返回值
void* ResultNode::getReturnResult()
{
	return this->returnResult;
}
//获取返回值大小
unsigned int ResultNode::getSizeReturnResult()
{
	return this->sizeReturnResult;
}
ResultNode* ResultNode::getNext()
{
	return this->next;
}
void ResultNode::setNext(ResultNode* next)
{
	this->next = next;
}
//结果队列构造函数
ResultQueue::ResultQueue()
{
	this->front = this->rear = 0;
	this->sizeResultQueue = 0;
}
//结果队列析构函数
ResultQueue::~ResultQueue()
{
	if (this->front)
		delete this->front;
	if (this->rear)
		this->rear = 0;
}
//结果队列节点入队
void ResultQueue::pushResultNode(ResultNode* resultNode)
{
	if (this->sizeResultQueue == 0)
		this->front = resultNode;
	else
		(this->rear)->setNext(resultNode);
	this->rear = resultNode;
	this->sizeResultQueue++;
}
//结果队列节点出队
ResultNode* ResultQueue::popResultNode()
{
	if (this->sizeResultQueue >= 1)
	{
		ResultNode* popResultNode = this->front;
		this->front = (this->front)->getNext();
		popResultNode->setNext(0);
		this->sizeResultQueue--;
		return popResultNode;
	}
	return 0;
}
//获取结果队列的大小
unsigned int ResultQueue::getSizeResultQueue()
{
	return this->sizeResultQueue;
}
//定义客户端访问服务器时的存储的客户端套接字队列的构造函数
ClientSocketQueue::ClientSocketQueue()
{
	this->front = this->rear = 0;
	this->sizeClientSocketQueue = 0;
}
//定义客户端访问服务器时的存储的客户端套接字队列的析构函数
ClientSocketQueue::~ClientSocketQueue()
{
	if (this->front)
		delete this->front;
	if (this->rear)
		this->rear = 0;
}
//对客户端套接字入队
void ClientSocketQueue::pushClientSocketNode(ClientSocketNode* clientSocketNode)
{
	if (this->sizeClientSocketQueue == 0)
		this->front = clientSocketNode;
	else
		(this->rear)->setNext(clientSocketNode);
	this->rear = clientSocketNode;
	this->sizeClientSocketQueue++;
}
//对客户端套接字出队
ClientSocketNode* ClientSocketQueue::popClientSocketNode()
{
	if (this->sizeClientSocketQueue >= 1)
	{
		ClientSocketNode* clientSocketNode = this->front;
		this->front = (this->front)->getNext();
		clientSocketNode->setNext(0);
		this->sizeClientSocketQueue--;
		return clientSocketNode;
	}
	return 0;
}
////获取队列中第一个客户端套接字
ClientSocketNode* ClientSocketQueue::getFront()
{
	return this->front;
}
//获取客户端套接字的大小
unsigned int ClientSocketQueue::getSizeClientSocketQueue()
{
	return this->sizeClientSocketQueue;
}
//定义客户端访问服务器时的存储的客户端套接字队列节点的构造函数
ClientSocketNode::ClientSocketNode(ClientSocket clientSocket)
{
	this->clientSocketNew = new struct ClientSocket;
	(this->clientSocketNew)->Address = clientSocket.Address;
	(this->clientSocketNew)->AddressSize = clientSocket.AddressSize;
	(this->clientSocketNew)->Socket = clientSocket.Socket;
	this->next = 0;
}
//定义客户端访问服务器时的存储的客户端套接字队列节点的析构函数
ClientSocketNode::~ClientSocketNode()
{
	if (this->next)
		delete this->next;
	if (this->clientSocketNew)
		delete this->clientSocketNew;
}
//获取队列节点上的客户端套接字
struct ClientSocket* ClientSocketNode::getClientSocketNew()
{
	return this->clientSocketNew;
}
ClientSocketNode* ClientSocketNode::getNext()
{
	return this->next;
}
void ClientSocketNode::setNext(ClientSocketNode* next)
{
	this->next = next;
}