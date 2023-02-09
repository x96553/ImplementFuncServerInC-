#include<iostream>
#include<stdarg.h>
#include<Windows.h>
#include<WinSock.h>
#include"SDK.h"
#pragma comment(lib,"ws2_32.lib")
using namespace std;
void* SDK::FuncSCallProxy1(char* FuncName, unsigned int paramNum, ...)
{
	//�����й��׽��ֵı���
	WSADATA wsaData;
	SOCKET clientSock;
	SOCKADDR_IN serverAddress;
	//�����йؿɱ�εı���
	int paramCounter;
	char paramType;
	unsigned long paramLength;
	void* paramContent;
	va_list paramPtr;
	//������շ��ؽ���йر���
	unsigned int returnSize = 1;
	void* returnResult;
	//��ʼ���׽��ֶ�̬��
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");
	clientSock = socket(PF_INET, SOCK_STREAM, 0);
	if (clientSock == INVALID_SOCKET)
		ErrorHandling("socket() error!");
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	serverAddress.sin_port = htons(atoi("789"));
	if (connect(clientSock, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");
	else
		puts("Connected............");
	//��paramPtrָ���ұߵ�һ���ɱ��
	va_start(paramPtr, paramNum);
	//���ͺ�����
	if (send(clientSock, FuncName, 255, 0) == SOCKET_ERROR)
		ErrorHandling("����FuncNameʧ�ܣ�");
	//���ͺ����Ĳ�������
	if (send(clientSock, (char*)&paramNum, sizeof(unsigned int), 0) == SOCKET_ERROR)
		ErrorHandling("����paramNumʧ�ܣ�");
	//���λ�ȡ�ɱ�ε�����
	for (paramCounter = 0; paramCounter < paramNum; paramCounter++)
	{
		paramType = va_arg(paramPtr, char);
		paramLength = va_arg(paramPtr, unsigned long);
		switch (paramType)
		{
		case 'd' | 'D':
			paramContent = va_arg(paramPtr, int*);
			break;
		case 'c' | 'C':
			paramContent = va_arg(paramPtr, char*);
			break;
		case 'f' | 'F':
			paramContent = va_arg(paramPtr, double*);
			break;
		case 's' | 'S':
			paramContent = va_arg(paramPtr, char*);
			break;
		default:
			paramContent = va_arg(paramPtr, int*);
			break;
		}
		//���Ͳ�������
		if (send(clientSock, &paramType, sizeof(char), 0) == SOCKET_ERROR)
			ErrorHandling("����paramTypeʧ�ܣ�");
		//���Ͳ�������
		if (send(clientSock, (char*)&paramLength, sizeof(unsigned long), 0) == SOCKET_ERROR)
			ErrorHandling("����paramLengthʧ�ܣ�");
		//���Ͳ�������
		if (send(clientSock, (char*)paramContent, paramLength, 0) == SOCKET_ERROR)
			ErrorHandling("����paramContentʧ�ܣ�");
	}
	//���շ��ؽ��
	if (recv(clientSock, (char*)&returnSize, sizeof(unsigned int), 0) == SOCKET_ERROR)
		ErrorHandling("���շ��ؽ����Сʧ�ܣ�");
	returnResult = new char[returnSize];
	if (recv(clientSock, (char*)returnResult, returnSize, 0) == SOCKET_ERROR)
		ErrorHandling("���շ��ؽ��ʧ�ܣ�");
	return returnResult;
	closesocket(clientSock);
	WSACleanup();
}
void SDK::FuncSCallProxy2(char* FuncName, unsigned long paramNum, char* paramContent, void* returnValue)
{

}
void SDK::FuncSCallProxy3(char* FuncName, char* paramNeed)
{

}
void SDK::ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
}