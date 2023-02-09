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
	//���������
	(this->tree)->DestroyRBTree(&this->tree);
}
int ServerLib::openLibServerClient()
{
	(this->fileLIB).open(nameLib, ios::_Nocreate);
	//�ж�LIB�ļ��Ƿ�򿪣�������ִ��if���
	if ((this->fileLIB).is_open())
	{
		//�������
		char funcBlock[11] = { 0 };
		unsigned int fileCur, fileEnd;
		//�ر�LIB�ļ�
		while ((this->fileLIB).is_open())
			(this->fileLIB).close();
		//��LIB�ļ�
		(this->fileLIB).open(nameLib, ios::app | ios::out | ios::in | ios::binary);
		while (!(this->fileLIB).is_open())
			(this->fileLIB).open(nameLib, ios::app | ios::out | ios::in | ios::binary);
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
				if (!funcName)
					return 0;
				(this->fileLIB).read(funcName, 255);
				//��ȡ������LIB�ļ��е���ʼλ�ã�������funcStartPos��
				funcStartPos = (this->fileLIB).tellg();
				//����������ڵ�
				node = new RBNode(funcName, funcStartPos, funcSize);
				if (!node)
					return 0;
				//��������ڵ����������
				(this->tree)->AddNodeC(&this->tree, node);
			}
			fileCur++;
			(this->fileLIB).seekg(fileCur, ios::beg);
		}
		//�ر�LIB�ļ�
		while ((this->fileLIB).is_open())
			(this->fileLIB).close();
		ServerLib::getLine();
	}
	else
	{
		//��LIB�ļ�
		(this->fileLIB).open(nameLib, ios::app | ios::out | ios::in | ios::binary);
		while (!(this->fileLIB).is_open())
			(this->fileLIB).open(nameLib, ios::app | ios::out | ios::in | ios::binary);
		fileLIB.seekp(0, ios::beg);
		fileLIB.write("Function Server Libs v1.0", strlen("Function Server Libs v1.0"));
		fileLIB.put(0x0D);
		fileLIB.put(0x0A);
		fileLIB.put(0x1A);
		//�ر�LIB�ļ�
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
	//�ر�LIB�ļ�
	while ((this->fileLIB).is_open())
		(this->fileLIB).close();
	//��LIB�ļ�
	(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
	while (!(this->fileLIB).is_open())
		(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
	while (1)
	{
		//�������
		RBNode* findNode;
		string funcAddName, funcAddNameAndPath;
		std::string::size_type funcStart = std::string::npos;
		std::string::size_type funcEnd = std::string::npos;
		//�������ַ��������������˳�ѭ��
		if ((this->Line)[endSpace] == '\0')
			return 0;
		//���λ�ȡ���뵽����̨�ĺ�������
		startSpace = endSpace;
		while ((this->Line)[startSpace] == ' ')
			startSpace++;
		endSpace = startSpace;
		while ((this->Line)[endSpace] != ' ' && (this->Line)[endSpace] != '\0')
			endSpace++;
		funcAddNameAndPath = (this->Line).substr(startSpace, endSpace - startSpace);//����������funcCmdBuf��
		//��������⿴�Ƿ������Ҫ���ӵĽڵ�
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
				//�������
				unsigned int startDLL, endDLL, sizeDLL, sizeDiv, sizeBytes, funcPackegeLen;
				std::string::size_type funcStart = std::string::npos;
				std::string::size_type funcEnd = std::string::npos;
				string funcPackegeName;
				//���������ڵ����
				char* funcName_Node = new char[255];
				unsigned int funcStartPos_Node;
				RBNode* node;
				(this->fileLIB).seekp(0, ios::end);
				(this->fileLIB).write("Func Block", strlen("Func Block"));
				//����DLL�ļ���С
				fileDLL.seekg(0, ios::beg);
				startDLL = fileDLL.tellg();
				fileDLL.seekg(0, ios::end);
				endDLL = fileDLL.tellg();
				sizeDLL = endDLL - startDLL;//������С����sizeDLL��
				//��DLL�Ĵ�С��ʮ����д��LIB�ļ���ռ�ĸ��ֽ�
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
				//��DLL���ļ�������������LIB�У��ṩ�����к�����LIB����ѭ������
				funcPackegeName = this->nameLib + "." + funcAddName;//LIB����+��.��+������
				(this->fileLIB).write(funcPackegeName.c_str(), strlen(funcPackegeName.c_str()));
				funcPackegeLen = 255 - strlen(funcPackegeName.c_str());
				while (1)
				{
					if (funcPackegeLen == 0)
						break;
					(this->fileLIB).put(0x00);
					funcPackegeLen--;
				}
				//��ȡ��ǰ������LIB���е���ʼ��ַ�ͺ�����
				strcpy_s(funcName_Node, 255, funcPackegeName.c_str());
				funcStartPos_Node = (this->fileLIB).tellp();
				//���ѱ���õģ���ΪDLL��̬����ļ�������д��LIB����
				fileDLL.seekg(0, ios::beg);
				while (1)
				{
					if (sizeDLL == 0)
						break;
					(this->fileLIB).put(fileDLL.get());
					sizeDLL--;
				}
				//�ر�DLL�ļ�
				fileDLL.close();
				node = new RBNode(funcName_Node, funcStartPos_Node, sizeDLL);
				(this->tree)->AddNodeC(&this->tree, node);
			}
			//�ر�LIB�ļ�
			while ((this->fileLIB).is_open())
				(this->fileLIB).close();
			//��LIB�ļ�
			(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
			while (!(this->fileLIB).is_open())
				(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
		}
	}
}
int ServerLib::modifyServerLib(std::string::size_type startSpace, std::string::size_type endSpace)
{
	ifstream fileDLL;
	//�ر�LIB�ļ�
	while ((this->fileLIB).is_open())
		(this->fileLIB).close();
	//��LIB�ļ�
	(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
	while (!(this->fileLIB).is_open())
		(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
	while (1)
	{
		//�������
		RBNode* findNode;
		string funcNameModify;
		string funcNameModified;
		//�������ַ��������������˳�ѭ��
		if ((this->Line)[endSpace] == '\0')
			return 0;
		//���λ�ȡ���뵽����̨�ĺ�������
		startSpace = endSpace;
		while ((this->Line)[startSpace] == ' ')
			startSpace++;
		endSpace = startSpace;
		while ((this->Line)[endSpace] != ' ' && (this->Line)[endSpace] != '\0')
			endSpace++;
		funcNameModified = (this->Line).substr(startSpace, endSpace - startSpace);
		//��������⿴�Ƿ������Ҫ�����Ľڵ�
		if (!(this->tree)->BrowseNodeC(this->tree, funcNameModified, &findNode))
			return 0;
		else
		{
			//�������ַ��������������˳�ѭ��
			if ((this->Line)[endSpace] == '\0')
				return 0;
			//���λ�ȡ���뵽����̨�ĺ�������
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
				//�������
				char tempFileName[] = "XXXXXX";
				errno_t errorMakeTempFile;
				fstream tempFile;
				unsigned int fileCur, fileEnd;
				char funcBlock[11] = { 0 };
				unsigned int sizeTempFile, TempFileCur;
				//������ʱ�ļ�����
				errorMakeTempFile = _mktemp_s(tempFileName, strlen(tempFileName) + 1);
				while (errorMakeTempFile != 0)
					errorMakeTempFile = _mktemp_s(tempFileName, strlen(tempFileName) + 1);
				if (errorMakeTempFile == 0)
					tempFile.open(tempFileName, ios::app | ios::out | ios::in | ios::binary);
				while (!tempFile.is_open())
					tempFile.open(tempFileName, ios::app | ios::out | ios::in | ios::binary);
				//����tempFile��ʱ�ļ�ָ��ĳ�ʼλ��
				tempFile.seekp(0, ios::beg);
				tempFile.write("Function Server Libs v1.0", strlen("Function Server Libs v1.0"));
				tempFile.put(0x0D);
				tempFile.put(0x0A);
				tempFile.put(0x1A);
				//�����ļ�ָ�뵽�ļ���ʼλ��
				(this->fileLIB).seekg(0, ios::end);
				fileEnd = (this->fileLIB).tellg();
				(this->fileLIB).seekg(0, ios::beg);
				fileCur = (this->fileLIB).tellg();
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
						std::string::size_type funcStart = std::string::npos;
						std::string::size_type funcEnd = std::string::npos;
						unsigned int funcSizeOfModity, funcSizeI;
						string funcInLibName;
						char findFuncOfLIB[255] = { 0 };
						funcSizeOfModity = 0;
						funcSizeI = 1;
						//��ȡ������ռ�ֽڴ�С������funcSize��
						while (1)
						{
							if (funcSizeI == unsigned int(10000000000))
								break;
							funcSizeOfModity = funcSizeOfModity + funcSizeI * (this->fileLIB).get();
							funcSizeI = funcSizeI * 10;
						}
						//��LIB�ж�ȥ������
						(this->fileLIB).read(findFuncOfLIB, 255);
						funcStart = (string(findFuncOfLIB)).find_last_of('.');
						funcInLibName = (string(findFuncOfLIB)).substr(funcStart + 1, strlen(findFuncOfLIB) - funcStart - 1);
						//�����������ʼ��ʶд����ʱ�ļ���
						tempFile.write(funcBlock, strlen("Func Block"));
						//�жϱ��滻�ĺ��������滻�ĺ������Ƿ����
						if (strcmp(funcInLibName.c_str(), funcNameModified.c_str()) == 0)
						{
							//�������
							string ModityName;
							unsigned int startDLL, endDLL, sizeDLL, sizeDiv, sizeBytes;
							//����DLL�ļ���С
							fileDLL.seekg(0, ios::beg);
							startDLL = fileDLL.tellg();
							fileDLL.seekg(0, ios::end);
							endDLL = fileDLL.tellg();
							sizeDLL = endDLL - startDLL;//������С����sizeDLL��
							//��DLL�Ĵ�С��ʮ����д��LIB�ļ���ռ�ĸ��ֽ�
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
							//��ȡ�滻�ĺ�����
							funcStart = funcNameModify.find_last_of('\\');
							funcEnd = funcNameModify.find_last_of('.');
							ModityName = funcNameModify.substr(funcStart + 1, funcEnd - funcStart - 1);
							if (strcmp(funcInLibName.c_str(), ModityName.c_str()) == 0)
							{
								(this->tree)->ModifyNodeC(&this->tree, findNode, sizeDLL);
								//�����������Ƶ���ʱ�ļ���
								tempFile.write(findFuncOfLIB, 255);
								//������������ݸ��Ƶ���ʱ�ļ���
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
								//�������
								unsigned int funcPackegeLen;
								string funcPackegeName, funcToModifyName;
								//���������ڵ����
								char* funcName_Node = new char[255];
								unsigned int funcStartPos_Node;
								RBNode* node;
								//��DLL���ļ�������������LIB�У��ṩ�����к�����LIB����ѭ������
								funcStart = funcNameModify.find_last_of('\\');
								funcEnd = funcNameModify.find_last_of('.');
								funcToModifyName = funcNameModify.substr(funcStart + 1, funcEnd - funcStart - 1);
								funcPackegeName = this->nameLib + "." + funcToModifyName;//LIB����+��.��+������
								tempFile.write(funcPackegeName.c_str(), strlen(funcPackegeName.c_str()));
								funcPackegeLen = 255 - strlen(funcPackegeName.c_str());
								while (1)
								{
									if (funcPackegeLen == 0)
										break;
									tempFile.put(0x00);
									funcPackegeLen--;
								}
								//��ȡ��ǰ������LIB���е���ʼ��ַ�ͺ�����
								strcpy_s(funcName_Node, 255, funcPackegeName.c_str());
								funcStartPos_Node = tempFile.tellp();
								(this->tree)->DeleteNodeC(&this->tree, &findNode);
								node = new RBNode(funcName_Node, funcStartPos_Node, sizeDLL);
								(this->tree)->AddNodeC(&this->tree, node);
								//������������ݸ��Ƶ���ʱ�ļ���
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
							//�������
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
							//�����������Ƶ���ʱ�ļ���
							tempFile.write(findFuncOfLIB, 255);
							//������������ݸ��Ƶ���ʱ�ļ���
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
				//�ر�LIB�ļ�
				while ((this->fileLIB).is_open())
					(this->fileLIB).close();
				//����ʱ�ļ��е�ȫ�����ݸ��Ƶ�ԭ����LIB�ļ���
				(this->fileLIB).open(this->nameLib, ios::trunc | ios::out | ios::in | ios::binary);
				while (!(this->fileLIB).is_open())
					(this->fileLIB).open(this->nameLib, ios::trunc | ios::out | ios::in | ios::binary);
				tempFile.seekg(0, ios::end);
				sizeTempFile = tempFile.tellg();
				//�����ļ�ָ�뵽�ļ���ʼλ��
				tempFile.seekg(0, ios::beg);
				TempFileCur = tempFile.tellg();
				while (1)
				{
					//�ж��Ƿ񵽴��ļ�ĩβ
					if (TempFileCur == sizeTempFile)
						break;
					(this->fileLIB).put(tempFile.get());
					TempFileCur++;
					tempFile.seekg(TempFileCur, ios::beg);
				}
				//�ر�LIB�ļ�
				tempFile.close();
				while (_unlink(tempFileName) == -1);
				//�ر�DLL�ļ�
				fileDLL.close();
			}
			//�ر�LIB�ļ�
			while ((this->fileLIB).is_open())
				(this->fileLIB).close();
			//��LIB�ļ�
			(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
			while (!(this->fileLIB).is_open())
				(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
		}
	}
}
int ServerLib::browseServerLib(std::string::size_type startSpace, std::string::size_type endSpace)
{
	//�������
	char funcBlock[11] = { 0 };
	unsigned int fileCur, fileEnd;
	RBNode* findNode;
	string funcNodeName, funcBrowseName;
	std::string::size_type funcStart = std::string::npos;
	std::string::size_type funcEnd = std::string::npos;
	//�ر�LIB�ļ�
	while ((this->fileLIB).is_open())
		(this->fileLIB).close();
	//��LIB�ļ�
	(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
	while (!(this->fileLIB).is_open())
		(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
	while (1)
	{
		//�������ַ��������������˳�ѭ��
		if ((this->Line)[endSpace] == '\0')
			return 0;
		//���λ�ȡ���뵽����̨�ĺ�������
		startSpace = endSpace;
		while ((this->Line)[startSpace] == ' ')
			startSpace++;
		endSpace = startSpace;
		while ((this->Line)[endSpace] != ' ' && (this->Line)[endSpace] != '\0')
			endSpace++;
		funcBrowseName = (this->Line).substr(startSpace, endSpace - startSpace);
		//���������
		funcStart = funcBrowseName.find_last_of('\\');
		funcEnd = funcBrowseName.find_last_of('.');
		funcNodeName = funcBrowseName.substr(funcStart + 1, funcEnd - funcStart - 1);
		if (!(this->tree)->BrowseNodeC(this->tree, funcNodeName, &findNode))
			return 0;
		else
		{
			//�����ļ�ָ�뵽�ļ���ʼλ��
			(this->fileLIB).seekg(0, ios::end);
			fileEnd = (this->fileLIB).tellg();
			(this->fileLIB).seekg(0, ios::beg);
			fileCur = (this->fileLIB).tellg();
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
					unsigned int funcSizeOfBrowse, funcSizeI;
					char findFuncOfLIB[255] = { 0 };
					funcSizeOfBrowse = 0;
					funcSizeI = 1;
					//��ȡ������ռ�ֽڴ�С������funcSize��
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
			//�ر�LIB�ļ�
			while ((this->fileLIB).is_open())
				(this->fileLIB).close();
			//��LIB�ļ�
			(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
			while (!(this->fileLIB).is_open())
				(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
		}
	}
}
int ServerLib::deleteServerLib(std::string::size_type startSpace, std::string::size_type endSpace)
{
	//�������
	char funcBlock[11] = { 0 };
	unsigned int fileCur, fileEnd;
	RBNode* findNode;
	string funcDeleteName;
	//�ر�LIB�ļ�
	while ((this->fileLIB).is_open())
		(this->fileLIB).close();
	//��LIB�ļ�
	(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
	while (!(this->fileLIB).is_open())
		(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
	while (1)
	{
		//�������ַ��������������˳�ѭ��
		if ((this->Line)[endSpace] == '\0')
			return 0;
		//���λ�ȡ���뵽����̨�ĺ�������
		startSpace = endSpace;
		while ((this->Line)[startSpace] == ' ')
			startSpace++;
		endSpace = startSpace;
		while ((this->Line)[endSpace] != ' ' && (this->Line)[endSpace] != '\0')
			endSpace++;
		funcDeleteName = (this->Line).substr(startSpace, endSpace - startSpace);
		//��������⿴�Ƿ������Ҫ�����Ľڵ�
		if (!(this->tree)->BrowseNodeC(this->tree, funcDeleteName, &findNode))
			return 0;
		else
		{
			//������ʱ�ļ�����ı���
			char tempFileName[] = "XXXXXX";
			errno_t errorMakeTempFile;
			fstream tempFile;
			unsigned int sizeTempFile, TempFileCur;
			//������ʱ�ļ�����
			errorMakeTempFile = _mktemp_s(tempFileName, strlen(tempFileName) + 1);
			while (errorMakeTempFile != 0)
				errorMakeTempFile = _mktemp_s(tempFileName, strlen(tempFileName) + 1);
			if (errorMakeTempFile == 0)
				tempFile.open(tempFileName, ios::app | ios::out | ios::in | ios::binary);
			while (!tempFile.is_open())
				tempFile.open(tempFileName, ios::app | ios::out | ios::in | ios::binary);
			//����tempFile��ʱ�ļ�ָ��ĳ�ʼλ��
			tempFile.seekp(0, ios::beg);
			tempFile.write("Function Server Libs v1.0", strlen("Function Server Libs v1.0"));
			tempFile.put(0x0D);
			tempFile.put(0x0A);
			tempFile.put(0x1A);
			//ɾ��������
			(this->tree)->DeleteNodeC(&this->tree, &findNode);
			//�����ļ�ָ�뵽�ļ���ʼλ��
			(this->fileLIB).seekg(0, ios::end);
			fileEnd = (this->fileLIB).tellg();
			(this->fileLIB).seekg(0, ios::beg);
			fileCur = (this->fileLIB).tellg();
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
					unsigned int funcSizeOfDelete, funcSizeI;
					char findFuncOfLIB[255] = { 0 };
					funcSizeOfDelete = 0;
					funcSizeI = 1;
					//��ȡ������ռ�ֽڴ�С������funcSize��
					while (1)
					{
						if (funcSizeI == unsigned int(10000000000))
							break;
						funcSizeOfDelete = funcSizeOfDelete + funcSizeI * (this->fileLIB).get();
						funcSizeI = funcSizeI * 10;
					}
					(this->fileLIB).read(findFuncOfLIB, 255);
					//�����ɾ���ĺ������뱻ɾ�ڵ�ĺ�������ͬ����������Ҫ�����Ƶ���ʱ�ļ��ĺ�����
					//����LIB�ļ����йر�ɾ�ڵ�ĺ��������ݸ��Ƶ���ʱ�ļ���
					if (strncmp(findFuncOfLIB, findNode->getFuncName(), 255) == 0)
						(this->fileLIB).seekg(funcSizeOfDelete, ios::cur);
					else
					{
						//�������
						int sizeBytes, sizeFuncBlock;
						//�����������ʼ��ʶд����ʱ�ļ���
						tempFile.write(funcBlock, strlen("Func Block"));
						//��������Ĵ�С��ʮ����д����ʱ�ļ��У�ռ�ĸ��ֽ�
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
						//�����������Ƶ���ʱ�ļ���
						tempFile.write(findFuncOfLIB, 255);
						//������������ݸ��Ƶ���ʱ�ļ���
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
			//�ر�LIB�ļ�
			while ((this->fileLIB).is_open())
				(this->fileLIB).close();
			//����ʱ�ļ��е�ȫ�����ݸ��Ƶ�ԭ����LIB�ļ���
			(this->fileLIB).open(this->nameLib, ios::trunc | ios::out | ios::in | ios::binary);
			while (!(this->fileLIB).is_open())
				(this->fileLIB).open(this->nameLib, ios::trunc | ios::out | ios::in | ios::binary);
			tempFile.seekg(0, ios::end);
			sizeTempFile = tempFile.tellg();
			//�����ļ�ָ�뵽�ļ���ʼλ��
			tempFile.seekg(0, ios::beg);
			TempFileCur = tempFile.tellg();
			while (1)
			{
				//�ж��Ƿ񵽴��ļ�ĩβ
				if (TempFileCur == sizeTempFile)
					break;
				(this->fileLIB).put(tempFile.get());
				TempFileCur++;
				tempFile.seekg(TempFileCur, ios::beg);
			}
			//�ر�LIB�ļ�
			tempFile.close();
			while (_unlink(tempFileName) == -1);
			//�ر�LIB�ļ�
			while ((this->fileLIB).is_open())
				(this->fileLIB).close();
			//��LIB�ļ�
			(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
			while (!(this->fileLIB).is_open())
				(this->fileLIB).open(this->nameLib, ios::app | ios::out | ios::in | ios::binary);
		}
	}
}