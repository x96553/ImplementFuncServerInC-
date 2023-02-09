#ifndef _SERVERCLIENTLIB_H_
#define _SERVERCLIENTLIB_H_
#include<iostream>
#include<fstream>
#include<WinSock.h>
#include"rbTreeH.h"
using namespace std;
//对结构体以C代码编译
#ifdef __cplusplus 
extern "C" {
#endif
	//定义服务器套接字结构体
	struct ServerSocket {
		WSADATA wsaData;
		SOCKET Socket;
		SOCKADDR_IN Address;
	};
	//定义客户端套接字结构体
	struct ClientSocket {
		SOCKET Socket;
		SOCKADDR_IN Address;
		int AddressSize;
	};
#ifdef __cplusplus 
}
#endif
//定义请求队列节点
class RequestNode {
public:
	RequestNode(unsigned int paramNum, char* FuncName);
	~RequestNode();
	void initializeRequestNode(struct ClientSocket* clientSocket);
	char* getFuncName();
	unsigned int getParamNum();
	char* getParamType();
	unsigned long* getParamLength();
	void** getParamContent();
	RequestNode* getNext();
	void setNext(RequestNode* next);
private:
	char* FuncName;
	unsigned int paramNum;
	char* paramType;
	unsigned long* paramLength;
	void** paramContent;
	RequestNode* next;
};
//定义请求队列
class RequestQueue {
public:
	RequestQueue();
	~RequestQueue();
	void pushRequestNode(RequestNode* requestNode);
	RequestNode* popRequestNode();
	unsigned int getSizeRequestQueue();
private:
	RequestNode* front;
	RequestNode* rear;
	unsigned int sizeRequestQueue;
};
//定义结果队列节点
class ResultNode {
public:
	ResultNode(unsigned int sizeReturnResult, void* returnResult);
	~ResultNode();
	void* getReturnResult();
	unsigned int getSizeReturnResult();
	ResultNode* getNext();
	void setNext(ResultNode* next);
private:
	unsigned int sizeReturnResult;
	void* returnResult;
	ResultNode* next;
};
//定义结果队列
class ResultQueue {
public:
	ResultQueue();
	~ResultQueue();
	void pushResultNode(ResultNode* resultNode);
	ResultNode* popResultNode();
	unsigned int getSizeResultQueue();
private:
	ResultNode* front;
	ResultNode* rear;
	unsigned int sizeResultQueue;
};
//定义客户端访问服务器时的存储的客户端套接字节点
class ClientSocketNode {
public:
	ClientSocketNode(ClientSocket clientSocket);
	~ClientSocketNode();
	struct ClientSocket* getClientSocketNew();
	ClientSocketNode* getNext();
	void setNext(ClientSocketNode* next);
private:
	struct ClientSocket* clientSocketNew;
	ClientSocketNode* next;
};
//定义客户端访问服务器时的存储的客户端套接字队列
class ClientSocketQueue {
public:
	ClientSocketQueue();
	~ClientSocketQueue();
	void pushClientSocketNode(ClientSocketNode* clientSocketNode);
	ClientSocketNode* popClientSocketNode();
	ClientSocketNode* getFront();
	unsigned int getSizeClientSocketQueue();
private:
	ClientSocketNode* front;
	ClientSocketNode* rear;
	unsigned int sizeClientSocketQueue;
};
//声明服务端获取命令行的类
class serverGetLine;
//声明服务端服务于客户端的类
class ServerClient {
public:
	ServerClient();
	~ServerClient();
	void openLibServerClient();
	void openLibCreateTree();
	void static  CycleFuncCallThread(void* thisThread);
	void static  CycleResultThread(void* thisThread);
	void static  createReceiveThread(void* thisThread);
	void static  createFuncCallThread(void* thisThread);
	void static  createResultThread(void* thisThread);
	unsigned int getNumRequestNode();
	unsigned int getNumResultNode();
	void setNumRequestNode(unsigned int numRequestNode);
	void setNumResultNode(unsigned int numResultNode);
	void evaluateFuncForClient(ServerClient* serverClient, RequestNode* requestNode, RBNode* findNode);
	void static ErrorHandling(const char* message);
private:
	CRITICAL_SECTION requestQueueRegion;
	CRITICAL_SECTION resultQueueRegion;
	RequestQueue* requestQueue;
	ResultQueue* resultQueue;
	ClientSocketQueue* clientSocketQueue;
	struct ServerSocket serverSocket;
	struct ClientSocket clientSocket;
	RBTree* tree;
	RBNode* nil;
	char* funcNameNil;
	fstream fileLIB, tempFile;
	string nameLib, tempFileName;
	unsigned int numRequestNode, numResultNode;
};
//声明服务端服务于LIB库的类
class ServerLib {
public:
	ServerLib();
	~ServerLib();
	int openLibServerClient();
	void getLine();
	int addServerLib(std::string::size_type startSpace, std::string::size_type endSpace);
	int modifyServerLib(std::string::size_type startSpace, std::string::size_type endSpace);
	int browseServerLib(std::string::size_type startSpace, std::string::size_type endSpace);
	int deleteServerLib(std::string::size_type startSpace, std::string::size_type endSpace);
private:
	RBTree* tree;
	RBNode* nil;
	char* funcNameNil;
	fstream fileLIB;
	string nameLib;
	string Line;
};
//定义服务端获取命令行的类
class serverGetLine {
public:
	serverGetLine();
	int getLine();
private:
	string Line;
	std::string::size_type startSpace;
	std::string::size_type endSpace;
};
#endif // !_SERVERCLIENTLIB_H_
