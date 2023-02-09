#ifndef _SERVERCLIENTLIB_H_
#define _SERVERCLIENTLIB_H_
#include<iostream>
#include<fstream>
#include<WinSock.h>
#include"rbTreeH.h"
using namespace std;
//�Խṹ����C�������
#ifdef __cplusplus 
extern "C" {
#endif
	//����������׽��ֽṹ��
	struct ServerSocket {
		WSADATA wsaData;
		SOCKET Socket;
		SOCKADDR_IN Address;
	};
	//����ͻ����׽��ֽṹ��
	struct ClientSocket {
		SOCKET Socket;
		SOCKADDR_IN Address;
		int AddressSize;
	};
#ifdef __cplusplus 
}
#endif
//����������нڵ�
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
//�����������
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
//���������нڵ�
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
//����������
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
//����ͻ��˷��ʷ�����ʱ�Ĵ洢�Ŀͻ����׽��ֽڵ�
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
//����ͻ��˷��ʷ�����ʱ�Ĵ洢�Ŀͻ����׽��ֶ���
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
//��������˻�ȡ�����е���
class serverGetLine;
//��������˷����ڿͻ��˵���
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
//��������˷�����LIB�����
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
//�������˻�ȡ�����е���
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
