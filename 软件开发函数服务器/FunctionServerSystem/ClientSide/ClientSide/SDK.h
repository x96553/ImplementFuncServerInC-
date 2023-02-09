#ifndef _SDK_H_
#define _SDK_H_
namespace SDK
{
	void ErrorHandling(const char* message);
	void* FuncSCallProxy1(char* FuncName, unsigned int paramNum, ...);
	void FuncSCallProxy2(char* FuncName, unsigned long paramNum, char* paramContent, void* returnValue);
	void FuncSCallProxy3(char* FuncName, char* paramNeed);
}
#endif // !_SDK_H_
