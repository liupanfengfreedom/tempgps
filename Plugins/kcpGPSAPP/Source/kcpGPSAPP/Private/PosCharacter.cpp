// Fill out your copyright notice in the Description page of Project Settings.


#include "PosCharacter.h"
#include "KcpChannelManager.h"
#include "JsonUtilities.h"
#include "Json.h"
#include "Engine.h"

// Sets default values
APosCharacter::APosCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APosCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APosCharacter::Addkcpchannel(const TSharedPtr<class KcpChannel, ESPMode::ThreadSafe>&kcpchannelp)
{
	mkcpchannel = kcpchannelp;
	mkcpchannel->OnkcpChannelReceivedatadelegate.BindLambda([=](const uint8* data, uint32 size) {
		FString rec = FString(size, (char*)data);
		TSharedPtr<FJsonObject> jsonobject = MakeShareable(new FJsonObject);
		TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(rec);
		FJsonSerializer::Deserialize(Reader, jsonobject);
		int Command = jsonobject->GetIntegerField("Command");
		if (Command == 2)//positiondata
		{
			FString channelid = jsonobject->GetStringField("channelid");
			float latitude = jsonobject->GetNumberField("latitude");
			float longitude = jsonobject->GetNumberField("longitude");
			AsyncTask(ENamedThreads::GameThread,
				[this,latitude,longitude,channelid]()
				{
					FVector location = GetActorLocation();
					location.X = (latitude - 3029082)*100;
					location.Y = (longitude - 11999529)*100;
					SetActorLocation(location);
				//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, channelid + "  : " + "latitude: " + FString::SanitizeFloat(latitude) + "longitude: " + FString::SanitizeFloat(longitude));
				//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, channelid + "  : " + "location.X: " + FString::SanitizeFloat(location.X) + "location.Y: " + FString::SanitizeFloat(location.Y));
				}
				);
		}
		else if (Command == 3)
		{
			AsyncTask(ENamedThreads::GameThread,
				[this]()
				{
					Destroy();
				}
			);
		}
		});
}
// Called every frame
void APosCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APosCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

