#pragma once
#include "ThostFtdcTraderApi.h"
#include <string>
using namespace std;

class CTraderSpi : public CThostFtdcTraderSpi
{
public:

	CTraderSpi(char* filePath);

	// 
	~CTraderSpi();

	/// Call it when the client connected with the backstage(before login)
	virtual void OnFrontConnected();

	/// Call it when the client disconnected with the backstage. 
	/// API will auto reconnect
	/// @param nReason
	///			0x1001 network reading failure
	///			0x1002 network writing failure
	///			0x2001 receiving heartbeat failure
	///			0x2002 sending heartbeat failure
	///			0x2003 wrong message package received
	virtual void OnFrontDisconnected(int nReason);

	/// Response to the login requst
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	/// Response to investor settlement results confirm 
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	/// Response to query instrument request
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	/// Response to query trading account request
	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	/// Response to the query investor position
	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	/// Response to the declaration entry request 
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	/// Response to executive entry request
	//virtual void OnRspExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	/// Response to the inquiry inserting request
	//virtual void OnRspForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	/// Response to quote inserting request
	//virtual void OnRspQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	/// Response to order action request
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	/// Response to execute order action request
	//virtual void OnRspExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	/// Response to quotation action request
	//virtual void OnRspQuoteAction(CThostFtdcInputQuoteActionField *pInputQuoteAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	/// Response to the error
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		
	/// Heartbeat overtime warning. Call it when wait to receive package overtime
	/// @param nTimeLapse interval of last time received message
	virtual void OnHeartBeatWarning(int nTimeLapse);
	
	/// Order information returned
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);

	/// Order execution information returned
	//virtual void OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder);

	/// Inquiry information returned
	//virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp);

	/// Quote information returned
	//virtual void OnRtnQuote(CThostFtdcQuoteField *pQuote);
	
	/// Trade information returned
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);


	//get the CThostFtdcInputOrderField according to trade type
	// gTradeType = 0 :limit order
	// gTradeType = 1 :market order
	// gTradeType = 2 :condition order
	// gTradeType = 3 :FOK order
	// gTradeType = 4 :FAK order
	CThostFtdcInputOrderField GetOrderField(int gTradeType);

	// read order config file and get the CThostFtdcInputOrderField to sent order
	CThostFtdcInputOrderField ReadOrderFieldIni(char* filePath);

	//request order insert according to para CThostFtdcInputOrderField
	void ReqOrderInsertBy(CThostFtdcInputOrderField req);

//private:
	/// Request user login 
	void ReqUserLogin();
	/// Request investor settlement information confirm 
	void ReqSettlementInfoConfirm();
	/// Request query instrument 
	void ReqQryInstrument();
	/// Request query trading account
	void ReqQryTradingAccount();
	/// Request query investor position
	void ReqQryInvestorPosition();
	/// Request order insert 
	void ReqOrderInsert();
	/// Request execate order insert
	void ReqExecOrderInsert();
	/// Request for quote insert
	void ReqForQuoteInsert();
	/// Request quote insert 
	void ReqQuoteInsert();
	/// Request order action
	void ReqOrderAction(CThostFtdcOrderField *pOrder);
	/// Request execute order action
	void ReqExecOrderAction(CThostFtdcExecOrderField *pExecOrder);
	/// Request quote action
	void ReqQuoteAction(CThostFtdcQuoteField *pQuote);

	// initrialize api
	void InitApi()
	{
		pTradeUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
	    pTradeUserApi->RegisterSpi((CThostFtdcTraderSpi*) this);
	    pTradeUserApi->SubscribePublicTopic(THOST_TERT_QUICK);
	    pTradeUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);
	    pTradeUserApi->RegisterFront(gTradeFrontAddr);
	    pTradeUserApi->Init();
	    cout<<"InitApi\n";
	}

	// api join
	void JoinApi();

	// Api pointer
	CThostFtdcTraderApi* pTradeUserApi = NULL;

	// state flag
	bool isFrontConnected = 0;
	bool isLogin = 0;
	bool isConfirm = 0;

private:
	/// Receive a successful response or not
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);
	/// Receive the response to my order or not 
	bool IsMyOrder(CThostFtdcOrderField *pOrder);
	/// Receive the response to my execute order or not 
	bool IsMyExecOrder(CThostFtdcExecOrderField *pExecOrder);
	/// Receive my quote or not
	bool IsMyQuote(CThostFtdcQuoteField *pQuote);
	/// Receive order that is trading or not
	bool IsTradingOrder(CThostFtdcOrderField *pOrder);	
	/// The announcement of unrevoked execution
	bool IsTradingExecOrder(CThostFtdcExecOrderField *pExecOrder);
	/// An unrevoked trading quote or not
	bool IsTradingQuote(CThostFtdcQuoteField *pQuote);

	

	// Trader spi
	//char gTradeFrontAddr[] = "tcp://180.168.146.187:10000";//10030 10000
	string sTradeFrontAddr;
	char* gTradeFrontAddr;
	TThostFtdcBrokerIDType gBrokerID ;		//code of the broker
	TThostFtdcInvestorIDType gInvestorID ;
	TThostFtdcPasswordType gInvestorPassword ;



	//request id
	int iRequestID = 0;

	// configure used in orderinserting
	TThostFtdcInstrumentIDType gTraderInstrumentID;		//instrument id
	TThostFtdcPriceType gLimitPrice; //limit price
	TThostFtdcPriceType gStopPrice; //stop price
	int gTradeType;// trade request type;
	// int gVolume; //volume
	// TThostFtdcDirectionType gTradeDirection;//Trading direction

	// session parameters 
	TThostFtdcFrontIDType frontID; 		//front id
	TThostFtdcSessionIDType	sessionID;		//session id
	TThostFtdcOrderRefType	gOrderRef;		//order reference
	TThostFtdcOrderRefType	exeOrderRef;	//execute order reference 
	TThostFtdcOrderRefType	forquoteRef;	//for quote reference
	TThostFtdcOrderRefType	quoteRef;		//quote reference
};