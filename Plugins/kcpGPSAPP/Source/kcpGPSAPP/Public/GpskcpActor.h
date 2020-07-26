// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GpskcpActor.generated.h"

UCLASS()
class KCPGPSAPP_API AGpskcpActor : public AActor
{
	GENERATED_BODY()
		class Kcpclient* mkcp;
	FTimerHandle th;
	void timerworker();
	TArray<uint8> realdata;
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
