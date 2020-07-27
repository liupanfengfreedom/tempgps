// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
DECLARE_DELEGATE_TwoParams(FOnKcpChannelReceivedata, const uint8*, uint32);
/**
 * 
 */
#define channelidlength 32
class KCPUL_API KcpChannelManager
{
	friend class KcpChannel;
	class Kcpclient* mkcpclient;
	static TSharedPtr<KcpChannelManager, ESPMode::ThreadSafe> _msingleston;
	TMap<FString, class KcpChannel*> OnchannelReceivedatacallbackmap;
	void exitkcpthread();
public:
	KcpChannelManager();
	~KcpChannelManager();
	static TSharedPtr<KcpChannelManager, ESPMode::ThreadSafe> Getsingleston();

};

class KCPUL_API KcpChannel
{
	friend class KcpChannelManager;
	FString channelid;
	TSharedPtr<class KcpChannelManager, ESPMode::ThreadSafe> kcpchannelmanager;
public:
	KcpChannel(FString channelidp);
	~KcpChannel();
	FOnKcpChannelReceivedata OnkcpChannelReceivedatadelegate;
	void send(const uint8* content, const int32& size);
	void send(FString& serialized);

};
