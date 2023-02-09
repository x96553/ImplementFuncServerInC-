#include<string>
#include"serverClientLib.h"
void ServerClient::ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
serverGetLine::serverGetLine()
{
	this->startSpace = std::string::npos;
	this->endSpace = std::string::npos;
}
int serverGetLine::getLine()
{
	while (1)
	{
		getline(cin, this->Line);
		this->startSpace = 0;
		while (this->Line[this->startSpace] == ' ')
			this->startSpace++;
		this->endSpace = this->startSpace;
		while (this->Line[this->endSpace] != ' ' && this->Line[this->endSpace] != '\0')
			this->endSpace++;
		if (string("exit").compare(this->Line.substr(this->startSpace, this->endSpace - this->startSpace)) == 0)
			return 0;
		else if (string("server_client").compare(this->Line.substr(this->startSpace, this->endSpace - this->startSpace)) == 0)
			return 1;
		else if (string("server_lib").compare(this->Line.substr(this->startSpace, this->endSpace - this->startSpace)) == 0)
			return 2;
	}
}