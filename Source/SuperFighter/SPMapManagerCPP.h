// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SPRoomManagerCPP.h"
#include "SPMapManagerCPP.generated.h"

USTRUCT(BlueprintType)
struct FSPRoomTable {

	GENERATED_BODY()

		UPROPERTY(EditAnywhere, Category = MapManager)
		TArray<ASPRoomManagerCPP*> RoomManagers;

		UPROPERTY(EditAnywhere, Category = MapManager)
		TArray<ULevelStreamingKismet*> RoomLevels;

		UPROPERTY(EditAnywhere, Category = MapManager)
		TArray<ASPRoomActivator1CPP*> RoomActivators;
};

USTRUCT(BlueprintType)
struct FSPCreateMapData {

	GENERATED_BODY()

		bool Creating; //Is Createing
		int State; //0 - Destroy Previous, 1 - Spawn RoomManagers, 2 - SetConnectionInfo
		int RoomAmount = 0;
};

UCLASS()
class SUPERFIGHTER_API ASPMapManagerCPP : public AActor
{
	GENERATED_BODY()

protected:

		UPROPERTY(EditAnywhere, Category = MapManager)
		FSPRoomTable Rooms;

		FSPCreateMapData NewMapData;

		bool CreationPending;
		FVector PendingLocation;
	
		void FinishCreating();

public:	
	// Sets default values for this actor's properties
	ASPMapManagerCPP();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = MapManager)
		bool CreateRoom(FVector _Location = FVector(0.0f, 0.0f, 0.0f));

	UFUNCTION(BlueprintCallable, Category = MapManager)
		void ManageActivators();

	UFUNCTION(BlueprintCallable, Category = MapManager)
		bool IsPending() { return CreationPending;	}

	UFUNCTION(BlueprintNativeEvent, Category = MapManager)
		ULevelStreamingKismet* CallLoadLevelInstance();

	UFUNCTION(BlueprintNativeEvent, Category = MapManager)
		TArray<ASPRoomActivator1CPP*> GetActivators();

	UFUNCTION(BlueprintCallable, Category = MapManager)
		TArray<ASPRoomManagerCPP*> GetRoomManager() { return Rooms.RoomManagers; }

	UFUNCTION(BlueprintCallable, Category = MapManager)
		void CreateNewMap();
};
