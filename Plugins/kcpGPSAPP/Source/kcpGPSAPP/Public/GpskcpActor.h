// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GpskcpActor.generated.h"
//Use USTRUCT(BlueprintType) if you would like to include your Struct in Blueprints
UENUM()
enum class Command : uint8
{
	none=0 ,
	Spawnnewplayer,
};
USTRUCT()
struct FWorldCommand
{
	GENERATED_BODY()
		// Use UPROPERTY() to decorate member variables as they allow for easier integration with network replication as well as potential garbage collection processing
		UPROPERTY()
		Command commandtype;
	UPROPERTY()
	FString payload;
	//UPROPERTY()
	//	float latitude;
	//UPROPERTY()
	//	float longitude;
	//UPROPERTY()
	//	FString channelid;
	FWorldCommand()
	{
	  
	}
};
UCLASS()
class KCPGPSAPP_API AGpskcpActor : public AActor
{
	GENERATED_BODY()
		TMap<FString, int> OnchannelReceivedatacallbackmap;
		//class Kcpclient* mkcp;
	FTimerHandle th;
	void timerworker();
	TArray<uint8> realdata;
	TSharedPtr<class KcpChannel, ESPMode::ThreadSafe>mkcpchannel;
	float latitude = 30.2345;
	float longitude = 119.87655;
public:	
	// Sets default values for this actor's properties
	AGpskcpActor();
	~AGpskcpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
