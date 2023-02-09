#include <io.h>
#include<string>
#include"serverClientLib.h"
ServerLib::ServerLib()
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
}
ServerLib::~ServerLib()
{
	//析构红黑树
	(this->tree)->DestroyRBTree(&this->tree);
}
int ServerLib::openLibServerClient()
{
	(this->fileLIB).open(nameLib, ios::_Nocreate);
	//判断LIB文件是否打开，若存在执行if语句
	if ((this->fileLIB).is_open())
	{
		//定义变量
		char funcBlock[11] = { 0 };
		unsigned int fileCur, fileEnd;
		//关闭LIB文件
		while ((this->fileLIB).is_open())
			(this->fileLIB).close();
		//打开LIB文件
		(this->fileLIB).open(nameLib, ios::app | ios::out | ios::in | ios::binary);
		while (!(this->fileLIB).is_open())
			(this->fileLIB).open(nameLib, ios::app | ios::out | ios::in | ios::binary);
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
				if (!funcName)
					return 0;
				(this->fileLIB).read(funcName, 255);
				//获取函数在LIB文件中的起始位置，并存入funcStartPos中
				funcStartPos = (this->fileLIB).tellg();
				//创建红黑树节点
				node = new RBNode(funcName, funcStartPos, funcSize);
				if (!node)
					return 0;
				//将红黑树节点插入红黑树中
				(this->tree)->AddNodeC(&this->tree, node);
			}
			fileCur++;
			(this->fileLIB).seekg(fileCur, ios::beg);
		}
		//关闭LIB文件
		while ((this->fileLIB).is_open())
			(this->fileLIB).close();
		ServerLib::getLine();
	}
	else
	{
		//打开LIB文件
		(this->fileLIB).open(nameLib, ios::app | ios::out | ios::in | ios::binary);
		while (!(this->fileLIB).is_open())
			(this->fileLIB).open(nameLib, ios::app | ios::out | ios::in | ios::binary);
		fileLIB.seekp(0, ios::beg);
		fileLIB.write("Function Server Libs v1.0", strlen("Function Server Libs v1.0"));
		fileLIB.put(0x0D);
		fileLIB.put(0x0A);
		fileLIB.put(0x1A);
		//关闭LIB文件
		while ((this->fileLIB).is_open())
			(this->fileLIB).close();
		ServerLib::getLine();
	}
}
void ServerLib::getLine()
{
	std::string::size_type startSpace = std::string::npos;
	std::string::size_type endSpace = std::string::npos;
	while (1)
	{
		getline(cin, this->Line);
		startSpace = 0;
		while (this->Line[startSpace] == ' ')
			startSpace++;
		endSpace = startSpace;
		while (this->Line[endSpace] != ' ' && this->Line[endSpace] != '\0')
			endSpace++;
		if (string("add").compare(this->Line.substr(startSpace, endSpace - startSpace)) == 0)
			ServerLib::addServerLib(startSpace, endSpace);
		else if (string("modify").compare(this->Line.substr(startSpace, endSpace - startSpace)) == 0)
			ServerLib::modifyServerLib(startSpace, endSpace);
		else if (string("browse").compare(this->Line.substr(startSpace, endSpace - startSpace)) == 0)
			ServerLib::browseServerLib(startSpace, endSpace);
		else if (string("delete").compare(this->Line.substr(startSpace, endSpace - startSpace)) == 0)
			ServerLib::deleteServerLib(startSpace, endSpace);
		else if (string("exit").compare(this->Line.substr(startSpace, endSpace - startSpace)) == 0)
			break;
	}
}
int ServerLib::addServerLib(std::string::size_type startSpace, std::string::size_type endSpace)
{
	ifstream fileDLL;
	//关闭LIB文件
	while ((this->fileLIB).is_open())
		(this->fileLIB).close();
	//打开LIB文件
	(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
	while (!(this->fileLIB).is_open())
		(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
	while (1)
	{
		//定义变量
		RBNode* findNode;
		string funcAddName, funcAddNameAndPath;
		std::string::size_type funcStart = std::string::npos;
		std::string::size_type funcEnd = std::string::npos;
		//若遇到字符串结束符，则退出循环
		if ((this->Line)[endSpace] == '\0')
			return 0;
		//依次获取输入到控制台的函数名称
		startSpace = endSpace;
		while ((this->Line)[startSpace] == ' ')
			startSpace++;
		endSpace = startSpace;
		while ((this->Line)[endSpace] != ' ' && (this->Line)[endSpace] != '\0')
			endSpace++;
		funcAddNameAndPath = (this->Line).substr(startSpace, endSpace - startSpace);//函数名存入funcCmdBuf中
		//浏览函数库看是否存在需要增加的节点
		funcStart = funcAddNameAndPath.find_last_of('\\');
		funcEnd = funcAddNameAndPath.find_last_of('.');
		funcAddName = funcAddNameAndPath.substr(funcStart + 1, funcEnd - funcStart - 1);
		if ((this->tree)->BrowseNodeC((this->tree), funcAddName, &findNode))
			return 0;
		else
		{
			fileDLL.open(funcAddNameAndPath, ios::in | ios::binary);
			if (!fileDLL.is_open())
				return 0;
			else
			{
				//定义变量
				unsigned int startDLL, endDLL, sizeDLL, sizeDiv, sizeBytes, funcPackegeLen;
				std::string::size_type funcStart = std::string::npos;
				std::string::size_type funcEnd = std::string::npos;
				string funcPackegeName;
				//定义红黑树节点变量
				char* funcName_Node = new char[255];
				unsigned int funcStartPos_Node;
				RBNode* node;
				(this->fileLIB).seekp(0, ios::end);
				(this->fileLIB).write("Func Block", strlen("Func Block"));
				//计算DLL文件大小
				fileDLL.seekg(0, ios::beg);
				startDLL = fileDLL.tellg();
				fileDLL.seekg(0, ios::end);
				endDLL = fileDLL.tellg();
				sizeDLL = endDLL - startDLL;//函数大小存入sizeDLL中
				//将DLL的大小以十进制写入LIB文件，占四个字节
				sizeBytes = 10;
				sizeDiv = sizeDLL;
				while (1)
				{
					if (sizeBytes == 0)
						break;
					if (sizeDiv == 0)
						(this->fileLIB).put(0x00);
					else
					{
						(this->fileLIB).put(sizeDiv % 10);
						sizeDiv = sizeDiv / 10;
					}
					sizeBytes--;
				}
				//将DLL的文件名或函数名存入LIB中，提供的所有函数在LIB中遵循包机制
				funcPackegeName = this->nameLib + "." + funcAddName;//LIB包名+‘.’+函数名
				(this->fileLIB).write(funcPackegeName.c_str(), strlen(funcPackegeName.c_str()));
				funcPackegeLen = 255 - strlen(funcPackegeName.c_str());
				while (1)
				{
					if (funcPackegeLen == 0)
						break;
					(this->fileLIB).put(0x00);
					funcPackegeLen--;
				}
				//获取当前函数在LIB库中的起始地址和函数名
				strcpy_s(funcName_Node, 255, funcPackegeName.c_str());
				funcStartPos_Node = (this->fileLIB).tellp();
				//将已编译好的，成为DLL动态库的文件的内容写入LIB库中
				fileDLL.seekg(0, ios::beg);
				while (1)
				{
					if (sizeDLL == 0)
						break;
					(this->fileLIB).put(fileDLL.get());
					sizeDLL--;
				}
				//关闭DLL文件
				fileDLL.close();
				node = new RBNode(funcName_Node, funcStartPos_Node, sizeDLL);
				(this->tree)->AddNodeC(&this->tree, node);
			}
			//关闭LIB文件
			while ((this->fileLIB).is_open())
				(this->fileLIB).close();
			//打开LIB文件
			(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
			while (!(this->fileLIB).is_open())
				(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
		}
	}
}
int ServerLib::modifyServerLib(std::string::size_type startSpace, std::string::size_type endSpace)
{
	ifstream fileDLL;
	//关闭LIB文件
	while ((this->fileLIB).is_open())
		(this->fileLIB).close();
	//打开LIB文件
	(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
	while (!(this->fileLIB).is_open())
		(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
	while (1)
	{
		//定义变量
		RBNode* findNode;
		string funcNameModify;
		string funcNameModified;
		//若遇到字符串结束符，则退出循环
		if ((this->Line)[endSpace] == '\0')
			return 0;
		//依次获取输入到控制台的函数名称
		startSpace = endSpace;
		while ((this->Line)[startSpace] == ' ')
			startSpace++;
		endSpace = startSpace;
		while ((this->Line)[endSpace] != ' ' && (this->Line)[endSpace] != '\0')
			endSpace++;
		funcNameModified = (this->Line).substr(startSpace, endSpace - startSpace);
		//浏览函数库看是否存在需要产出的节点
		if (!(this->tree)->BrowseNodeC(this->tree, funcNameModified, &findNode))
			return 0;
		else
		{
			//若遇到字符串结束符，则退出循环
			if ((this->Line)[endSpace] == '\0')
				return 0;
			//依次获取输入到控制台的函数名称
			startSpace = endSpace;
			while ((this->Line)[startSpace] == ' ')
				startSpace++;
			endSpace = startSpace;
			while ((this->Line)[endSpace] != ' ' && (this->Line)[endSpace] != '\0')
				endSpace++;
			funcNameModify = (this->Line).substr(startSpace, endSpace - startSpace);
			fileDLL.open(funcNameModify, ios::in | ios::binary);
			if (!fileDLL.is_open())
				return 0;
			else
			{
				//定义变量
				char tempFileName[] = "XXXXXX";
				errno_t errorMakeTempFile;
				fstream tempFile;
				unsigned int fileCur, fileEnd;
				char funcBlock[11] = { 0 };
				unsigned int sizeTempFile, TempFileCur;
				//创建临时文件并打开
				errorMakeTempFile = _mktemp_s(tempFileName, strlen(tempFileName) + 1);
				while (errorMakeTempFile != 0)
					errorMakeTempFile = _mktemp_s(tempFileName, strlen(tempFileName) + 1);
				if (errorMakeTempFile == 0)
					tempFile.open(tempFileName, ios::app | ios::out | ios::in | ios::binary);
				while (!tempFile.is_open())
					tempFile.open(tempFileName, ios::app | ios::out | ios::in | ios::binary);
				//设置tempFile临时文件指针的初始位置
				tempFile.seekp(0, ios::beg);
				tempFile.write("Function Server Libs v1.0", strlen("Function Server Libs v1.0"));
				tempFile.put(0x0D);
				tempFile.put(0x0A);
				tempFile.put(0x1A);
				//设置文件指针到文件初始位置
				(this->fileLIB).seekg(0, ios::end);
				fileEnd = (this->fileLIB).tellg();
				(this->fileLIB).seekg(0, ios::beg);
				fileCur = (this->fileLIB).tellg();
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
						std::string::size_type funcStart = std::string::npos;
						std::string::size_type funcEnd = std::string::npos;
						unsigned int funcSizeOfModity, funcSizeI;
						string funcInLibName;
						char findFuncOfLIB[255] = { 0 };
						funcSizeOfModity = 0;
						funcSizeI = 1;
						//获取函数所占字节大小并存入funcSize中
						while (1)
						{
							if (funcSizeI == unsigned int(10000000000))
								break;
							funcSizeOfModity = funcSizeOfModity + funcSizeI * (this->fileLIB).get();
							funcSizeI = funcSizeI * 10;
						}
						//从LIB中读去函数名
						(this->fileLIB).read(findFuncOfLIB, 255);
						funcStart = (string(findFuncOfLIB)).find_last_of('.');
						funcInLibName = (string(findFuncOfLIB)).substr(funcStart + 1, strlen(findFuncOfLIB) - funcStart - 1);
						//将函数块的起始标识写入临时文件中
						tempFile.write(funcBlock, strlen("Func Block"));
						//判断被替换的函数名与替换的函数名是否相等
						if (strcmp(funcInLibName.c_str(), funcNameModified.c_str()) == 0)
						{
							//定义变量
							string ModityName;
							unsigned int startDLL, endDLL, sizeDLL, sizeDiv, sizeBytes;
							//计算DLL文件大小
							fileDLL.seekg(0, ios::beg);
							startDLL = fileDLL.tellg();
							fileDLL.seekg(0, ios::end);
							endDLL = fileDLL.tellg();
							sizeDLL = endDLL - startDLL;//函数大小存入sizeDLL中
							//将DLL的大小以十进制写入LIB文件，占四个字节
							sizeBytes = 10;
							sizeDiv = sizeDLL;
							while (1)
							{
								if (sizeBytes == 0)
									break;
								if (sizeDiv == 0)
									tempFile.put(0x00);
								else
								{
									tempFile.put(sizeDiv % 10);
									sizeDiv = sizeDiv / 10;
								}
								sizeBytes--;
							}
							//获取替换的函数名
							funcStart = funcNameModify.find_last_of('\\');
							funcEnd = funcNameModify.find_last_of('.');
							ModityName = funcNameModify.substr(funcStart + 1, funcEnd - funcStart - 1);
							if (strcmp(funcInLibName.c_str(), ModityName.c_str()) == 0)
							{
								(this->tree)->ModifyNodeC(&this->tree, findNode, sizeDLL);
								//将函数名复制到临时文件中
								tempFile.write(findFuncOfLIB, 255);
								//将函数块的内容复制到临时文件中
								fileDLL.seekg(0, ios::beg);
								while (1)
								{
									if (sizeDLL == 0)
										break;
									tempFile.put(fileDLL.get());
									sizeDLL--;
								}
							}
							else
							{
								//定义变量
								unsigned int funcPackegeLen;
								string funcPackegeName, funcToModifyName;
								//定义红黑树节点变量
								char* funcName_Node = new char[255];
								unsigned int funcStartPos_Node;
								RBNode* node;
								//将DLL的文件名或函数名存入LIB中，提供的所有函数在LIB中遵循包机制
								funcStart = funcNameModify.find_last_of('\\');
								funcEnd = funcNameModify.find_last_of('.');
								funcToModifyName = funcNameModify.substr(funcStart + 1, funcEnd - funcStart - 1);
								funcPackegeName = this->nameLib + "." + funcToModifyName;//LIB包名+‘.’+函数名
								tempFile.write(funcPackegeName.c_str(), strlen(funcPackegeName.c_str()));
								funcPackegeLen = 255 - strlen(funcPackegeName.c_str());
								while (1)
								{
									if (funcPackegeLen == 0)
										break;
									tempFile.put(0x00);
									funcPackegeLen--;
								}
								//获取当前函数在LIB库中的起始地址和函数名
								strcpy_s(funcName_Node, 255, funcPackegeName.c_str());
								funcStartPos_Node = tempFile.tellp();
								(this->tree)->DeleteNodeC(&this->tree, &findNode);
								node = new RBNode(funcName_Node, funcStartPos_Node, sizeDLL);
								(this->tree)->AddNodeC(&this->tree, node);
								//将函数块的内容复制到临时文件中
								fileDLL.seekg(0, ios::beg);
								while (1)
								{
									if (sizeDLL == 0)
										break;
									tempFile.put(fileDLL.get());
									sizeDLL--;
								}
							}
						}
						else
						{
							//定义变量
							int sizeBytes, sizeFuncBlock;
							sizeBytes = 10;
							sizeFuncBlock = funcSizeOfModity;
							while (1)
							{
								if (sizeBytes == 0)
									break;
								if (funcSizeOfModity == 0)
									tempFile.put(0x00);
								else
								{
									tempFile.put(funcSizeOfModity % 10);
									funcSizeOfModity = funcSizeOfModity / 10;
								}
								sizeBytes--;
							}
							//将函数名复制到临时文件中
							tempFile.write(findFuncOfLIB, 255);
							//将函数块的内容复制到临时文件中
							while (1)
							{
								if (sizeFuncBlock == 0)
									break;
								tempFile.put(fileLIB.get());
								sizeFuncBlock--;
							}
						}
					}
					fileCur++;
					(this->fileLIB).seekg(fileCur, ios::beg);
				}
				delete findNode;
				//关闭LIB文件
				while ((this->fileLIB).is_open())
					(this->fileLIB).close();
				//将临时文件中的全部内容复制到原来的LIB文件中
				(this->fileLIB).open(this->nameLib, ios::trunc | ios::out | ios::in | ios::binary);
				while (!(this->fileLIB).is_open())
					(this->fileLIB).open(this->nameLib, ios::trunc | ios::out | ios::in | ios::binary);
				tempFile.seekg(0, ios::end);
				sizeTempFile = tempFile.tellg();
				//设置文件指针到文件初始位置
				tempFile.seekg(0, ios::beg);
				TempFileCur = tempFile.tellg();
				while (1)
				{
					//判断是否到达文件末尾
					if (TempFileCur == sizeTempFile)
						break;
					(this->fileLIB).put(tempFile.get());
					TempFileCur++;
					tempFile.seekg(TempFileCur, ios::beg);
				}
				//关闭LIB文件
				tempFile.close();
				while (_unlink(tempFileName) == -1);
				//关闭DLL文件
				fileDLL.close();
			}
			//关闭LIB文件
			while ((this->fileLIB).is_open())
				(this->fileLIB).close();
			//打开LIB文件
			(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
			while (!(this->fileLIB).is_open())
				(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
		}
	}
}
int ServerLib::browseServerLib(std::string::size_type startSpace, std::string::size_type endSpace)
{
	//定义变量
	char funcBlock[11] = { 0 };
	unsigned int fileCur, fileEnd;
	RBNode* findNode;
	string funcNodeName, funcBrowseName;
	std::string::size_type funcStart = std::string::npos;
	std::string::size_type funcEnd = std::string::npos;
	//关闭LIB文件
	while ((this->fileLIB).is_open())
		(this->fileLIB).close();
	//打开LIB文件
	(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
	while (!(this->fileLIB).is_open())
		(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
	while (1)
	{
		//若遇到字符串结束符，则退出循环
		if ((this->Line)[endSpace] == '\0')
			return 0;
		//依次获取输入到控制台的函数名称
		startSpace = endSpace;
		while ((this->Line)[startSpace] == ' ')
			startSpace++;
		endSpace = startSpace;
		while ((this->Line)[endSpace] != ' ' && (this->Line)[endSpace] != '\0')
			endSpace++;
		funcBrowseName = (this->Line).substr(startSpace, endSpace - startSpace);
		//浏览函数库
		funcStart = funcBrowseName.find_last_of('\\');
		funcEnd = funcBrowseName.find_last_of('.');
		funcNodeName = funcBrowseName.substr(funcStart + 1, funcEnd - funcStart - 1);
		if (!(this->tree)->BrowseNodeC(this->tree, funcNodeName, &findNode))
			return 0;
		else
		{
			//设置文件指针到文件初始位置
			(this->fileLIB).seekg(0, ios::end);
			fileEnd = (this->fileLIB).tellg();
			(this->fileLIB).seekg(0, ios::beg);
			fileCur = (this->fileLIB).tellg();
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
					unsigned int funcSizeOfBrowse, funcSizeI;
					char findFuncOfLIB[255] = { 0 };
					funcSizeOfBrowse = 0;
					funcSizeI = 1;
					//获取函数所占字节大小并存入funcSize中
					while (1)
					{
						if (funcSizeI == unsigned int(10000000000))
							break;
						funcSizeOfBrowse = funcSizeOfBrowse + funcSizeI * fileLIB.get();
						funcSizeI = funcSizeI * 10;
					}
					(this->fileLIB).read(findFuncOfLIB, 255);
					if (strncmp(findFuncOfLIB, findNode->getFuncName(), 255) == 0)
					{
						unsigned int funcSizeC;
						funcSizeC = 0;
						while (1)
						{
							if (funcSizeC == funcSizeOfBrowse)
								break;
							printf("%c", (this->fileLIB).get());
							funcSizeC++;
						}
					}
				}
				fileCur++;
				(this->fileLIB).seekg(fileCur, ios::beg);
			}
			//关闭LIB文件
			while ((this->fileLIB).is_open())
				(this->fileLIB).close();
			//打开LIB文件
			(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
			while (!(this->fileLIB).is_open())
				(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
		}
	}
}
int ServerLib::deleteServerLib(std::string::size_type startSpace, std::string::size_type endSpace)
{
	//定义变量
	char funcBlock[11] = { 0 };
	unsigned int fileCur, fileEnd;
	RBNode* findNode;
	string funcDeleteName;
	//关闭LIB文件
	while ((this->fileLIB).is_open())
		(this->fileLIB).close();
	//打开LIB文件
	(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
	while (!(this->fileLIB).is_open())
		(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
	while (1)
	{
		//若遇到字符串结束符，则退出循环
		if ((this->Line)[endSpace] == '\0')
			return 0;
		//依次获取输入到控制台的函数名称
		startSpace = endSpace;
		while ((this->Line)[startSpace] == ' ')
			startSpace++;
		endSpace = startSpace;
		while ((this->Line)[endSpace] != ' ' && (this->Line)[endSpace] != '\0')
			endSpace++;
		funcDeleteName = (this->Line).substr(startSpace, endSpace - startSpace);
		//浏览函数库看是否存在需要产出的节点
		if (!(this->tree)->BrowseNodeC(this->tree, funcDeleteName, &findNode))
			return 0;
		else
		{
			//定义临时文件所需的变量
			char tempFileName[] = "XXXXXX";
			errno_t errorMakeTempFile;
			fstream tempFile;
			unsigned int sizeTempFile, TempFileCur;
			//创建临时文件并打开
			errorMakeTempFile = _mktemp_s(tempFileName, strlen(tempFileName) + 1);
			while (errorMakeTempFile != 0)
				errorMakeTempFile = _mktemp_s(tempFileName, strlen(tempFileName) + 1);
			if (errorMakeTempFile == 0)
				tempFile.open(tempFileName, ios::app | ios::out | ios::in | ios::binary);
			while (!tempFile.is_open())
				tempFile.open(tempFileName, ios::app | ios::out | ios::in | ios::binary);
			//设置tempFile临时文件指针的初始位置
			tempFile.seekp(0, ios::beg);
			tempFile.write("Function Server Libs v1.0", strlen("Function Server Libs v1.0"));
			tempFile.put(0x0D);
			tempFile.put(0x0A);
			tempFile.put(0x1A);
			//删除函数库
			(this->tree)->DeleteNodeC(&this->tree, &findNode);
			//设置文件指针到文件初始位置
			(this->fileLIB).seekg(0, ios::end);
			fileEnd = (this->fileLIB).tellg();
			(this->fileLIB).seekg(0, ios::beg);
			fileCur = (this->fileLIB).tellg();
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
					unsigned int funcSizeOfDelete, funcSizeI;
					char findFuncOfLIB[255] = { 0 };
					funcSizeOfDelete = 0;
					funcSizeI = 1;
					//获取函数所占字节大小并存入funcSize中
					while (1)
					{
						if (funcSizeI == unsigned int(10000000000))
							break;
						funcSizeOfDelete = funcSizeOfDelete + funcSizeI * (this->fileLIB).get();
						funcSizeI = funcSizeI * 10;
					}
					(this->fileLIB).read(findFuncOfLIB, 255);
					//如果被删除的函数块与被删节点的函数名相同，则跳过需要被复制到临时文件的函数块
					//否则将LIB文件中有关被删节点的函数的内容复制到临时文件中
					if (strncmp(findFuncOfLIB, findNode->getFuncName(), 255) == 0)
						(this->fileLIB).seekg(funcSizeOfDelete, ios::cur);
					else
					{
						//定义变量
						int sizeBytes, sizeFuncBlock;
						//将函数块的起始标识写入临时文件中
						tempFile.write(funcBlock, strlen("Func Block"));
						//将函数块的大小以十进制写入临时文件中，占四个字节
						sizeBytes = 10;
						sizeFuncBlock = funcSizeOfDelete;
						while (1)
						{
							if (sizeBytes == 0)
								break;
							if (funcSizeOfDelete == 0)
								tempFile.put(0x00);
							else
							{
								tempFile.put(funcSizeOfDelete % 10);
								funcSizeOfDelete = funcSizeOfDelete / 10;
							}
							sizeBytes--;
						}
						//将函数名复制到临时文件中
						tempFile.write(findFuncOfLIB, 255);
						//将函数块的内容复制到临时文件中
						while (1)
						{
							if (sizeFuncBlock == 0)
								break;
							tempFile.put((this->fileLIB).get());
							sizeFuncBlock--;
						}
					}
				}
				fileCur++;
				(this->fileLIB).seekg(fileCur, ios::beg);
			}
			delete findNode;
			//关闭LIB文件
			while ((this->fileLIB).is_open())
				(this->fileLIB).close();
			//将临时文件中的全部内容复制到原来的LIB文件中
			(this->fileLIB).open(this->nameLib, ios::trunc | ios::out | ios::in | ios::binary);
			while (!(this->fileLIB).is_open())
				(this->fileLIB).open(this->nameLib, ios::trunc | ios::out | ios::in | ios::binary);
			tempFile.seekg(0, ios::end);
			sizeTempFile = tempFile.tellg();
			//设置文件指针到文件初始位置
			tempFile.seekg(0, ios::beg);
			TempFileCur = tempFile.tellg();
			while (1)
			{
				//判断是否到达文件末尾
				if (TempFileCur == sizeTempFile)
					break;
				(this->fileLIB).put(tempFile.get());
				TempFileCur++;
				tempFile.seekg(TempFileCur, ios::beg);
			}
			//关闭LIB文件
			tempFile.close();
			while (_unlink(tempFileName) == -1);
			//关闭LIB文件
			while ((this->fileLIB).is_open())
				(this->fileLIB).close();
			//打开LIB文件
			(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
			while (!(this->fileLIB).is_open())
				(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
		}
	}
}