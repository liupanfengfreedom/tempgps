// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PosCharacter.generated.h"

UCLASS()
class KCPGPSAPP_API APosCharacter : public ACharacter
{
	GENERATED_BODY()
		TSharedPtr<class KcpChannel, ESPMode::ThreadSafe>mkcpchannel;
	FVector targetlocation;
	FVector direction;
public:
	// Sets default values for this character's properties
	APosCharacter();
	void Addkcpchannel(const TSharedPtr<class KcpChannel, ESPMode::ThreadSafe> & kcpchannelp);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
