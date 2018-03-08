#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <dlfcn.h>
#include <cstdlib>
#include <unistd.h>
#include "ThostFtdcTraderApi.h"
#include "CTraderSpi.h"
#include "Ini.h"

using namespace std;


//configuration parameter
//read config.ini file and asign to the global varibles
void readLoginConfig(char* filePath);



int main(int argc,char* argv[])
{
	printf("-------------\n");

	//get parameters
	if(argc < 2)
	{
		printf("Usage: ./Md -opt1 para1 -opt2 para2 ...\n");
		printf("Use \"./Md -h\" to see the options\n");
		exit(0);
	}

	char ch;

	char* filePath;

	while((ch = getopt(argc, argv, "f:h"))!= -1){
		switch(ch){

			case 'f':
				filePath = optarg;
				printf("%s\n", filePath);
				break;

			case 'h':
				printf("Options:\n");
				printf("Usage: ./Md -opt1 para1 -opt2 para2 ...\n");
				// printf("-b: Broker ID\n");
				// printf("-i: Investor ID\n");
				// printf("-p: Investor Password\n");
				printf("-f: login config filepath\n");
				// printf("-n: Sum of instrument\n");
				printf("-h: Help to list the options\n");
				exit(0);
				break;

			default:
				printf("Usage: ./Md -opt1 para1 -opt2 para2 ...\n");
				printf("Use \"./Md -h\" to see the options\n");
				exit(0);
				break;
		}
	}

    //TraderSpi
    CTraderSpi *pTradeUserSpi = new CTraderSpi(filePath);

    pTradeUserSpi->InitApi();

    while(!pTradeUserSpi->isConfirm){}
    while(pTradeUserSpi->isConfirm){
    	printf("Please Enter orderConfig.ini path:\n");
    	scanf("%s",filePath);
    	pTradeUserSpi->ReqOrderInsertBy(pTradeUserSpi->ReadOrderFieldIni(filePath));
    }

    pTradeUserSpi->JoinApi();

	//pTradeUserApi->Release();
	return 0;
}
