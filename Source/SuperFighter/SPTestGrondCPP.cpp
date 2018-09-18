// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperFighter.h"
#include "SPTestGrondCPP.h"


// Sets default values
ASPTestGrondCPP::ASPTestGrondCPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASPTestGrondCPP::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASPTestGrondCPP::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

FString ASPTestGrondCPP::SubLevelName()
{
	return GetLevel()->GetOuter()->GetPathName();
}

