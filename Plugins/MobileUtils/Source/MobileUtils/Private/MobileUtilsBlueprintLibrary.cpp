// Mobile Utils Plugin
// Created by Patryk Stepniewski
// Copyright (c) 2014-2019 gameDNA Ltd. All Rights Reserved.

#include "MobileUtilsBlueprintLibrary.h"
#include "IMobileUtils.h"

UMobileUtilsBlueprintLibrary::UMobileUtilsBlueprintLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMobileUtilsBlueprintLibrary::SetOrientation(int orientation)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	IMobileUtils::Get().GetPlatformInterface()->SetOrientation(orientation);
#endif
}

void UMobileUtilsBlueprintLibrary::StartActivity(const FString& activity)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	IMobileUtils::Get().GetPlatformInterface()->StartActivity(activity);
#endif
}
FString UMobileUtilsBlueprintLibrary::ConvertToAbsolutePath(const FString& path)
{
	return IMobileUtils::Get().GetPlatformInterface()->ConvertToAbsolutePath(path);
}
FString UMobileUtilsBlueprintLibrary::javafunc(const FString& activity)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	return IMobileUtils::Get().GetPlatformInterface()->javafunc(activity);
#endif
	return "";
}
void UMobileUtilsBlueprintLibrary::javafunctionv1(const FString& activity)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	 IMobileUtils::Get().GetPlatformInterface()->javafuncv1(activity);
#endif
	
}
void UMobileUtilsBlueprintLibrary::javafunctiontoast(const FString& activity)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	IMobileUtils::Get().GetPlatformInterface()->javatoast(activity);
#endif

}
float UMobileUtilsBlueprintLibrary::javafunctionbattery()
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	return IMobileUtils::Get().GetPlatformInterface()->getbattery();
#endif
	return 0;
}
FString UMobileUtilsBlueprintLibrary::javafunctionAndroidID()
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	return IMobileUtils::Get().GetPlatformInterface()->GetAndroidID();
#endif
	return "";
}
int UMobileUtilsBlueprintLibrary::javafunctionNetInfor()
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	return IMobileUtils::Get().GetPlatformInterface()->NetworkconnectionInfor();
#endif
	return 0;
}
void UMobileUtilsBlueprintLibrary::javafunctionGPSInfor(float &Lat, float &Long)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	 IMobileUtils::Get().GetPlatformInterface()->GPSInfor(Lat, Long);
#endif

}
bool UMobileUtilsBlueprintLibrary::javafunctionIsgpsenabled()
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	return IMobileUtils::Get().GetPlatformInterface()->Isgpsenabled();
#endif
	return false;
}
void UMobileUtilsBlueprintLibrary::javafunctionGotoSettinggps()
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	IMobileUtils::Get().GetPlatformInterface()->GotoSettinggps();
#endif

}