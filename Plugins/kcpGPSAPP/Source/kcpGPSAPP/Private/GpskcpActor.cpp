// Fill out your copyright notice in the Description page of Project Settings.


#include "GpskcpActor.h"
#include "Kcpclient.h"
#include "Engine.h"
#include "MobileUtilsBlueprintLibrary.h"
#include "KcpChannelManager.h"
#include "JsonUtilities.h"
#include "Json.h"
#include "MessageManager.h"

// Sets default values
AGpskcpActor::AGpskcpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}
AGpskcpActor::~AGpskcpActor()
{
	//if (mkcp)
	//{
	//	delete mkcp;
	//	mkcp = nullptr;
	//}
}
void AGpskcpActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//if (mkcp)
	//{
	//	mkcp->exitthread = true;
	//}
	GetWorld()->GetTimerManager().ClearTimer(th);
	mkcpchannel->exit();

}
// Called when the game starts or when spawned
void AGpskcpActor::BeginPlay()
{
	OnchannelReceivedatacallbackmap.Add("s1", 1);
	OnchannelReceivedatacallbackmap.Add("s2", 2);
	OnchannelReceivedatacallbackmap.Add("s1", 3);
	Super::BeginPlay();
	//mkcp = new Kcpclient();
	//mkcp->setserveraddress("192.168.20.96", 8000);
	//mkcp->OnkcpReceiveddata.BindLambda([](const uint8* data, uint32 size) {
	//	FString str = FString(UTF8_TO_TCHAR(data)).Left(size);
	//	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, str);
	//	});

	//bool b = UMobileUtilsBlueprintLibrary::javafunctionIsgpsenabled();
	//if (b)
	//{
	//}
	//else
	//{
	//	UMobileUtilsBlueprintLibrary::javafunctionGotoSettinggps();
	//}
	int32 id = FMath::Rand();
	FString androidid = "aassddff1122aaddss"; 
	androidid = FString::FromInt(id);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "thisid : "+ androidid);

	//androidid = UMobileUtilsBlueprintLibrary::javafunctionAndroidID();
	mkcpchannel = MakeShareable(new KcpChannel(androidid));
	mkcpchannel->OnkcpChannelReceivedatadelegate.BindLambda([](const uint8*data, uint32 size) {
		FString rec = FString(size,(char*)data);
		TSharedPtr<FJsonObject> jsonobject = MakeShareable(new FJsonObject);
		TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(rec);
		FJsonSerializer::Deserialize(Reader, jsonobject);
		int Command = jsonobject->GetIntegerField("Command");
		if (Command == 1)//create new player
		{
			FString channelid = jsonobject->GetStringField("channelid");
			float latitude = jsonobject->GetNumberField("latitude");
			float longitude = jsonobject->GetNumberField("longitude");


			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, channelid + "  : "+ "latitude: "+ FString::SanitizeFloat(latitude)+ "longitude: " + FString::SanitizeFloat(longitude));
			//create player
		}
	});
	GetWorld()->GetTimerManager().SetTimer(th, this, &AGpskcpActor::timerworker, 1, true, 1);
	ADDMESSAGELISTEN(this, "1", [=](const void* p) {
		     FString str = *(FString*)p;
			 latitude = FCString::Atof(*str);
		})
	ADDMESSAGELISTEN(this, "2", [=](const void* p) {
			FString str = *(FString*)p;
			longitude = FCString::Atof(*str);
		})
}

// Called every frame
void AGpskcpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGpskcpActor::timerworker()
{
	//float latitude, longitude;
	//UMobileUtilsBlueprintLibrary::javafunctionGPSInfor(latitude,longitude);
	//FString androidid = UMobileUtilsBlueprintLibrary::javafunctionAndroidID();
	//TCHAR* serializedChar = androidid.GetCharArray().GetData();
	//int32 size = FCString::Strlen(serializedChar);
	//int floatsize = sizeof(float);

	//realdata.SetNum(32+(floatsize*2));
	//FMemory::Memcpy(realdata.GetData(), (const uint8*)TCHAR_TO_UTF8(serializedChar), size);//here is 2 byte valide data length
	//FMemory::Memcpy(realdata.GetData() + 32, (const uint8*)&latitude, sizeof(float));//here is 2 byte valide data length
	//FMemory::Memcpy(realdata.GetData() + 32+ sizeof(float), (const uint8*)&longitude, sizeof(float));//here is 2 byte valide data length
	realdata.Empty();
	for (int i = 1; i < 10; i++)
	{
		realdata.Add(i);
	}
	TSharedPtr<FJsonObject> newplayerjoin = MakeShareable(new FJsonObject);
	newplayerjoin->SetNumberField("Command", 1);
	newplayerjoin->SetNumberField("latitude", latitude);
	newplayerjoin->SetNumberField("longitude", longitude);
	//newplayerjoin->SetNumberField("latitude", 30.2345);
	//newplayerjoin->SetNumberField("longitude", 119.87655);
	FString OutputString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(newplayerjoin.ToSharedRef(), Writer);
	mkcpchannel->send(OutputString);
	
}
