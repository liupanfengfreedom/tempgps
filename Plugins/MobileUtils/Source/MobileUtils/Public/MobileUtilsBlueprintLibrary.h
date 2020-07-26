// Mobile Utils Plugin
// Created by Patryk Stepniewski
// Copyright (c) 2014-2019 gameDNA Ltd. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "MobileUtilsBlueprintLibrary.generated.h"

UCLASS()
class MOBILEUTILS_API UMobileUtilsBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, Category = MobileUtils)
	static void SetOrientation(int orientation);
	
	UFUNCTION(BlueprintCallable, Category = MobileUtils)
	static void StartActivity(const FString& activity);
	UFUNCTION(BlueprintCallable, Category = MobileUtils)
		static FString ConvertToAbsolutePath(const FString& path);
	UFUNCTION(BlueprintCallable, Category = MobileUtils)
		static FString javafunc(const FString& activity);

	UFUNCTION(BlueprintCallable, Category = MobileUtils)
		static void javafunctionv1(const FString& activity);
	UFUNCTION(BlueprintCallable, Category = MobileUtils)
		static void javafunctiontoast(const FString& activity);
	UFUNCTION(BlueprintCallable, Category = MobileUtils)
		static float javafunctionbattery();
	UFUNCTION(BlueprintCallable, Category = MobileUtils)
		static FString javafunctionAndroidID();
	UFUNCTION(BlueprintCallable, Category = MobileUtils)
		static int javafunctionNetInfor();
	UFUNCTION(BlueprintCallable, Category = MobileUtils)
		static void javafunctionGPSInfor(float &Lat, float &Long);
	UFUNCTION(BlueprintCallable, Category = MobileUtils)
		static bool javafunctionIsgpsenabled();
	UFUNCTION(BlueprintCallable, Category = MobileUtils)
		static void javafunctionGotoSettinggps();
};
