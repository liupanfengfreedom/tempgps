// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalCharacter.h"
#include "Kcpclient.h"
#include "Engine.h"
#include "MobileUtilsBlueprintLibrary.h"
#include "KcpChannelManager.h"
#include "JsonUtilities.h"
#include "Json.h"
#include "MessageManager.h"
#include "PosCharacter.h"
// Sets default values
ALocalCharacter::ALocalCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALocalCharacter::BeginPlay()
{
	Super::BeginPlay();
	//bool b = UMobileUtilsBlueprintLibrary::javafunctionIsgpsenabled();
	//if (b)
	//{
	//}
	//else
	//{
	//	UMobileUtilsBlueprintLibrary::javafunctionGotoSettinggps();
	//}
	int32 id = FMath::Rand();
	FString androidid = "aassddff1122aaddss";
	androidid = FString::FromInt(id);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "thisid : " + androidid);

	//androidid = UMobileUtilsBlueprintLibrary::javafunctionAndroidID();
	mkcpchannel = MakeShareable(new KcpChannel(androidid));
	mkcpchannel->OnkcpChannelReceivedatadelegate.BindLambda([=](const uint8* data, uint32 size) {
		FString rec = FString(size, (char*)data);
		TSharedPtr<FJsonObject> jsonobject = MakeShareable(new FJsonObject);
		TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(rec);
		FJsonSerializer::Deserialize(Reader, jsonobject);
		int Command = jsonobject->GetIntegerField("Command");
		if (Command == 1)//create new player
		{
			AsyncTask(ENamedThreads::GameThread,
				[this, jsonobject]()
				{
					FString robotchannelid = jsonobject->GetStringField("channelid");
					float latitude = jsonobject->GetNumberField("latitude");
					float longitude = jsonobject->GetNumberField("longitude");
					latitude -= 3029082;
					longitude -= 11999529;
					FVector Location = FVector(latitude * 100, longitude * 100, 300);
					FRotator Rotation = FRotator(0, 0, 0);
					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					APosCharacter* mcharactor = GetWorld()->SpawnActor<APosCharacter>(Robotclass, Location, Rotation, ActorSpawnParams);
					mcharactor->SpawnDefaultController();
					mcharactor->Addkcpchannel(MakeShareable(new KcpChannel(robotchannelid)));
					GEngine->AddOnScreenDebugMessage(-1, 55.0f, FColor::Yellow, robotchannelid + "  : " + "latitude: " + FString::SanitizeFloat(latitude) + "longitude: " + FString::SanitizeFloat(longitude));
					GEngine->AddOnScreenDebugMessage(-1, 55.0f, FColor::Yellow, robotchannelid + "  : " + "Location: " + Location.ToString());

				}
			);


			//create player
		}
		});
	GetWorld()->GetTimerManager().SetTimer(th, this, &ALocalCharacter::timerworker, 1, true, 1);
	ADDMESSAGELISTEN(this, "1", [=](const void* p) {
		FString str = *(FString*)p;
		latitude = FCString::Atof(*str);
		})
		ADDMESSAGELISTEN(this, "2", [=](const void* p) {
			FString str = *(FString*)p;
			longitude = FCString::Atof(*str);
			})
}
void ALocalCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//if (mkcp)
	//{
	//	mkcp->exitthread = true;
	//}
	GetWorld()->GetTimerManager().ClearTimer(th);
	mkcpchannel->exit();

}
// Called every frame
void ALocalCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALocalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ALocalCharacter::timerworker()
{
	
	//UMobileUtilsBlueprintLibrary::javafunctionGPSInfor(latitude, longitude);

	//FString androidid = UMobileUtilsBlueprintLibrary::javafunctionAndroidID();
	//TCHAR* serializedChar = androidid.GetCharArray().GetData();
	//int32 size = FCString::Strlen(serializedChar);
	//int floatsize = sizeof(float);

	//realdata.SetNum(32+(floatsize*2));
	//FMemory::Memcpy(realdata.GetData(), (const uint8*)TCHAR_TO_UTF8(serializedChar), size);//here is 2 byte valide data length
	//FMemory::Memcpy(realdata.GetData() + 32, (const uint8*)&latitude, sizeof(float));//here is 2 byte valide data length
	//FMemory::Memcpy(realdata.GetData() + 32+ sizeof(float), (const uint8*)&longitude, sizeof(float));//here is 2 byte valide data length

	TSharedPtr<FJsonObject> newplayerjoin = MakeShareable(new FJsonObject);
	newplayerjoin->SetNumberField("Command", 1);
	newplayerjoin->SetNumberField("latitude", latitude);
	newplayerjoin->SetNumberField("longitude", longitude);
	//newplayerjoin->SetNumberField("latitude", 30.2345);
	//newplayerjoin->SetNumberField("longitude", 119.87655);
	FString OutputString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(newplayerjoin.ToSharedRef(), Writer);
	mkcpchannel->send(OutputString);

}