// Fill out your copyright notice in the Description page of Project Settings.


#include "MessageMiddlewareLibrary.h"
#include "MessageManager.h"

void UMessageMiddlewareLibrary::sendmessage(const FString& id, const FString& payload)
{
	FString tempstr = payload;
	SENDMESSAGE(id, &tempstr);
}
void UMessageMiddlewareLibrary::sendmessagefaster(const FString& id, FString& payload)
{
	SENDMESSAGE(id, &payload);
}
void UMessageMiddlewareLibrary::addmessagelistener(UObject* instance, const FString& id, FOnsetlistenerevent func)
{
	ADDMESSAGELISTEN(instance, id,
		[func](const void* const pd) {
			func.ExecuteIfBound(*(FString*)pd);
		}
	)
}
void UMessageMiddlewareLibrary::removemessagelistener(UObject* instance)
{
	REMOVEMESSAGELISTEN(instance);
}