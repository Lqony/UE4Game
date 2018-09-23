// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Engine/LevelStreamingKismet.h"
#include "SPRoomLoaderCPP.generated.h"

UCLASS()
class SUPERFIGHTER_API ASPRoomLoaderCPP : public AActor
{
	GENERATED_BODY()
protected:

	bool Show;
	ULevelStreamingKismet* SubLevel;
	
public:	
	// Sets default values for this actor's properties
	ASPRoomLoaderCPP();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = RoomLoader)
		void SetRoomLoader(bool _Show, ULevelStreamingKismet* _SubLevel);
	
	UFUNCTION(BlueprintCallable, Category = RoomLoader)
		void Activate();
};
