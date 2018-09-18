// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperFighter.h"
#include "SPRoomActivator1CPP.h"


// Sets default values
ASPRoomActivator1CPP::ASPRoomActivator1CPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Active = false;
}

// Called when the game starts or when spawned
void ASPRoomActivator1CPP::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASPRoomActivator1CPP::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

bool ASPRoomActivator1CPP::Activate(int _ID, ULevelStreamingKismet * _ParentSubLevel, FVector _Location, FVector _Rotation, float _SomeOtherData)
{
	Data.ID = _ID;
	Data.ParentSubLevel = _ParentSubLevel;
	Data.Location = _Location;
	Data.Rotation = _Rotation;
	Data.SomeOtherData = _SomeOtherData;

	Active = true;
	return Active;
}

FSPRoomActivatorData1 ASPRoomActivator1CPP::GetData(AActor * _RoomManager)
{
	RoomManager = _RoomManager;
	return Data;
}

