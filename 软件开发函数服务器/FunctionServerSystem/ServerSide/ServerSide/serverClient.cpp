#include<iostream>
#include<process.h>
#include<fstream>
#include<Windows.h>
#include<WinSock.h>
#include"serverClientLib.h"
#pragma comment(lib,"ws2_32.lib")
using namespace std;
ServerClient::ServerClient()
{
	this->funcNameNil = new char[255];
	while (!this->funcNameNil)
		this->funcNameNil = new char[255];
	this->nil = new RBNode(funcNameNil, 0, 0);
	while (!this->nil)
		this->nil = new RBNode(funcNameNil, 0, 0);
	this->tree = new RBTree(nil);
	while (!this->tree)
		this->tree = new RBTree(nil);
	this->nameLib = "E:\\FunctionServerSystem\\ServerSide\\FuncSlibs.LIB";
	this->tempFileName = "E:\\FunctionServerSystem\\ServerSide\\ServerSide\\tempFile.dll";
}
ServerClient::~ServerClient()
{
	//���������
	(this->tree)->DestroyRBTree(&this->tree);
	//�����������
	if (this->requestQueue)
		delete this->requestQueue;
	//�����������
	if (this->resultQueue)
		delete this->resultQueue;
	//�����ͻ����׽��ֶ���
	if (this->clientSocketQueue)
		delete this->clientSocketQueue;
}
void ServerClient::openLibServerClient()
{
	//�����������ʼ��
	int CycleFuncCallThreadID, CycleResultThreadID;
	//��LIB���������ݳ�Ա��ʼ��
	ServerClient::openLibCreateTree();
	this->requestQueueRegion = { 0 };
	this->resultQueueRegion = { 0 };
	this->requestQueue = new RequestQueue();
	this->resultQueue = new ResultQueue;
	this->numRequestNode = 0;
	this->numResultNode = 0;
	this->clientSocketQueue = new ClientSocketQueue;
	this->serverSocket = { 0 };
	this->clientSocket = { 0 };
	//��ʼ�������ٽ����ı������������
	InitializeCriticalSection(&this->requestQueueRegion);
	InitializeCriticalSection(&this->resultQueueRegion);
	//����ѭ���ĺ��������̺߳ͽ���߳�
	CycleFuncCallThreadID = _beginthread(ServerClient::CycleFuncCallThread, 0, this);
	CycleResultThreadID = _beginthread(ServerClient::CycleResultThread, 0, this);
	//��ʼ���׽��ֶ�̬��,���Լ���������
	if (WSAStartup(MAKEWORD(2, 2), &(this->serverSocket).wsaData) != 0)
		ServerClient::ErrorHandling("WSAStartup() error!");
	(this->serverSocket).Socket = socket(PF_INET, SOCK_STREAM, 0);
	if ((this->serverSocket).Socket == INVALID_SOCKET)
		ServerClient::ErrorHandling("socket() error!");
	memset(&(this->serverSocket).Address, 0, sizeof((this->serverSocket).Address));
	(this->serverSocket).Address.sin_family = AF_INET;
	(this->serverSocket).Address.sin_addr.S_un.S_addr = INADDR_ANY;
	(this->serverSocket).Address.sin_port = htons(atoi("789"));
	if (bind((this->serverSocket).Socket, (SOCKADDR*)&(this->serverSocket).Address, sizeof((this->serverSocket).Address)) == SOCKET_ERROR)
		ServerClient::ErrorHandling("bind() error!");
	if (listen((this->serverSocket).Socket, 3) == SOCKET_ERROR)
		ServerClient::ErrorHandling("listen() error!");
	(this->clientSocket).AddressSize = sizeof((this->clientSocket).Address);
	//ѭ�����տͻ�������
	while (1)
	{
		(this->clientSocket).Socket = accept((this->serverSocket).Socket, (SOCKADDR*)&(this->clientSocket).Address, &(this->clientSocket).AddressSize);
		if ((this->clientSocket).Socket == SOCKET_ERROR)
			ServerClient::ErrorHandling("accept() error!");
		else
		{
			int ReceiveThreadID;
			ClientSocketNode* clientSocketNode = new ClientSocketNode(this->clientSocket);
			this->clientSocketQueue->pushClientSocketNode(clientSocketNode);
			ReceiveThreadID = _beginthread(ServerClient::createReceiveThread, 0, this);
		}
	}
}
void ServerClient::openLibCreateTree()
{
	//�������
	char funcBlock[11] = { 0 };
	unsigned int fileCur, fileEnd;
	//�ر�LIB�ļ�
	while ((this->fileLIB).is_open())
		(this->fileLIB).close();
	//��LIB�ļ�
	(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
	while (!(this->fileLIB).is_open())
		(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
	//�����ļ�ָ�뵽�ļ���ʼλ��
	(this->fileLIB).seekg(0, ios::end);
	fileEnd = (this->fileLIB).tellg();
	(this->fileLIB).seekg(0, ios::beg);
	fileCur = (this->fileLIB).tellg();
	//���������
	while (1)
	{
		//�ж��Ƿ񵽴��ļ�ĩβ
		if (fileCur == fileEnd)
			break;
		//���ļ��ж�ȡʮ���ַ���funcBlock������
		(this->fileLIB).read(funcBlock, strlen("Func Block"));
		//�ж�funcBlock�����е��ַ����롰func Block���Ƿ���ȣ��������ִ��if���
		if (strncmp(funcBlock, "Func Block", strlen("Func Block")) == 0)
		{
			//�������
			unsigned int funcStartPos, funcSize, funcSizeI;
			char* funcName;
			RBNode* node;
			funcSize = 0;
			funcSizeI = 1;
			//��ȡ������ռ�ֽڴ�С������funcSize��
			while (1)
			{
				if (funcSizeI == unsigned int(10000000000))
					break;
				funcSize = funcSize + funcSizeI * fileLIB.get();
				funcSizeI = funcSizeI * 10;
			}
			//Ϊ�洢�������Ʒ����ڴ棬����LIB�ļ��ж��뺯�����Ʋ�����funcName��
			funcName = new char[255];
			while (!funcName)
				funcName = new char[255];
			(this->fileLIB).read(funcName, 255);
			//��ȡ������LIB�ļ��е���ʼλ�ã�������funcStartPos��
			funcStartPos = (this->fileLIB).tellg();
			//����������ڵ�
			node = new RBNode(funcName, funcStartPos, funcSize);
			while (!node)
				node = new RBNode(funcName, funcStartPos, funcSize);
			//��������ڵ����������
			(this->tree)->AddNodeC(&this->tree, node);
		}
		fileCur++;
		(this->fileLIB).seekg(fileCur, ios::beg);
	}
}
//ѭ���������������߳�
void ServerClient::CycleFuncCallThread(void* thisThread)
{
	//����ͻ����׽���ָ����մ����̵߳Ĳ���
	ServerClient* serverClient = (ServerClient*)thisThread;
	int FuncCallThreadID;
	unsigned int numRequestNode;
	while (1)
	{
		numRequestNode = serverClient->getNumRequestNode();
		while (1)
		{
			if (numRequestNode < 1)
				break;
			else
			{
				FuncCallThreadID = _beginthread(ServerClient::createFuncCallThread, 0, serverClient);
				numRequestNode--;
				serverClient->setNumRequestNode(numRequestNode);
			}
		}
	}
}
//ѭ����������߳�
void ServerClient::CycleResultThread(void* thisThread)
{
	//����ͻ����׽���ָ����մ����̵߳Ĳ���
	ServerClient* serverClient = (ServerClient*)thisThread;
	int ResultThreadID;
	unsigned int numResultNode;
	while (1)
	{
		numResultNode = serverClient->getNumResultNode();
		while (1)
		{
			if (numResultNode < 1)
				break;
			else
			{
				ResultThreadID = _beginthread(ServerClient::createResultThread, 0, serverClient);
				numResultNode--;
				serverClient->setNumResultNode(numResultNode);
			}
		}
	}
}
//���������̴߳�������
void ServerClient::createReceiveThread(void* thisThread)
{
	//����ͻ����׽���ָ����մ����̵߳Ĳ���
	ServerClient* serverClient = (ServerClient*)thisThread;
	struct ClientSocket* clientSocket = ((ServerClient*)thisThread)->clientSocketQueue->getFront()->getClientSocketNew();
	//����洢������������,�洢�����������������α���,������нڵ�
	RequestNode* requestNode;
	char FuncName[255] = { 0 };
	unsigned int paramNum = 0;
	//�������˽��պ�����
	if (recv(clientSocket->Socket, FuncName, 255, 0) == SOCKET_ERROR)
		ServerClient::ErrorHandling("The receive function name failed");
	//�������˽��պ����Ĳ�������
	if (recv(clientSocket->Socket, (char*)&paramNum, sizeof(unsigned int), 0) == SOCKET_ERROR)
		ServerClient::ErrorHandling("Number of received parameters failed!");
	//����ӽڵ���г�ʼ��
	requestNode = new RequestNode(paramNum, FuncName);
	requestNode->initializeRequestNode(clientSocket);
	//�Գ�ʼ���Ľڵ����
	EnterCriticalSection(&serverClient->requestQueueRegion);
	serverClient->requestQueue->pushRequestNode(requestNode);
	LeaveCriticalSection(&serverClient->requestQueueRegion);
	//���ö��еĽڵ���
	serverClient->setNumRequestNode(serverClient->requestQueue->getSizeRequestQueue());
}
//���庯�������̴߳�������
void ServerClient::createFuncCallThread(void* thisThread)
{
	//�������������ͻ����׽���ָ����մ����̵߳Ĳ���
	ServerClient* serverClient = (ServerClient*)thisThread;
	RBNode* findNode;
	RequestNode* requestNode;
	EnterCriticalSection(&serverClient->requestQueueRegion);
	requestNode = serverClient->requestQueue->popRequestNode();
	LeaveCriticalSection(&serverClient->requestQueueRegion);
	//���ö��еĽڵ���
	serverClient->setNumRequestNode(serverClient->requestQueue->getSizeRequestQueue());
	//���Һ�����к�����Ϊ�ͻ��˴����ĺ�������ͬ�Ľڵ㣬�����ڣ���ִ��if���
	if ((serverClient->tree)->BrowseNodeC((serverClient->tree), requestNode->getFuncName(), &findNode))
		serverClient->evaluateFuncForClient(serverClient, requestNode, findNode);
	//ɾ����������еĽڵ�
	if (requestNode)
		delete requestNode;
}
//��������߳�
void ServerClient::createResultThread(void* thisThread)
{
	//�������������ͻ����׽���ָ����մ����̵߳Ĳ���
	ServerClient* serverClient = (ServerClient*)thisThread;
	ResultNode* resultNode;
	ClientSocketNode* clientSocketNode;
	EnterCriticalSection(&serverClient->resultQueueRegion);
	resultNode = serverClient->resultQueue->popResultNode();
	LeaveCriticalSection(&serverClient->resultQueueRegion);
	//���ö��еĽڵ���
	serverClient->setNumResultNode(serverClient->resultQueue->getSizeResultQueue());
	//��������ظ��ͻ���
	unsigned int sizeReturnResult = resultNode->getSizeReturnResult();
	if (send(serverClient->clientSocketQueue->getFront()->getClientSocketNew()->Socket, (char*)&sizeReturnResult, sizeof(unsigned int), 0) == SOCKET_ERROR)
		ServerClient::ErrorHandling("The data size sent to the client failed!");
	if (send(serverClient->clientSocketQueue->getFront()->getClientSocketNew()->Socket, (char*)resultNode->getReturnResult(), sizeReturnResult, 0) == SOCKET_ERROR)
		ServerClient::ErrorHandling("The data sent to the client failed!");
	//ɾ��������нڵ�
	if (resultNode)
		delete resultNode;
	//�Կͻ����׽��ֽڵ����ɾ��
	clientSocketNode = serverClient->clientSocketQueue->popClientSocketNode();
	if (clientSocketNode)
		delete clientSocketNode;
}
//��ȡ���еĽڵ���
unsigned int ServerClient::getNumRequestNode()
{
	return this->numRequestNode;
}
unsigned int ServerClient::getNumResultNode()
{
	return this->numResultNode;
}
//���ö��еĽڵ���
void ServerClient::setNumRequestNode(unsigned int numRequestNode)
{
	this->numRequestNode = numRequestNode;
}
void ServerClient::setNumResultNode(unsigned int numResultNode)
{
	this->numResultNode = numResultNode;
}
void ServerClient::evaluateFuncForClient(ServerClient* serverClient, RequestNode* requestNode, RBNode* findNode)
{
	//�������
	HMODULE header;
	unsigned int funcStartPos, funcSize;
	//�ر�LIB�ļ�
	while ((serverClient->fileLIB).is_open())
		(serverClient->fileLIB).close();
	//��LIB�ļ�
	(serverClient->fileLIB).open(serverClient->nameLib, ios::app | ios::out | ios::in | ios::binary);
	while (!(serverClient->fileLIB).is_open())
		(serverClient->fileLIB).open(serverClient->nameLib, ios::app | ios::out | ios::in | ios::binary);
	//������ʱ�ļ�����
	serverClient->tempFile.open(serverClient->tempFileName, ios::app | ios::out | ios::in | ios::binary);
	//���������ʱ�ļ������ʱ�ļ���LIB�ļ����в���
	while (!serverClient->tempFile.is_open())
		serverClient->tempFile.open(serverClient->tempFileName, ios::app | ios::out | ios::in | ios::binary);
	//��ȡ������LIB���е���ʼλ�ÿɴ�С
	funcStartPos = findNode->getfuncStartPos();
	funcSize = findNode->getfuncSize();
	//����̬��д����ʱ�ļ�
	(serverClient->fileLIB).seekg(funcStartPos, ios::beg);
	while (1)
	{
		if (funcSize == 0)
			break;
		serverClient->tempFile.put((serverClient->fileLIB).get());
		funcSize--;
	}
	//�ر���ʱ�ļ�
	serverClient->tempFile.close();
	//���ض�̬��
	header = LoadLibrary(serverClient->tempFileName.c_str());
	if (header != NULL)
	{
		//���庯��ָ��
		typedef double (*funcPtr)(double, double);
		funcPtr func;
		func = (funcPtr)GetProcAddress(header, requestNode->getFuncName());
		if (func != NULL)
		{
			if (strcmp(requestNode->getFuncName(), "add") == 0 && requestNode->getParamNum() == 2)
			{
				if (requestNode->getParamType()[0] == 'f' && requestNode->getParamType()[1] == 'f')
				{
					ResultNode* resultNode;
					double returnResult = func(*(double*)requestNode->getParamContent()[0], *(double*)requestNode->getParamContent()[1]);
					resultNode = new ResultNode(sizeof(returnResult), &returnResult);
					EnterCriticalSection(&serverClient->resultQueueRegion);
					serverClient->resultQueue->pushResultNode(resultNode);
					LeaveCriticalSection(&serverClient->resultQueueRegion);
					//���ö��еĽڵ���
					serverClient->setNumResultNode(serverClient->resultQueue->getSizeResultQueue());
				}
			}
		}
		FreeLibrary(header);
	}
	DeleteFile(serverClient->tempFileName.c_str());
	//�ر�LIB�ļ�
	while ((serverClient->fileLIB).is_open())
		(serverClient->fileLIB).close();
}