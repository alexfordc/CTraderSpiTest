#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <dlfcn.h>
#include <cstdlib>
#include <unistd.h>
#include "ThostFtdcMdApi.h"
#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcTraderApi.h"
#include "ThostFtdcUserApiStruct.h"
#include "CMdSpi.h"
#include "CTraderSpi.h"
#include "Ini.h"

using namespace std;


//configuration parameter
//read config.ini file and asign to the global varibles
void readConfig(char* filePath);

// Api pointer
CThostFtdcTraderApi* pTradeUserApi = NULL;

// Trader spi
char gTradeFrontAddr[] = "tcp://180.168.146.187:10000";
TThostFtdcBrokerIDType gBrokerID = "9999";		//code of the broker
TThostFtdcInvestorIDType gInvestorID = "112585";
TThostFtdcPasswordType gInvestorPassword = "qq824073152";
TThostFtdcInstrumentIDType gTraderInstrumentID = "al1804";
TThostFtdcDirectionType gTradeDirection = THOST_FTDC_D_Buy; //THOST_FTDC_D_Sell or THOST_FTDC_D_Buy
int gTradeType=0;
TThostFtdcPriceType gLimitPrice = 14260;
TThostFtdcPriceType gStopPrice = 14260;
int gVolume = 5;

//request id
int iRequestID = 0;

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

			// case 'b':
			// 	sprintf(gBrokerID,"%s",optarg);
			// 	printf("%s\n", gBrokerID);
			// 	break;

			// case 'i':
			// 	sprintf(gInvestorID,"%s",optarg);
			// 	printf("%s\n", gInvestorID);
			// 	break;

			// case 'p':
			// 	sprintf(gInvestorPassword,"%s",optarg);
			// 	printf("%s\n", gInvestorPassword);
			// 	break;

			case 'f':
				filePath = optarg;
				printf("%s\n", filePath);
				break;

			// case 'n':
			// 	instrumentNum = atoi(optarg);
			// 	printf("%d\n", instrumentNum);
				break;

			case 'h':
				printf("Options:\n");
				printf("Usage: ./Md -opt1 para1 -opt2 para2 ...\n");
				// printf("-b: Broker ID\n");
				// printf("-i: Investor ID\n");
				// printf("-p: Investor Password\n");
				printf("-f: config filepath\n");
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

	readConfig(filePath);

    // TraderSpi
    // pTradeUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
    // CTraderSpi *pTradeUserSpi = new CTraderSpi();
    // pTradeUserApi->RegisterSpi((CThostFtdcTraderSpi*) pTradeUserSpi);
    // pTradeUserApi->SubscribePublicTopic(THOST_TERT_QUICK);
    // pTradeUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);
    // pTradeUserApi->RegisterFront(gTradeFrontAddr);
    // pTradeUserApi->Init();


    // pTradeUserApi->Join();

	//pTradeUserApi->Release();
	return 0;
}

//read config.ini file and asign to the global varibles
void readConfig(char* filePath){
	//read config.ini file
	CIni ini;

	ini.openFile(filePath,"r");

	char* brokerId = ini.getStr("Broker","ID");
	sprintf(gBrokerID,"%s",brokerId);
	printf("gBrokerID=%s\n", gBrokerID);

	char* investorId = ini.getStr("Investor","ID");
	sprintf(gInvestorID,"%s",investorId);
	printf("gInvestorID=%s\n", gInvestorID);

	char* password = ini.getStr("Investor","Password");
	sprintf(gInvestorPassword,"%s",password);
	printf("gInvestorPassword=%s\n", gInvestorPassword);

	gTradeType = ini.getInt("TradePara","TradeType");
	printf("gTradeType=%d\n", gTradeType);

	char* instrumentID = ini.getStr("TradePara","TraderInstrumentID");
	sprintf(gTraderInstrumentID,"%s",instrumentID);
	printf("gTraderInstrumentID=%s\n", gTraderInstrumentID);

	char* tradeDirection = ini.getStr("TradePara","TradeDirection");
	//sprintf(gTradeDirection,"%s",tradeDirection);
	gTradeDirection = tradeDirection[0];
	printf("gTradeDirection=%c\n", gTradeDirection);

	gLimitPrice = ini.getDouble("TradePara","LimitPrice");
	printf("gLimitPrice=%lf\n", gLimitPrice);

	gVolume = ini.getInt("TradePara","Volume");
	printf("gVolume=%d\n", gVolume);
}