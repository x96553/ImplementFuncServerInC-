#include"serverClientLib.h"
int main()
{
	serverGetLine line;
	switch (line.getLine())
	{
		case 0:
			break;
		case 1: 
		{
			ServerClient client;
			client.openLibServerClient();
			break;
		}
		case 2:
		{
			ServerLib lib;
			lib.openLibServerClient();
			break;
		}
	}
	system("pause");
	return 0;
}