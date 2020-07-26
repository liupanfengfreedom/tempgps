// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ikcp.h"

/**
 * 
 */
//DECLARE_DELEGATE_OneParam(FOnUdpClientReceivedata, const TArray<uint8>&);
DECLARE_DELEGATE_TwoParams(FOnKcpReceivedata, const uint8*, uint32);
DECLARE_DELEGATE_FourParams(FOnRawsend, const char*, int, struct IKCPCB*, void*);
class KCP_API Kcpclient
{
	TSharedPtr<class FInternetAddr>	RemoteAddr;
	class FSocket* SenderSocket;
	void ReceiveWork();
	uint32 datasize;
	TArray<uint8> datareceive;
	ikcpcb* kcp1;
	static FOnRawsend onrawsendevent;
	uint8 kcpreceive[1024*4] = { 0 };	
public:
	Kcpclient();
	~Kcpclient();
	bool exitthread = false;
	void setserveraddress(FString ipaddress, int32 port);
	void kcpsend(const uint8* buffer, int len);
	void kcpsend( FString &msg);
	//FOnUdpClientReceivedata OnUdpClientReceiveddata;
	FOnKcpReceivedata OnkcpReceiveddata;
};
