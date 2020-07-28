// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LocalCharacter.generated.h"

UCLASS()
class KCPGPSAPP_API ALocalCharacter : public ACharacter
{
	GENERATED_BODY()
		FTimerHandle th;
	void timerworker();
	TArray<uint8> realdata;
	TSharedPtr<class KcpChannel, ESPMode::ThreadSafe>mkcpchannel;
	float latitude = 30.29082;
	float longitude = 119.99529;
public:
	// Sets default values for this character's properties
	ALocalCharacter();
	UPROPERTY(EditDefaultsOnly, Category = ROBOT)
		TSubclassOf<class APosCharacter> Robotclass;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
