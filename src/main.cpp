#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <dlfcn.h>
#include "ThostFtdcMdApi.h"
#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcTraderApi.h"
#include "ThostFtdcUserApiStruct.h"
#include "CMdSpi.h"
#include "CTraderSpi.h"

using namespace std;


//configuration parameter

// Api pointer
CThostFtdcTraderApi* pTradeUserApi = NULL;

// Trader spi
char gTradeFrontAddr[] = "tcp://180.168.146.187:10000";
TThostFtdcBrokerIDType gBrokerID = "9999";		//code of the broker
TThostFtdcInvestorIDType gInvestorID = "112585";
TThostFtdcPasswordType gInvestorPassword = "qq824073152";
TThostFtdcInstrumentIDType gTraderInstrumentID = "al1804";
TThostFtdcDirectionType gTradeDirection = THOST_FTDC_D_Buy; //THOST_FTDC_D_Sell or THOST_FTDC_D_Buy
TThostFtdcPriceType gLimitPrice = 14260;
int volume = 5;

//request id
int iRequestID = 0;

int main()
{
    // TraderSpi
    pTradeUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
    CTraderSpi *pTradeUserSpi = new CTraderSpi();
    pTradeUserApi->RegisterSpi((CThostFtdcTraderSpi*) pTradeUserSpi);
    pTradeUserApi->SubscribePublicTopic(THOST_TERT_QUICK);
    pTradeUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);
    pTradeUserApi->RegisterFront(gTradeFrontAddr);
    pTradeUserApi->Init();


    pTradeUserApi->Join();

	//pTradeUserApi->Release();
	return 0;
}
