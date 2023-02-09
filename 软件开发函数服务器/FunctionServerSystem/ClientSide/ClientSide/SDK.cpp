#include<iostream>
#include<stdarg.h>
#include<Windows.h>
#include<WinSock.h>
#include"SDK.h"
#pragma comment(lib,"ws2_32.lib")
using namespace std;
void* SDK::FuncSCallProxy1(char* FuncName, unsigned int paramNum, ...)
{
	//定义有关套接字的变量
	WSADATA wsaData;
	SOCKET clientSock;
	SOCKADDR_IN serverAddress;
	//定义有关可变参的变量
	int paramCounter;
	char paramType;
	unsigned long paramLength;
	void* paramContent;
	va_list paramPtr;
	//定义接收返回结果有关变量
	unsigned int returnSize = 1;
	void* returnResult;
	//初始化套接字动态库
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
	//将paramPtr指向右边第一个可变参
	va_start(paramPtr, paramNum);
	//发送函数名
	if (send(clientSock, FuncName, 255, 0) == SOCKET_ERROR)
		ErrorHandling("发送FuncName失败！");
	//发送函数的参数个数
	if (send(clientSock, (char*)&paramNum, sizeof(unsigned int), 0) == SOCKET_ERROR)
		ErrorHandling("发送paramNum失败！");
	//依次获取可变参的内容
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
		//发送参数类型
		if (send(clientSock, &paramType, sizeof(char), 0) == SOCKET_ERROR)
			ErrorHandling("发送paramType失败！");
		//发送参数长度
		if (send(clientSock, (char*)&paramLength, sizeof(unsigned long), 0) == SOCKET_ERROR)
			ErrorHandling("发送paramLength失败！");
		//发送参数内容
		if (send(clientSock, (char*)paramContent, paramLength, 0) == SOCKET_ERROR)
			ErrorHandling("发送paramContent失败！");
	}
	//接收返回结果
	if (recv(clientSock, (char*)&returnSize, sizeof(unsigned int), 0) == SOCKET_ERROR)
		ErrorHandling("接收返回结果大小失败！");
	returnResult = new char[returnSize];
	if (recv(clientSock, (char*)returnResult, returnSize, 0) == SOCKET_ERROR)
		ErrorHandling("接收返回结果失败！");
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