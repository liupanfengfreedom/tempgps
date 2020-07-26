// Fill out your copyright notice in the Description page of Project Settings.


#include "GRFunctionLibrary.h"
#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#include "Android/Utils/ARMethodCallUtils.h"
#include "Android/Utils/ARArrayConvertor.h"
#endif
static const ANSICHAR* GRBridgetocppClassName = "dev/nick/app/screencast/app/GRBridgetocpp";
void UGRFunctionLibrary::Recordegame(Filetype type)
{

#if PLATFORM_ANDROID

	ARMethodCallUtils::CallStaticVoidMethod(GRBridgetocppClassName, "starttask", "(Landroid/app/Activity;ZI)V", FJavaWrapper::GameActivityThis,false,int(type));

#endif
}