#include "stdio.h"
#include "stdlib.h"
#include "sys.h"

#include "CommandTree.h"
#include "ExecuteCommand.h"
#include "globe.h"
#include "PreExecute.h"
#include "string.h"

char line[1000];
char* args[50];
int argsCnt;
int main1(int argc,char *argv[])
{
	char lineInput[512];
	getPath( curPath );	
	int root;
	while( 1 )
	{
		root = -1;
		argsCnt = 0;
		InitCommandTree();
		printf("[%s %s]$", argv[1],curPath);
		gets( lineInput );		
		if ( strcmp( "Logout", lineInput ) == 0 )
		{
			syncFileSystem();
			printf("You can safely Logout the system now!\n");
			break;
		}
		argsCnt = SpiltCommand(lineInput);
		root = AnalizeCommand(0, argsCnt - 1, 0);
		if ( root >= 0 )
		{
			ExecuteCommand( &commandNodes[root], 0, 0 );
		}		
	}

	return 0;
}

