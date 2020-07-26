// Mobile Utils Plugin
// Created by Patryk Stepniewski
// Copyright (c) 2014-2019 gameDNA Ltd. All Rights Reserved.

#include "MobileUtilsPlatform.h"

jmethodID FMobileUtilsPlatform::SetOrientationMethod;
jmethodID FMobileUtilsPlatform::StartActivityMethod;
jmethodID FMobileUtilsPlatform::javafuncMethod;
jmethodID FMobileUtilsPlatform::javatoastMethod;
jmethodID FMobileUtilsPlatform::javabatteryMethod;
jmethodID FMobileUtilsPlatform::javaGetAndroidIDMethod;
jmethodID FMobileUtilsPlatform::javaNetInforMethod;
jmethodID FMobileUtilsPlatform::javaGPSlatitudeMethod;
jmethodID FMobileUtilsPlatform::javaGPSlongitudeMethod;
jmethodID FMobileUtilsPlatform::javaGPSIsgpsenabled;
jmethodID FMobileUtilsPlatform::javaGPSGotoSettinggps;


FMobileUtilsPlatform::FMobileUtilsPlatform()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		SetOrientationMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_SetOrientation", "(I)V", false);
		StartActivityMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_StartActivity", "(Ljava/lang/String;)V", false);
		javafuncMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_javafuncMethod", "(Ljava/lang/String;)Ljava/lang/String;", false);
		javatoastMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_javafuncv1Method", "(Ljava/lang/String;)V", false);
		javabatteryMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_GetBatteryPct", "()F", false);
		javaGetAndroidIDMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_GetAndroidID", "()Ljava/lang/String;", false);
		javaNetInforMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_getActiveNetworkInfo", "()I", false);
		javaGPSlatitudeMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_GetGPSlatitude", "()D", false);
		javaGPSlongitudeMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_GetGPSlongitude", "()D", false);
		javaGPSIsgpsenabled = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_IsGpsEnabled", "()Z", false);
		javaGPSGotoSettinggps = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_GotoSetingGps", "()V", false);

	}
}

FMobileUtilsPlatform::~FMobileUtilsPlatform()
{
}

void FMobileUtilsPlatform::SetOrientation(int orientation)
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, FMobileUtilsPlatform::SetOrientationMethod, orientation);
	}
}

void FMobileUtilsPlatform::StartActivity(const FString& activity)
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jstring JStr = Env->NewStringUTF(TCHAR_TO_UTF8(*activity));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, FMobileUtilsPlatform::StartActivityMethod, JStr);
		Env->DeleteLocalRef(JStr);
	}
}
FString FMobileUtilsPlatform::javafunc(const FString& activity)
{
	FString returnvalue;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jstring JStr = Env->NewStringUTF(TCHAR_TO_UTF8(*activity));
		jstring ResultDeviceIdString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, FMobileUtilsPlatform::javafuncMethod, JStr);
		const char *nativeDeviceIdString = Env->GetStringUTFChars(ResultDeviceIdString, 0);
		returnvalue = FString(nativeDeviceIdString);
		Env->ReleaseStringUTFChars(ResultDeviceIdString, nativeDeviceIdString);
		Env->DeleteLocalRef(ResultDeviceIdString);
		Env->DeleteLocalRef(JStr);

	}
	return returnvalue;
}
void FMobileUtilsPlatform::javafuncv1(const FString& activity)
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jstring JStr = Env->NewStringUTF(TCHAR_TO_UTF8(*activity));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, FJavaWrapper::AndroidThunkJava_ShowConsoleWindow, JStr);
		Env->DeleteLocalRef(JStr);
	}
}
void FMobileUtilsPlatform::javatoast(const FString& activity)
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jstring JStr = Env->NewStringUTF(TCHAR_TO_UTF8(*activity));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, FMobileUtilsPlatform::javatoastMethod, JStr);
		Env->DeleteLocalRef(JStr);
	}
}
float FMobileUtilsPlatform::getbattery()
{
	float Result = 0.0f;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{		
		Result = (float)FJavaWrapper::CallFloatMethod(Env, FJavaWrapper::GameActivityThis, FMobileUtilsPlatform::javabatteryMethod);
	}
	return Result;
}
FString FMobileUtilsPlatform::GetAndroidID()
{
	FString returnvalue;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{

		jstring ResultDeviceIdString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, FMobileUtilsPlatform::javaGetAndroidIDMethod);
		const char* nativeDeviceIdString = Env->GetStringUTFChars(ResultDeviceIdString, 0);
		returnvalue = FString(nativeDeviceIdString);
		Env->ReleaseStringUTFChars(ResultDeviceIdString, nativeDeviceIdString);
		Env->DeleteLocalRef(ResultDeviceIdString);
	}
	return returnvalue;

}
int FMobileUtilsPlatform::NetworkconnectionInfor()
{
	int Result = 0;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		Result = (int)FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, FMobileUtilsPlatform::javaNetInforMethod);
	}
	return Result;
}
void FMobileUtilsPlatform::GPSInfor(float &a, float &b)
{
	
	FVector2D Result;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		a = (double)FJavaWrapper::CallDoubleMethod(Env, FJavaWrapper::GameActivityThis, FMobileUtilsPlatform::javaGPSlatitudeMethod);
		b = (double)FJavaWrapper::CallDoubleMethod(Env, FJavaWrapper::GameActivityThis, FMobileUtilsPlatform::javaGPSlongitudeMethod);
	}

}
bool FMobileUtilsPlatform::Isgpsenabled()
{
	
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		return  (bool)FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, FMobileUtilsPlatform::javaGPSIsgpsenabled);
	}
	return false;
}
void FMobileUtilsPlatform::GotoSettinggps()
{

	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		 FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, FMobileUtilsPlatform::javaGPSGotoSettinggps);
	}
}

const FString& GetBasePath()
{
	extern FString GFilePathBase;
	static FString BasePath = GFilePathBase + FString("/UE4Game/") + FApp::GetProjectName() + FString("/");
	return BasePath;
}

FString FMobileUtilsPlatform::ConvertToAbsolutePath(const FString& path)
{
	if (FPaths::IsRelative(path))
	{
		FString file = path.Replace(TEXT("../"), TEXT(""), ESearchCase::CaseSensitive);
		return GetBasePath() + file;
	}
	return path;
}




