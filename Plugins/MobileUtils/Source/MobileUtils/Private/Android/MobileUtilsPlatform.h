// Mobile Utils Plugin
// Created by Patryk Stepniewski
// Copyright (c) 2014-2019 gameDNA Ltd. All Rights Reserved.

#pragma once

#include "Interfaces/MobileUtilsInterface.h"
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#include <android_native_app_glue.h>
#include "EngineMinimal.h"
#include "Core.h"

class FMobileUtilsPlatform : public IMobileUtilsInterface
{
public:
	FMobileUtilsPlatform();
	virtual ~FMobileUtilsPlatform();

	virtual void SetOrientation(int orientation) override;
	virtual void StartActivity(const FString& activity) override;
	virtual FString ConvertToAbsolutePath(const FString& path) override;

	virtual FString javafunc(const FString& activity) override;
	virtual void javafuncv1(const FString& activity) override;
	virtual void javatoast(const FString& activity) override;
	virtual float getbattery() override;
	virtual FString GetAndroidID() override;
	virtual int NetworkconnectionInfor() override;
	virtual void GPSInfor(float &a, float &b) override;
	virtual bool Isgpsenabled() override;
	virtual void GotoSettinggps() override;

	// JNI Methods
	static jmethodID SetOrientationMethod;
	static jmethodID StartActivityMethod;
	static jmethodID javafuncMethod;
	static jmethodID javatoastMethod;
	static jmethodID javabatteryMethod;
	static jmethodID javaGetAndroidIDMethod;
	static jmethodID javaNetInforMethod;
	static jmethodID javaGPSlatitudeMethod;
	static jmethodID javaGPSlongitudeMethod;
	static jmethodID javaGPSIsgpsenabled;
	static jmethodID javaGPSGotoSettinggps;


	
};
