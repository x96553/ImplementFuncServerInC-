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
	//析构红黑树
	(this->tree)->DestroyRBTree(&this->tree);
	//析构请求队列
	if (this->requestQueue)
		delete this->requestQueue;
	//析构结果队列
	if (this->resultQueue)
		delete this->resultQueue;
	//析构客户端套接字队列
	if (this->clientSocketQueue)
		delete this->clientSocketQueue;
}
void ServerClient::openLibServerClient()
{
	//定义变量，初始化
	int CycleFuncCallThreadID, CycleResultThreadID;
	//对LIB树和类数据成员初始化
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
	//初始化设置临界区的变量，请求队列
	InitializeCriticalSection(&this->requestQueueRegion);
	InitializeCriticalSection(&this->resultQueueRegion);
	//调用循环的函数运算线程和结果线程
	CycleFuncCallThreadID = _beginthread(ServerClient::CycleFuncCallThread, 0, this);
	CycleResultThreadID = _beginthread(ServerClient::CycleResultThread, 0, this);
	//初始化套接字动态库,绑定以及监听工作
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
	//循环接收客户端请求
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
	//定义变量
	char funcBlock[11] = { 0 };
	unsigned int fileCur, fileEnd;
	//关闭LIB文件
	while ((this->fileLIB).is_open())
		(this->fileLIB).close();
	//打开LIB文件
	(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
	while (!(this->fileLIB).is_open())
		(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
	//设置文件指针到文件初始位置
	(this->fileLIB).seekg(0, ios::end);
	fileEnd = (this->fileLIB).tellg();
	(this->fileLIB).seekg(0, ios::beg);
	fileCur = (this->fileLIB).tellg();
	//构建红黑树
	while (1)
	{
		//判断是否到达文件末尾
		if (fileCur == fileEnd)
			break;
		//从文件中读取十个字符到funcBlock数组中
		(this->fileLIB).read(funcBlock, strlen("Func Block"));
		//判断funcBlock数组中的字符串与“func Block”是否相等，若相等则执行if语句
		if (strncmp(funcBlock, "Func Block", strlen("Func Block")) == 0)
		{
			//定义变量
			unsigned int funcStartPos, funcSize, funcSizeI;
			char* funcName;
			RBNode* node;
			funcSize = 0;
			funcSizeI = 1;
			//获取函数所占字节大小并存入funcSize中
			while (1)
			{
				if (funcSizeI == unsigned int(10000000000))
					break;
				funcSize = funcSize + funcSizeI * fileLIB.get();
				funcSizeI = funcSizeI * 10;
			}
			//为存储函数名称分配内存，并从LIB文件中读入函数名称并存入funcName中
			funcName = new char[255];
			while (!funcName)
				funcName = new char[255];
			(this->fileLIB).read(funcName, 255);
			//获取函数在LIB文件中的起始位置，并存入funcStartPos中
			funcStartPos = (this->fileLIB).tellg();
			//创建红黑树节点
			node = new RBNode(funcName, funcStartPos, funcSize);
			while (!node)
				node = new RBNode(funcName, funcStartPos, funcSize);
			//将红黑树节点插入红黑树中
			(this->tree)->AddNodeC(&this->tree, node);
		}
		fileCur++;
		(this->fileLIB).seekg(fileCur, ios::beg);
	}
}
//循环创建函数调用线程
void ServerClient::CycleFuncCallThread(void* thisThread)
{
	//定义客户端套接字指针接收传进线程的参数
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
//循环创建结果线程
void ServerClient::CycleResultThread(void* thisThread)
{
	//定义客户端套接字指针接收传进线程的参数
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
//定义请求线程创建函数
void ServerClient::createReceiveThread(void* thisThread)
{
	//定义客户端套接字指针接收传进线程的参数
	ServerClient* serverClient = (ServerClient*)thisThread;
	struct ClientSocket* clientSocket = ((ServerClient*)thisThread)->clientSocketQueue->getFront()->getClientSocketNew();
	//定义存储函数名的数组,存储函数参数个数的整形变量,请求队列节点
	RequestNode* requestNode;
	char FuncName[255] = { 0 };
	unsigned int paramNum = 0;
	//服务器端接收函数名
	if (recv(clientSocket->Socket, FuncName, 255, 0) == SOCKET_ERROR)
		ServerClient::ErrorHandling("The receive function name failed");
	//服务器端接收函数的参数个数
	if (recv(clientSocket->Socket, (char*)&paramNum, sizeof(unsigned int), 0) == SOCKET_ERROR)
		ServerClient::ErrorHandling("Number of received parameters failed!");
	//对入队节点进行初始化
	requestNode = new RequestNode(paramNum, FuncName);
	requestNode->initializeRequestNode(clientSocket);
	//对初始化的节点入队
	EnterCriticalSection(&serverClient->requestQueueRegion);
	serverClient->requestQueue->pushRequestNode(requestNode);
	LeaveCriticalSection(&serverClient->requestQueueRegion);
	//设置队列的节点数
	serverClient->setNumRequestNode(serverClient->requestQueue->getSizeRequestQueue());
}
//定义函数调用线程创建函数
void ServerClient::createFuncCallThread(void* thisThread)
{
	//定义变量，定义客户端套接字指针接收传进线程的参数
	ServerClient* serverClient = (ServerClient*)thisThread;
	RBNode* findNode;
	RequestNode* requestNode;
	EnterCriticalSection(&serverClient->requestQueueRegion);
	requestNode = serverClient->requestQueue->popRequestNode();
	LeaveCriticalSection(&serverClient->requestQueueRegion);
	//设置队列的节点数
	serverClient->setNumRequestNode(serverClient->requestQueue->getSizeRequestQueue());
	//查找红黑树中函数名为客户端传来的函数名相同的节点，若存在，则执行if语句
	if ((serverClient->tree)->BrowseNodeC((serverClient->tree), requestNode->getFuncName(), &findNode))
		serverClient->evaluateFuncForClient(serverClient, requestNode, findNode);
	//删除请求队列中的节点
	if (requestNode)
		delete requestNode;
}
//创建结果线程
void ServerClient::createResultThread(void* thisThread)
{
	//定义变量，定义客户端套接字指针接收传进线程的参数
	ServerClient* serverClient = (ServerClient*)thisThread;
	ResultNode* resultNode;
	ClientSocketNode* clientSocketNode;
	EnterCriticalSection(&serverClient->resultQueueRegion);
	resultNode = serverClient->resultQueue->popResultNode();
	LeaveCriticalSection(&serverClient->resultQueueRegion);
	//设置队列的节点数
	serverClient->setNumResultNode(serverClient->resultQueue->getSizeResultQueue());
	//将结果返回给客户端
	unsigned int sizeReturnResult = resultNode->getSizeReturnResult();
	if (send(serverClient->clientSocketQueue->getFront()->getClientSocketNew()->Socket, (char*)&sizeReturnResult, sizeof(unsigned int), 0) == SOCKET_ERROR)
		ServerClient::ErrorHandling("The data size sent to the client failed!");
	if (send(serverClient->clientSocketQueue->getFront()->getClientSocketNew()->Socket, (char*)resultNode->getReturnResult(), sizeReturnResult, 0) == SOCKET_ERROR)
		ServerClient::ErrorHandling("The data sent to the client failed!");
	//删除结果队列节点
	if (resultNode)
		delete resultNode;
	//对客户端套接字节点进行删除
	clientSocketNode = serverClient->clientSocketQueue->popClientSocketNode();
	if (clientSocketNode)
		delete clientSocketNode;
}
//获取队列的节点数
unsigned int ServerClient::getNumRequestNode()
{
	return this->numRequestNode;
}
unsigned int ServerClient::getNumResultNode()
{
	return this->numResultNode;
}
//设置队列的节点数
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
	//定义变量
	HMODULE header;
	unsigned int funcStartPos, funcSize;
	//关闭LIB文件
	while ((serverClient->fileLIB).is_open())
		(serverClient->fileLIB).close();
	//打开LIB文件
	(serverClient->fileLIB).open(serverClient->nameLib, ios::app | ios::out | ios::in | ios::binary);
	while (!(serverClient->fileLIB).is_open())
		(serverClient->fileLIB).open(serverClient->nameLib, ios::app | ios::out | ios::in | ios::binary);
	//创建临时文件并打开
	serverClient->tempFile.open(serverClient->tempFileName, ios::app | ios::out | ios::in | ios::binary);
	//如果打开了临时文件则对临时文件和LIB文件进行操作
	while (!serverClient->tempFile.is_open())
		serverClient->tempFile.open(serverClient->tempFileName, ios::app | ios::out | ios::in | ios::binary);
	//获取函数在LIB库中的起始位置可大小
	funcStartPos = findNode->getfuncStartPos();
	funcSize = findNode->getfuncSize();
	//将动态库写入临时文件
	(serverClient->fileLIB).seekg(funcStartPos, ios::beg);
	while (1)
	{
		if (funcSize == 0)
			break;
		serverClient->tempFile.put((serverClient->fileLIB).get());
		funcSize--;
	}
	//关闭临时文件
	serverClient->tempFile.close();
	//加载动态库
	header = LoadLibrary(serverClient->tempFileName.c_str());
	if (header != NULL)
	{
		//定义函数指针
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
					//设置队列的节点数
					serverClient->setNumResultNode(serverClient->resultQueue->getSizeResultQueue());
				}
			}
		}
		FreeLibrary(header);
	}
	DeleteFile(serverClient->tempFileName.c_str());
	//关闭LIB文件
	while ((serverClient->fileLIB).is_open())
		(serverClient->fileLIB).close();
}