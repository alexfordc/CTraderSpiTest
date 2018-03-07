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
void readConfig(char* filePath);

// Api pointer
CThostFtdcTraderApi* pTradeUserApi = NULL;

// Trader spi
char gTradeFrontAddr[] = "tcp://180.168.146.187:10000";//10030 10000
TThostFtdcBrokerIDType gBrokerID ;		//code of the broker
TThostFtdcInvestorIDType gInvestorID ;
TThostFtdcPasswordType gInvestorPassword ;
TThostFtdcInstrumentIDType gTraderInstrumentID ;
TThostFtdcDirectionType gTradeDirection = THOST_FTDC_D_Buy; //THOST_FTDC_D_Sell or THOST_FTDC_D_Buy
int gTradeType=0;
TThostFtdcPriceType gLimitPrice = 14260;
TThostFtdcPriceType gStopPrice = 14250;
int gVolume = 5;

// state flag
bool isFrontConnected = 0;
bool isLogin = 0;
bool isConfirm = 0;

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
	printf("gBrokerID=%s\n", gBrokerID);
	printf("gInvestorID=%s\n", gInvestorID);
	printf("gInvestorPassword=%s\n", gInvestorPassword);
	printf("gTradeType=%d\n", gTradeType);
	printf("gTraderInstrumentID=%s\n", gTraderInstrumentID);
	printf("gTradeDirection=%c\n", gTradeDirection);
	printf("gLimitPrice=%lf\n", gLimitPrice);
	printf("gVolume=%d\n", gVolume);

    //TraderSpi
    pTradeUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
    CTraderSpi *pTradeUserSpi = new CTraderSpi();
    pTradeUserApi->RegisterSpi((CThostFtdcTraderSpi*) pTradeUserSpi);
    pTradeUserApi->SubscribePublicTopic(THOST_TERT_QUICK);
    pTradeUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);
    pTradeUserApi->RegisterFront(gTradeFrontAddr);
    pTradeUserApi->Init();

    while(!isConfirm){}
    while(isConfirm){
    	printf("Please Enter config.ini path:\n");
    	scanf("%s",filePath);
    	readConfig(filePath);
    	pTradeUserSpi->ReqOrderInsertBy(pTradeUserSpi->GetOrderField(gTradeType));
    }

    

    

    //pTradeUserSpi->ReqOrderInsertBy(pTradeUserSpi->GetOrderField(gTradeType));


    pTradeUserApi->Join();

    
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
	

	char* investorId = ini.getStr("Investor","ID");
	sprintf(gInvestorID,"%s",investorId);
	

	char* password = ini.getStr("Investor","Password");
	sprintf(gInvestorPassword,"%s",password);
	

	gTradeType = ini.getInt("TradePara","TradeType");
	

	char* instrumentID = ini.getStr("TradePara","TraderInstrumentID");
	sprintf(gTraderInstrumentID,"%s",instrumentID);
	

	char* tradeDirection = ini.getStr("TradePara","TradeDirection");
	//sprintf(gTradeDirection,"%s",tradeDirection);
	gTradeDirection = tradeDirection[0];
	

	gLimitPrice = ini.getDouble("TradePara","LimitPrice");
	

	gVolume = ini.getInt("TradePara","Volume");
	
}