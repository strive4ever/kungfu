//
// Created by qlu on 2019/1/10.
//

#ifndef KUNGFU_MDGATEWAY_H
#define KUNGFU_MDGATEWAY_H

#include "ThostFtdcMdApi.h"
#include "../../impl/gateway_impl.h"

namespace kungfu
{
    namespace ctp
    {
        class MdGateway: public CThostFtdcMdSpi, public kungfu::MdGatewayImpl
        {
        public:
            MdGateway(const std::string& front_uri, const std::string& broker_id, const std::string& account_id, const std::string& password, int log_level):
                    kungfu::MdGatewayImpl(SOURCE_CTP, log_level),front_uri_(front_uri), broker_id_(broker_id), account_id_(account_id), password_(password) {}
            virtual ~MdGateway() {};

            virtual void init();
            virtual void start();

            virtual bool subscribe(const std::vector<Instrument>& instruments, bool is_level2 = false);
            virtual bool unsubscribe(const std::vector<Instrument>& instruments);

            bool subscribe(const std::vector<std::string>& instrument_ids);
            bool unsubscribe(const std::vector<std::string>& instrument_ids);

            bool login();

            ///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
            virtual void OnFrontConnected();

            ///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
            ///@param nReason 错误原因
            ///        0x1001 网络读失败
            ///        0x1002 网络写失败
            ///        0x2001 接收心跳超时
            ///        0x2002 发送心跳失败
            ///        0x2003 收到错误报文
            virtual void OnFrontDisconnected(int nReason);

            ///登录请求响应
            virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

            ///登出请求响应
            virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

            ///订阅行情应答
            virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

            ///取消订阅行情应答
            virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

            ///深度行情通知
            virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

        private:
            std::string front_uri_;
            std::string broker_id_;
            std::string account_id_;
            std::string password_;

            int request_id_;

            CThostFtdcMdApi* api_;

            static const std::unordered_map<int, std::string> kDisconnectedReasonMap;

        };
    }
}
#endif //KUNGFU_MDGATEWAY_H
