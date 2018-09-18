// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Engine/LevelStreamingKismet.h"
#include "SPRoomActivator1CPP.generated.h"

USTRUCT(BlueprintType)
struct FSPRoomActivatorData1 {

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = RoomActivator)
		//Room ID
		int ID;
	UPROPERTY(EditAnywhere, Category = RoomActivator)
		ULevelStreamingKismet* ParentSubLevel;
	UPROPERTY(EditAnywhere, Category = RoomActivator)
		FVector Location;
	UPROPERTY(EditAnywhere, Category = RoomActivator)
		FVector Rotation;
	UPROPERTY(EditAnywhere, Category = RoomActivator)
		//Placeholder for now
		float SomeOtherData;
};

UCLASS()
class SUPERFIGHTER_API ASPRoomActivator1CPP : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = RoomActivator)
	FSPRoomActivatorData1 Data;

	UPROPERTY(EditAnywhere, Category = RoomActivator)
	bool Active;
	
public:	

	UPROPERTY(EditAnywhere, BlueprintReadOnly ,Category = RoomActivator)
		AActor *RoomManager;

	// Sets default values for this actor's properties
	ASPRoomActivator1CPP();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = RoomActivator1)
	bool Activate(int _ID, ULevelStreamingKismet *_ParentSubLevel, FVector _Location, FVector _Rotation, float _SomeOtherData);

	UFUNCTION(BlueprintCallable, Category = RoomActivator1)
		bool GetActive() { return Active;  }

	UFUNCTION(BlueprintCallable, Category = RoomActivator1)
	FSPRoomActivatorData1 GetData(AActor *_RoomManager);

	int GetID() { return Data.ID;  }
	
};
