#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include "CTraderSpi.h"
#include "ThostFtdcTraderApi.h"
#include "Ini.h"
using namespace std;

// USER_API parameter
extern CThostFtdcTraderApi* pTradeUserApi;

// configure parameter
extern char gTradeFrontAddr[];               //front address
extern TThostFtdcBrokerIDType gBrokerID;				    //broker id
extern TThostFtdcInvestorIDType gInvestorID;				//investor id
extern TThostFtdcPasswordType gInvestorPassword;			//investor password

// configure used in orderinserting
TThostFtdcInstrumentIDType gTraderInstrumentID;		//instrument id
TThostFtdcPriceType gLimitPrice; //limit price
TThostFtdcPriceType gStopPrice; //stop price
int gTradeType;// trade request type;
// int gVolume; //volume
// TThostFtdcDirectionType gTradeDirection;//Trading direction

// state flag
extern bool isFrontConnected;
extern bool isConfirm;

// request id
extern int iRequestID;

// session parameters 
TThostFtdcFrontIDType frontID; 		//front id
TThostFtdcSessionIDType	sessionID;		//session id
TThostFtdcOrderRefType	gOrderRef;		//order reference
TThostFtdcOrderRefType	exeOrderRef;	//execute order reference 
TThostFtdcOrderRefType	forquoteRef;	//for quote reference
TThostFtdcOrderRefType	quoteRef;		//quote reference

// 
bool IsFlowControl(int iResult)
{
	return ((iResult == -2) || (iResult == -3));
}

void CTraderSpi::OnFrontConnected()
{
	cout << "--->>> " << "OnFrontConnected" << endl;

	// set flag
	isFrontConnected = 1;
	/// user login requst
	ReqUserLogin();
}

void CTraderSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, gBrokerID);
	strcpy(req.UserID, gInvestorID);
	strcpy(req.Password, gInvestorPassword);
	int iResult = pTradeUserApi->ReqUserLogin(&req, ++iRequestID);
	cout << "--->>> Send user login request result: " << iResult << ((iResult == 0) ? ", Succeed" : ", Failed") << endl;
}

void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "--->>> " << "OnRspUserLogin" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		// save session parameter
		frontID = pRspUserLogin->FrontID;
		sessionID = pRspUserLogin->SessionID;
		int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
		iNextOrderRef++;
		sprintf(gOrderRef, "%d", iNextOrderRef);
		sprintf(exeOrderRef, "%d", 1);
		sprintf(forquoteRef, "%d", 1);
		sprintf(quoteRef, "%d", 1);
		///get current trading day
		cout << "--->>> TradingDay = " << pTradeUserApi->GetTradingDay() << endl;

		///Settlement infomation confirm
		ReqSettlementInfoConfirm();
		
	}
}

void CTraderSpi::ReqSettlementInfoConfirm()
{
	CThostFtdcSettlementInfoConfirmField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, gBrokerID);
	strcpy(req.InvestorID, gInvestorID);
	int iResult = pTradeUserApi->ReqSettlementInfoConfirm(&req, ++iRequestID);
	cout << "--->>> ReqSettlementInfoConfirm result: " << iResult << ((iResult == 0) ? ", Succeed" : ", Failed") << endl;
}

void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "--->>> " << "OnRspSettlementInfoConfirm" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///Requst query instrument
		//ReqQryInstrument();
		isConfirm = 1;
	}
}

void CTraderSpi::ReqQryInstrument()
{
	CThostFtdcQryInstrumentField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.InstrumentID, gTraderInstrumentID);
	while (true)
	{
		int iResult = pTradeUserApi->ReqQryInstrument(&req, ++iRequestID);
		if (!IsFlowControl(iResult))
		{
			cout << "--->>> ReqQryInstrument result: "  << iResult << ((iResult == 0) ? ", Succeed" : ", Failed") << endl;
			break;
		}
		else
		{
			cout << "--->>> ReqQryInstrument result: "  << iResult << ", FlowControl" << endl;
			usleep(1000);
		}
	} // while
}

void CTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "--->>> " << "OnRspQryInstrument" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		/// requst query trading account
		//ReqQryTradingAccount();
	}
}

void CTraderSpi::ReqQryTradingAccount()
{
	CThostFtdcQryTradingAccountField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, gBrokerID);
	strcpy(req.InvestorID, gInvestorID);
	while (true)
	{
		int iResult = pTradeUserApi->ReqQryTradingAccount(&req, ++iRequestID);
		if (!IsFlowControl(iResult))
		{
			cout << "--->>> ReqQryTradingAccount result: "  << iResult << ((iResult == 0) ? ", Succeed" : ", Failed") << endl;
			break;
		}
		else
		{
			cout << "--->>> ReqQryTradingAccount result: "  << iResult << ", FlowControl" << endl;
			usleep(1000);
		}
	} // while
}

void CTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "--->>> " << "OnRspQryTradingAccount" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///Requst query investor position
		//ReqQryInvestorPosition();
	}
}

void CTraderSpi::ReqQryInvestorPosition()
{
	CThostFtdcQryInvestorPositionField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, gBrokerID);
	strcpy(req.InvestorID, gInvestorID);
	strcpy(req.InstrumentID, gTraderInstrumentID);
	while (true)
	{
		int iResult = pTradeUserApi->ReqQryInvestorPosition(&req, ++iRequestID);
		if (!IsFlowControl(iResult))
		{
			cout << "--->>> ReqQryInvestorPosition result: "  << iResult << ((iResult == 0) ? ", Succeed" : ", Failed") << endl;
			break;
		}
		else
		{
			cout << "--->>> ReqQryInvestorPosition result: "  << iResult << ", FlowControl" << endl;
			usleep(1000);
		}
	} // while
}

void CTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "--->>> " << "OnRspQryInvestorPosition" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		cout<<"todo..."<<endl;
		cout<<"QryInvestorPosition ok\n";
	}
}

void CTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "--->>> " << "OnRspOrderInsert" << endl;
	IsErrorRspInfo(pRspInfo);
}

void CTraderSpi::ReqOrderAction(CThostFtdcOrderField *pOrder)
{
	static bool ORDER_ACTION_SENT = false;		//wheter sent order action
	if (ORDER_ACTION_SENT)
		return;

	CThostFtdcInputOrderActionField req;
	memset(&req, 0, sizeof(req));
	///broker id
	strcpy(req.BrokerID, pOrder->BrokerID);
	///investor id
	strcpy(req.InvestorID, pOrder->InvestorID);
	///order action reference
	//TThostFtdcOrderActionRefType	OrderActionRef;
	///order reference
	strcpy(req.OrderRef, pOrder->OrderRef);
	///request id
	//TThostFtdcRequestIDType	RequestID;
	///front id
	req.FrontID = frontID;
	///session id
	req.SessionID = sessionID;
	///exchange id
	//TThostFtdcExchangeIDType	ExchangeID;
	///order system id
	//TThostFtdcOrderSysIDType	OrderSysID;
	///action flag setting:delete order
	req.ActionFlag = THOST_FTDC_AF_Delete;
	/// price type
	//TThostFtdcPriceType	LimitPrice;
	///volume type
	//TThostFtdcVolumeType	VolumeChange;
	///user id
	//TThostFtdcUserIDType	UserID;
	///instrument id
	strcpy(req.InstrumentID, pOrder->InstrumentID);

	int iResult = pTradeUserApi->ReqOrderAction(&req, ++iRequestID);
	cout << "--->>> Req OrderCanel: "  << iResult << ((iResult == 0) ? ", Succeed" : ", Failed") << endl;
	ORDER_ACTION_SENT = true;
}

void CTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "--->>> " << "OnRspOrderAction" << endl;
	IsErrorRspInfo(pRspInfo);
}

///Return Order information
void CTraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	cout << "--->>> " << "OnRtnOrder"  << endl;
	if (IsMyOrder(pOrder))
	{
		printf("OrderLocalID=%s\n",pOrder-> OrderLocalID );
		printf("OrderSysID=%s\n",pOrder-> OrderSysID );
		if (IsTradingOrder(pOrder))
		{
			cout<< "--->>> IsTradingOrder..." << endl;
			//ReqOrderAction(pOrder);
		}
		else if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)
		{
			cout << "--->>> Order canceled" << endl;
		}
	}
}

///trade suceed information return
void CTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	cout << "--->>> " << "OnRtnTrade"  << endl;
}

void CTraderSpi:: OnFrontDisconnected(int nReason)
{
	cout << "--->>> " << "OnFrontDisconnected" << endl;
	cout << "--->>> Reason = " << nReason << endl;
}
		
void CTraderSpi::OnHeartBeatWarning(int nTimeLapse)
{
	cout << "--->>> " << "OnHeartBeatWarning" << endl;
	cout << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

void CTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "--->>> " << "OnRspError" << endl;
	IsErrorRspInfo(pRspInfo);
}

bool CTraderSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// error occurred and recieve error response if ErrorID != 0
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		cout << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	return bResult;
}

bool CTraderSpi::IsMyOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->FrontID == frontID) &&
			(pOrder->SessionID == sessionID) &&
			(strcmp(pOrder->OrderRef, gOrderRef) == 0));
}

bool CTraderSpi::IsTradingOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
			(pOrder->OrderStatus != THOST_FTDC_OST_Canceled) &&
			(pOrder->OrderStatus != THOST_FTDC_OST_AllTraded));
}

//get the CThostFtdcInputOrderField according to config.ini
CThostFtdcInputOrderField CTraderSpi::ReadOrderFieldIni(char* filePath){
	// OrderField used in OrderInsert
	CThostFtdcInputOrderField req;

	////////////////same
	memset(&req, 0, sizeof(req));
	///Broder id
	strcpy(req.BrokerID, gBrokerID);
	///inverstor id
	strcpy(req.InvestorID, gInvestorID);
	///order reference
	strcpy(req.OrderRef, gOrderRef);

	cout<< "gOrderRef=" << gOrderRef <<endl;
	int iOrderRef = atoi(gOrderRef);
	iOrderRef++;
	sprintf(gOrderRef,"%d",iOrderRef);
	cout<< "gOrderRef=" << gOrderRef <<endl;


	//read config.ini file
	CIni ini;
	ini.openFile(filePath,"r");

	gTradeType = ini.getInt("TradePara","TradeType");
	printf("%d\n", gTradeType);
	
	///instrument id
	char* instrumentID = ini.getStr("TradePara","TraderInstrumentID");
	sprintf(req.InstrumentID,"%s",instrumentID);
	printf("%s\n", req.InstrumentID);
	

	///Trade direction:
	char* tradeDirection = ini.getStr("TradePara","TradeDirection");
	req.Direction = tradeDirection[0];
	printf("%c\n", req.Direction);
	
	//limit price
	gLimitPrice = ini.getDouble("TradePara","LimitPrice");
	printf("%lf\n",gLimitPrice);

	//stop price
	gStopPrice = ini.getDouble("TradePara","StopPrice");
	printf("%lf\n",gStopPrice);

	///volume: defult:1
	req.VolumeTotalOriginal = ini.getInt("TradePara","Volume");
	printf("%d\n",req.VolumeTotalOriginal);
	
	///combine offset flag: open
	req.CombOffsetFlag[0] = ini.getStr("TradePara","CombOffsetFlag")[0];
	printf("%c\n", req.CombOffsetFlag[0]);

	///combine hedge flag
	req.CombHedgeFlag[0] = ini.getStr("TradePara","CombHedgeFlag")[0];
	printf("%c\n", req.CombHedgeFlag[0]);
	
	///Minimum volume: 1
	req.MinVolume = ini.getInt("TradePara","MinVolume");
	printf("%d\n", req.MinVolume);

	///Force close reason
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;

	///is auto suspend: not
	req.IsAutoSuspend = 0;
	
	///business unit
	//	TThostFtdcBusinessUnitType	BusinessUnit;
	///requst id
	//	TThostFtdcRequestIDType	RequestID;
	///User force close flag
	req.UserForceClose = 0;
	////////////////same


	////////////////unture
	///contingent condition
	///Immediately
	req.ContingentCondition = ini.getStr("TradePara","CombHedgeFlag")[0];
	printf("%c\n", req.ContingentCondition);
	////////////////unture

	// limit order
	if(gTradeType == 0)
	{
		///order price type: limit price
		req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
		///price
		req.LimitPrice = gLimitPrice;
		///Time condition: 
		req.TimeCondition = THOST_FTDC_TC_GFD;
		///Volume condition
		///type of volume condition
		//THOST_FTDC_VC_AV:any volume
		//THOST_FTDC_VC_MV:minimem volume
		//THOST_FTDC_VC_CV:all the volume
		req.VolumeCondition = THOST_FTDC_VC_AV;
	}
	// market order
	else if(gTradeType == 1)
	{
		///order price type: limit price
		req.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
		// price
		req.LimitPrice = 0;
		// valuid type
		///Time condition: 
		req.TimeCondition = THOST_FTDC_TC_IOC;
		req.VolumeCondition = THOST_FTDC_VC_AV;
	}
	// condition order
	else if (gTradeType == 2)
	{
		///contingent condition, this para can change
		req.ContingentCondition = THOST_FTDC_CC_Immediately;
		//stop price
		//triggered when price falls or rises to this price
		req.StopPrice = gStopPrice;
		req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
		///price
		req.LimitPrice = gLimitPrice;
		///Time condition: 
		req.TimeCondition = THOST_FTDC_TC_GFD;
		req.VolumeCondition = THOST_FTDC_VC_AV;
	}
	//FOK order
	else if(gTradeType == 3)
	{
		///order price type: limit price
		req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
		// price
		req.LimitPrice = gLimitPrice;
		// valuid type
		///Time condition: 
		req.TimeCondition = THOST_FTDC_TC_IOC;
		req.VolumeCondition = THOST_FTDC_VC_CV;
	}
	//FAK order
	else if(gTradeType == 4)
	{
		///order price type: limit price
		req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
		// price
		req.LimitPrice = gLimitPrice;
		// valuid type
		///Time condition: 
		req.TimeCondition = THOST_FTDC_TC_IOC;
		req.VolumeCondition = THOST_FTDC_VC_AV;
	}
	
	///GTD date
	//TThostFtdcDateType	GTDDate;

	///stop price
	//TThostFtdcPriceType	StopPrice;

	cout<<"set req by ini\n";
	return req;
	
}

//request order insert according to para CThostFtdcInputOrderField
void CTraderSpi::ReqOrderInsertBy(CThostFtdcInputOrderField req)
{
	int iResult = pTradeUserApi->ReqOrderInsert(&req, ++iRequestID);
	cout << "iRequestID=" <<iRequestID<<endl;
	cout << "--->>> request order insert : " << iResult << ((iResult == 0) ? ", Succeed" : ", Failed") << endl;
}