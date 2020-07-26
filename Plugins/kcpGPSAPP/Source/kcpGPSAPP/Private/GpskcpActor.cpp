// Fill out your copyright notice in the Description page of Project Settings.


#include "GpskcpActor.h"
#include "Kcpclient.h"
#include "Engine.h"
#include "MobileUtilsBlueprintLibrary.h"

// Sets default values
AGpskcpActor::AGpskcpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}
AGpskcpActor::~AGpskcpActor()
{
	if (mkcp)
	{
		delete mkcp;
		mkcp = nullptr;
	}

}
void AGpskcpActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (mkcp)
	{
		mkcp->exitthread = true;
	}
}
// Called when the game starts or when spawned
void AGpskcpActor::BeginPlay()
{
	Super::BeginPlay();
	mkcp = new Kcpclient();
	mkcp->setserveraddress("192.168.20.96", 8000);
	mkcp->OnkcpReceiveddata.BindLambda([](const uint8* data, uint32 size) {
		FString str = FString(UTF8_TO_TCHAR(data)).Left(size);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, str);

		});
	bool b = UMobileUtilsBlueprintLibrary::javafunctionIsgpsenabled();
	if (b)
	{
	}
	else
	{
		UMobileUtilsBlueprintLibrary::javafunctionGotoSettinggps();
	}
	GetWorld()->GetTimerManager().SetTimer(th, this, &AGpskcpActor::timerworker, 1, true, 1);
}

// Called every frame
void AGpskcpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGpskcpActor::timerworker()
{
	float latitude, longitude;
	UMobileUtilsBlueprintLibrary::javafunctionGPSInfor(latitude,longitude);
	FString androidid = UMobileUtilsBlueprintLibrary::javafunctionAndroidID();
	TCHAR* serializedChar = androidid.GetCharArray().GetData();
	int32 size = FCString::Strlen(serializedChar);
	int floatsize = sizeof(float);

	realdata.SetNum(32+(floatsize*2));
	FMemory::Memcpy(realdata.GetData(), (const uint8*)TCHAR_TO_UTF8(serializedChar), size);//here is 2 byte valide data length
	FMemory::Memcpy(realdata.GetData() + 32, (const uint8*)&latitude, sizeof(float));//here is 2 byte valide data length
	FMemory::Memcpy(realdata.GetData() + 32+ sizeof(float), (const uint8*)&longitude, sizeof(float));//here is 2 byte valide data length

	if (mkcp)
	{
		mkcp->kcpsend(realdata.GetData(), realdata.Num());
	}
}
