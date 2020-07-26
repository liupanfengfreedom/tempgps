// Mobile Utils Plugin
// Created by Patryk Stepniewski
// Copyright (c) 2014-2019 gameDNA Ltd. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"
#include "Core.h"

class IMobileUtilsInterface
{
public:
	virtual void SetOrientation(int orientation) = 0;

	virtual void StartActivity(const FString& activity) = 0;
	virtual FString ConvertToAbsolutePath(const FString& path) = 0;

	virtual FString javafunc(const FString& activity) = 0;
	virtual void javafuncv1(const FString& activity) = 0;
	virtual void javatoast(const FString& activity) = 0;
	virtual float getbattery()=0;
	virtual FString GetAndroidID()=0;
	virtual int NetworkconnectionInfor()=0;
	virtual void GPSInfor(float &a, float &b) = 0;
	virtual bool Isgpsenabled() = 0;
	virtual void GotoSettinggps()=0;
};
