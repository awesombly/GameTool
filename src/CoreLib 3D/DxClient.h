//#pragma once
//#include "../../include/server/ServerBasic.h"
//#include "../../include/server/MyProtocol.h"
//
//// + Sender에서 서버로 패킷 보내고 Recver에서 받아서 패킷풀로 보내고, Worker에서 처리
//constexpr WORD MaxBufferSize = 10000;
//
//class DxClientRecv : public ServerBasic
//{
//protected:
//	UPACKET*  m_pUPacket = nullptr;		// 기준 확인용 포인터
//	UPACKET*  m_pPacket = nullptr;		// 전송할 패킷
//	int		  m_popPos = 0;		// = 버퍼 땡길때 기준
//	char	  m_packetBuffer[MaxBufferSize] = { 0, };
//
//	//WSABUF	  m_WSABuffer = { MaxBufferSize, m_packetBuffer };
//	int		  m_pushPos = 0;			// 남은 데이터 양
//	//WORD & m_MsgType;
//protected:
//	bool Run() override;
//	void PushData(const int& size) noexcept;
//public:
//	//DxClientRecv(WORD& msgType) : m_MsgType(msgType) {};
//	DxClientRecv() = default;
//	virtual ~DxClientRecv() = default;
//};
//
//class DxClientSend : public ServerBasic
//{
//protected:
//	//WORD & m_MsgType;
//	bool Run() override;
//public:
//	//DxClientSend(WORD& msgType) : m_MsgType(msgType) {};
//	bool Start(const USHORT& portNum, const string_view& address, const bool& isClient = false) override;
//	DxClientSend() = default;
//	virtual ~DxClientSend() = default;
//};