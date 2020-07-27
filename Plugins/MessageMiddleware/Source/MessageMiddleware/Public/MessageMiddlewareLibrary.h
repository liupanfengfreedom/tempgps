// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MessageMiddlewareLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MESSAGEMIDDLEWARE_API UMessageMiddlewareLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
    UFUNCTION(BlueprintCallable, Category = "MessageMiddlewareLibrary")
        static void sendmessage(const FString& id , const FString& payload);
    UFUNCTION(BlueprintCallable, Category = "MessageMiddlewareLibrary")//this function may be not nessary
        static void sendmessagefaster(const FString& id, FString& payload);
    DECLARE_DYNAMIC_DELEGATE_OneParam(FOnsetlistenerevent, const FString&, message);
    UFUNCTION(BlueprintCallable, Category = "MessageMiddlewareLibrary")
        static void addmessagelistener(UObject* instance, const FString& id, FOnsetlistenerevent func);
    UFUNCTION(BlueprintCallable, Category = "MessageMiddlewareLibrary")
        static void removemessagelistener(UObject* instance);
};
