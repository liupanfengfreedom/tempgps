// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "Kcpclient.h"
#include "Engine.h"
// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}
AMyActor::~AMyActor()
{
	if (mkcp)
	{
		delete mkcp;
		mkcp = nullptr;
	}
}
void AMyActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (mkcp)
	{
		mkcp->exitthread = true;
	}
}
// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	mkcp = new Kcpclient();
	mkcp->setserveraddress("192.168.20.96",8000);
	mkcp->OnkcpReceiveddata.BindLambda([](const uint8* data, uint32 size) {
		FString str = FString(UTF8_TO_TCHAR(data)).Left(size);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, str);

		});

}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//if (counter++ > 1000)
	//{
	//	if (mkcp)
	//	{
	//		mkcp->exitthread = true;
	//	}
	//}
	//if (counter++ > 1500)
	//{
	//	if (mkcp)
	//	{
	//		delete mkcp;
	//		mkcp = nullptr;
	//	}
	//}
}
void AMyActor::fv1()
{ 
	FString str = "hi kcp this is a message from unrealengine!";
	FString strpayload = ""; 
	for (int i = 0; i < 1200; i++)
	{
		int j = i % 10;
		strpayload += FString::FromInt(j);
	}
	if (mkcp)
	{
		mkcp->kcpsend(strpayload);
	}
}
