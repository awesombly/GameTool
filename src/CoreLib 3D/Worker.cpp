//#include "Worker.h"
////#include "ChattingBox.h"
//#include "../../include/server/MyPacket.h"
//#include "../../include/server/ClientServer.h"
//
//
//
//bool Worker::Start(const USHORT& portNum, const string_view& address, const bool& isClient)
//{
//	std::this_thread::sleep_for(std::chrono::milliseconds(1));
//	// 쓰레드 가동
//	CreateThread();
//	return true;
//	portNum; address; isClient;
//}
//
//bool Worker::Run()
//{
//	{
//		std::lock_guard<std::mutex> guarder(m_Mutex);
//		cout << __FUNCTION__ << " -> 연결 되었습니다.\n";
//	}
//	while (m_isCreated)
//	{
//		{
//			std::lock_guard<std::mutex> guarder(m_Mutex);
//			while (!ClientServer::Get().m_PacketPool.empty())
//			{
//				ProcessPacket(ClientServer::Get().m_PacketPool.front());
//			}
//		}
//		std::this_thread::yield();
//	}
//	Server::ErrorMessage(""s + __FUNCTION__ + " -> Thread Exit!");
//	closesocket(m_ListenSocket);
//	return true;
//}
//
//void Worker::ProcessPacket(UPACKET* pPacket)
//{
//	switch (pPacket->header.type)
//	{
//	case EPacket::CHAT_MSG:
//	{
//	}	break;
//	default:
//	{
//
//	}	break;
//	}
//	//ChattingBox::m_msgList.emplace_back(ClientServer::CharToWChar(pPacket->msg));
//
//	ClientServer::Get().m_PacketPool.pop();
//	ClientServer::Get().m_DisablePacketPool.push(pPacket);
//}