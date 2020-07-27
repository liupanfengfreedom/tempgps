// Fill out your copyright notice in the Description page of Project Settings.


#include "KcpChannelManager.h"
#include "Kcpclient.h"
#include "Engine.h"

TSharedPtr<KcpChannelManager, ESPMode::ThreadSafe> KcpChannelManager::_msingleston = nullptr;

KcpChannelManager::KcpChannelManager()
{
	mkcpclient = new Kcpclient();
	mkcpclient->setserveraddress("192.168.20.96", 8000);

	mkcpclient->OnkcpReceiveddata.BindLambda([this](const uint8* data, uint32 size) {
		//FString str = FString(UTF8_TO_TCHAR(data)).Left(size);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, str);
		FString id = FString(channelidlength,(char*)data);
		bool bcontain = OnchannelReceivedatacallbackmap.Contains(id);
		if (bcontain)
		{
			OnchannelReceivedatacallbackmap[id]->OnkcpChannelReceivedatadelegate.ExecuteIfBound(data+ channelidlength, size- channelidlength);
		}
	});
}
TSharedPtr<KcpChannelManager, ESPMode::ThreadSafe>  KcpChannelManager::Getsingleston()
{
	if (_msingleston == nullptr)
	{
		_msingleston = MakeShareable(new KcpChannelManager());
	}
	return _msingleston;
}
KcpChannelManager::~KcpChannelManager()
{
	if (mkcpclient)//this seem never be excuted
	{
		mkcpclient->exitthread = true;
	}
	Async(EAsyncExecution::ThreadPool, [=]() {exitkcpthread(); }, nullptr);

}
void KcpChannelManager::exitkcpthread()
{
	FPlatformProcess::Sleep(0.5);
	if (mkcpclient)
	{
		delete mkcpclient;
		mkcpclient = nullptr;
	}
}
KcpChannel::KcpChannel(FString channelidp)
{
	channelid = channelidp;
	kcpchannelmanager = KcpChannelManager::Getsingleston();
	kcpchannelmanager->OnchannelReceivedatacallbackmap.Add(channelid,this);
}
KcpChannel::~KcpChannel()
{
	kcpchannelmanager->OnchannelReceivedatacallbackmap.Remove(channelid);
}
void KcpChannel::send(const uint8* content, const int32& size)
{
	TArray<uint8> tempcontent;
	tempcontent.SetNumZeroed(channelidlength + size);
	TCHAR* arr = channelid.GetCharArray().GetData();
	int sendidlength = FCString::Strlen(arr);
	sendidlength = sendidlength < channelidlength?sendidlength:channelidlength;//avoid over copy
	FMemory::Memcpy(tempcontent.GetData(), (uint8*)TCHAR_TO_UTF8(arr), sendidlength);
	FMemory::Memcpy(tempcontent.GetData() + channelidlength, content, size);
	kcpchannelmanager->mkcpclient->kcpsend(tempcontent.GetData(), tempcontent.Num());
}
void KcpChannel::send(FString& serialized)
{
	FString temp = channelid+ serialized;
	kcpchannelmanager->mkcpclient->kcpsend(temp);
}
