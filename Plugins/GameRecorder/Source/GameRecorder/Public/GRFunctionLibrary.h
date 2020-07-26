// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GRFunctionLibrary.generated.h"
UENUM(BlueprintType)
enum Filetype
{
	None = 0 UMETA(Hidden),
	start     UMETA(DisplayName = "startrecord"),
	stop   UMETA(DisplayName = "stopandshare"),
};
/**
 * 
 */
UCLASS()
class GAMERECORDER_API UGRFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
		UFUNCTION(BlueprintCallable, Category = "AndroidNative")
		static void Recordegame(Filetype type);
};
