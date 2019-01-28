//#include "DxClient.h"
//#include "Worker.h"
//#include "../../include/server/ClientServer.h"
//
//
//
//void DxClientRecv::PushData(const int& size) noexcept
//{
//	//memcpy(m_packetBuffer + m_pushPos, data, size);
//	m_pushPos += size;
//
//	// 헤더를 못받았더라도 기본적으로 0으로 처리되므로 문제 X
//	m_pUPacket = (UPACKET*)m_packetBuffer;
//	// 남은 데이터가 바꿀 패킷 길이보다 크다면
//	while (m_pUPacket->header.length <= m_pushPos)
//	{
//		// 패킷 완성
//		m_pPacket = ClientServer::GetInstance().TakePacket();
//		memcpy(m_pPacket, m_packetBuffer + m_popPos, m_pUPacket->header.length);
//		// 패킷 처리(+줄바꿈)
//		if (auto length = strlen(m_pPacket->msg);  m_pPacket->msg[length - 1] != '\n')
//			m_pPacket->msg[length] = '\n';
//		ClientServer::Get().m_PacketPool.push(m_pPacket);
//		// 버퍼 기준 정리
//		m_popPos += m_pUPacket->header.length;				// 버퍼 기준점
//		m_pushPos -= m_pUPacket->header.length;				// 패킷 길이만큼 감소
//		m_pUPacket = (UPACKET*)&m_packetBuffer[m_popPos];	// 다음 패킷 기준
//		if (m_pushPos == 0 || m_pUPacket->header.length == 0)
//			break;
//	}
//	// 남은 버퍼 땡기기
//	memcpy(m_packetBuffer, m_packetBuffer + m_popPos, MaxBufferSize - m_popPos);
//	ZeroMemory(m_packetBuffer + MaxBufferSize - m_popPos, m_popPos);
//	m_popPos = 0;
//}
//
//bool DxClientRecv::Run()
//{
//	{
//		std::lock_guard<std::mutex> guarder(m_Mutex);
//		cout << __FUNCTION__ << " -> 연결 되었습니다.\n";
//	}
//	// 클라 소켓 연결
//	ClientServer::Get().m_ClientSock = m_ListenSocket;
//
//	UPACKET sendPacket = { 0, };
//	std::this_thread::sleep_for(std::chrono::seconds(10));
//	sendPacket.msg[0] = 'A';
//	sendPacket.header.length = (WORD)(strlen(sendPacket.msg) + PACKET_HEADER_SIZE);
//	sendPacket.header.type = EPacket::CHAT_NAME_ACK;
//	send(m_ListenSocket, (char*)&sendPacket, sendPacket.header.length, 0);
//
//	while (m_isCreated)
//	{
//		// 서버->리시브
//		int receiveByte = 0;
//		receiveByte = recv(m_ListenSocket, (char*)&m_packetBuffer + m_pushPos, MaxBufferSize / 2, 0);
//		if (ByteErrorCheck(receiveByte))
//		{
//			Server::ErrorMessage(""s + __FUNCTION__ + " -> DxClient Recv Error!");
//			m_isCreated = false;
//			break;
//		}
//		else
//		{
//			// 패킷 처리
//			std::lock_guard<std::mutex> guarder(m_Mutex);
//			PushData(receiveByte);
//		}
//		std::this_thread::yield();
//	}
//	shutdown(m_ListenSocket, SD_SEND);
//	closesocket(m_ListenSocket);
//	Server::ErrorMessage(""s + __FUNCTION__ + " -> Thread Exit!");
//	return false;
//}
//
//bool DxClientSend::Start(const USHORT& portNum, const string_view& address, const bool& isClient)
//{
//	std::this_thread::sleep_for(std::chrono::milliseconds(1));
//	if (ClientServer::Get().m_ClientSock == 0)
//	{
//		Server::ErrorMessage(""s + __FUNCTION__ + " -> DxClient Socket 0!");
//		return false;
//	}
//	m_ListenSocket = ClientServer::Get().m_ClientSock;
//	// 쓰레드 가동
//	CreateThread();
//	return true;
//	portNum; address; isClient;
//}
//
//// 패킷 스트림화?
//bool DxClientSend::Run()
//{
//	{
//		std::lock_guard<std::mutex> guarder(m_Mutex);
//		cout << __FUNCTION__ << " -> 연결 되었습니다.\n";
//	}
//
//	UPACKET* pPacket = nullptr;
//	while (m_isCreated)
//	{
//		// 서버->샌드
//		{
//			std::lock_guard<std::mutex> guarder(m_Mutex);
//			while (!ClientServer::Get().m_SendPacketPool.empty())
//			{
//				pPacket = ClientServer::Get().m_SendPacketPool.front();
//				int sendByte = send(m_ListenSocket, (char*)pPacket, pPacket->header.length, 0);
//				if (ByteErrorCheck(sendByte))
//				{
//					Server::ErrorMessage(""s + __FUNCTION__ + " -> Server DxClient Send Error!");
//					break;
//				}
//				// + 서버 있으면 서버로 보내고, 없으면 바로 패킷풀로
//				ClientServer::Get().m_SendPacketPool.pop();
//				ClientServer::Get().m_DisablePacketPool.push(pPacket);
//			}
//		}
//		std::this_thread::yield();
//	}
//	shutdown(m_ListenSocket, SD_SEND);
//	closesocket(m_ListenSocket);
//	Server::ErrorMessage(""s + __FUNCTION__ + " -> Thread Exit!");
//	return false;
//}