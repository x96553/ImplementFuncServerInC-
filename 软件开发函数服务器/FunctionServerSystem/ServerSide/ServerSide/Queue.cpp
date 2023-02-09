#include"serverClientLib.h"
#include<iostream>
#include<process.h>
#include<fstream>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
//����������нڵ㹹�캯��
RequestNode::RequestNode(unsigned int paramNum, char* FuncName)
{
	//����ĳ�Ա������ֵ
	this->FuncName = new char[255];
	std::strncpy(this->FuncName, FuncName, 255);
	this->paramNum = paramNum;
	this->paramType = new char[paramNum];
	this->paramLength = new unsigned long[paramNum];
	this->paramContent = new void* [paramNum];
	this->next = 0;
}
//����������нڵ���������
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
//��ʼ������ڵ�
void RequestNode::initializeRequestNode(struct ClientSocket* clientSocket)
{
	for (unsigned int paramCounter = 0; paramCounter < this->paramNum; paramCounter++)
	{
		//���Ͳ�������
		if (recv(clientSocket->Socket, &this->paramType[paramCounter], sizeof(char), 0) == SOCKET_ERROR)
			ServerClient::ErrorHandling("����paramTypeʧ�ܣ�");
		//���Ͳ�������
		if (recv(clientSocket->Socket, (char*)&this->paramLength[paramCounter], sizeof(unsigned long), 0) == SOCKET_ERROR)
			ServerClient::ErrorHandling("����paramLengthʧ�ܣ�");
		//���Ͳ�������
		(*(this->paramContent + paramCounter)) = new char[this->paramLength[paramCounter]];
		if (recv(clientSocket->Socket, (char*)this->paramContent[paramCounter], this->paramLength[paramCounter], 0) == SOCKET_ERROR)
			ServerClient::ErrorHandling("����paramContentʧ�ܣ�");
	}
}
//��ȡ������нڵ�����ݳ�Ա
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
//����������й��캯��
RequestQueue::RequestQueue()
{
	this->front = this->rear = 0;
	this->sizeRequestQueue = 0;
}
//�������������������
RequestQueue::~RequestQueue()
{
	if (this->front)
		delete this->front;
	if (this->rear)
		this->rear = 0;
}
//������нڵ����
void RequestQueue::pushRequestNode(RequestNode* requestNode)
{
	if (this->sizeRequestQueue == 0)
		this->front = requestNode;
	else
		(this->rear)->setNext(requestNode);
	this->rear = requestNode;
	this->sizeRequestQueue++;
}
//������нڵ����
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
//��ȡ������еĴ�С
unsigned int RequestQueue::getSizeRequestQueue()
{
	return this->sizeRequestQueue;
}
//���������нڵ㹹�캯��
ResultNode::ResultNode(unsigned int sizeReturnResult, void* returnResult)
{
	this->sizeReturnResult = sizeReturnResult;
	this->returnResult = new char[this->sizeReturnResult];
	std::strncpy((char*)this->returnResult, (char*)returnResult, this->sizeReturnResult);
	this->next = 0;
}
//���������нڵ���������
ResultNode::~ResultNode()
{
	if (this->next)
		delete this->next;
	if (this->returnResult)
		delete[] this->returnResult;
}
//��ȡ����ֵ
void* ResultNode::getReturnResult()
{
	return this->returnResult;
}
//��ȡ����ֵ��С
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
//������й��캯��
ResultQueue::ResultQueue()
{
	this->front = this->rear = 0;
	this->sizeResultQueue = 0;
}
//���������������
ResultQueue::~ResultQueue()
{
	if (this->front)
		delete this->front;
	if (this->rear)
		this->rear = 0;
}
//������нڵ����
void ResultQueue::pushResultNode(ResultNode* resultNode)
{
	if (this->sizeResultQueue == 0)
		this->front = resultNode;
	else
		(this->rear)->setNext(resultNode);
	this->rear = resultNode;
	this->sizeResultQueue++;
}
//������нڵ����
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
//��ȡ������еĴ�С
unsigned int ResultQueue::getSizeResultQueue()
{
	return this->sizeResultQueue;
}
//����ͻ��˷��ʷ�����ʱ�Ĵ洢�Ŀͻ����׽��ֶ��еĹ��캯��
ClientSocketQueue::ClientSocketQueue()
{
	this->front = this->rear = 0;
	this->sizeClientSocketQueue = 0;
}
//����ͻ��˷��ʷ�����ʱ�Ĵ洢�Ŀͻ����׽��ֶ��е���������
ClientSocketQueue::~ClientSocketQueue()
{
	if (this->front)
		delete this->front;
	if (this->rear)
		this->rear = 0;
}
//�Կͻ����׽������
void ClientSocketQueue::pushClientSocketNode(ClientSocketNode* clientSocketNode)
{
	if (this->sizeClientSocketQueue == 0)
		this->front = clientSocketNode;
	else
		(this->rear)->setNext(clientSocketNode);
	this->rear = clientSocketNode;
	this->sizeClientSocketQueue++;
}
//�Կͻ����׽��ֳ���
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
////��ȡ�����е�һ���ͻ����׽���
ClientSocketNode* ClientSocketQueue::getFront()
{
	return this->front;
}
//��ȡ�ͻ����׽��ֵĴ�С
unsigned int ClientSocketQueue::getSizeClientSocketQueue()
{
	return this->sizeClientSocketQueue;
}
//����ͻ��˷��ʷ�����ʱ�Ĵ洢�Ŀͻ����׽��ֶ��нڵ�Ĺ��캯��
ClientSocketNode::ClientSocketNode(ClientSocket clientSocket)
{
	this->clientSocketNew = new struct ClientSocket;
	(this->clientSocketNew)->Address = clientSocket.Address;
	(this->clientSocketNew)->AddressSize = clientSocket.AddressSize;
	(this->clientSocketNew)->Socket = clientSocket.Socket;
	this->next = 0;
}
//����ͻ��˷��ʷ�����ʱ�Ĵ洢�Ŀͻ����׽��ֶ��нڵ����������
ClientSocketNode::~ClientSocketNode()
{
	if (this->next)
		delete this->next;
	if (this->clientSocketNew)
		delete this->clientSocketNew;
}
//��ȡ���нڵ��ϵĿͻ����׽���
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