// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SPRoomActivator1CPP.h"
#include "SPRoomManagerCPP.generated.h"

USTRUCT(BlueprintType)
struct FSPRoomConnectionInfo {

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = RoomManager)
		//From Wchic room we enter this one
		int EntraceRoomID;
	UPROPERTY(EditAnywhere, Category = RoomManager)
		//From Wchich Exit doors of previus room we get to this one
		int EntraceDoorID;
};

UCLASS()
class SUPERFIGHTER_API ASPRoomManagerCPP : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = RoomManager)
		FSPRoomActivatorData1 Data;

	UPROPERTY(EditAnywhere, Category = RoomManager)
		bool Active;

	TArray<ASPRoomManagerCPP*> RoomManagers;

	FSPRoomConnectionInfo RoomConnectionInfo;

	ASPRoomActivator1CPP* ChildActivator;
	
public:	
	// Sets default values for this actor's properties
	ASPRoomManagerCPP();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = RoomManager)
	bool GetActive() { return Active;  }
	

	UFUNCTION(BlueprintCallable, Category = RoomManager)
		bool Activate(FSPRoomActivatorData1 _Data);

	UFUNCTION(BlueprintCallable, Category = RoomManager)
		void SetConnectionInfo(int _EntraceRoomID, int _EntraceDoorID);

	UFUNCTION(BlueprintCallable, Category = RoomManager)
		void SetRoomManagersTable(TArray<ASPRoomManagerCPP*> _RoomManagers);
};
