// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperFighter.h"
#include "SPRoomLoaderCPP.h"


// Sets default values
ASPRoomLoaderCPP::ASPRoomLoaderCPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASPRoomLoaderCPP::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASPRoomLoaderCPP::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ASPRoomLoaderCPP::SetRoomLoader(bool _Show, ULevelStreamingKismet* _SubLevel)
{
	Show = _Show;
	SubLevel = _SubLevel;	
}

void ASPRoomLoaderCPP::Activate()
{
	if (SubLevel != nullptr) {
		SubLevel->bShouldBeVisible = Show;
	}
}

