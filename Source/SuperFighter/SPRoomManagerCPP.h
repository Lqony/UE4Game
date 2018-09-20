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
		int EntraceRoomID = -1;
	UPROPERTY(EditAnywhere, Category = RoomManager)
		//From Wchich Exit doors of previus room we get to this one
		int EntraceDoorID;

	UPROPERTY(EditAnywhere, Category = RoomManager)
		//How Many Exits from this room
		int ExitAmount;

	UPROPERTY(EditAnywhere, Category = RoomManager)
		//Exit 1 type : 1 - Left Wall(1-3tile, 2 - Floor ((n-8)-(n-5)tile), 3 - roof ((n-8)-(n-5)tile))
		int ExitType1 = -1;

	UPROPERTY(EditAnywhere, Category = RoomManager)
		//From Wchich Exit doors of previus room we get to this one
		int ExitType2 = -1;

	UPROPERTY(EditAnywhere, Category = RoomManager)
		//From Wchich Exit doors of previus room we get to this one
		int ExitType3 = -1;
};

USTRUCT(BlueprintType)
struct FSPRoomInfo {
	GENERATED_BODY()

		
	UPROPERTY(EditAnywhere, Category = RoomManager)
	//In tiles amount
	int RoomWidth;
	UPROPERTY(EditAnywhere, Category = RoomManager)
	int RoomHeight;
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

	UPROPERTY(EditAnywhere, Category = RoomManager)
	TArray<ASPRoomManagerCPP*> RoomManagers;

	UPROPERTY(EditAnywhere, Category = RoomManager)
		FSPRoomInfo Info;

	UPROPERTY(EditAnywhere, Category = RoomManager)
	FSPRoomConnectionInfo RoomConnectionInfo;

	ASPRoomActivator1CPP* ChildActivator;

	UFUNCTION(BLueprintNativeEvent, Category = RoomManager)
		TArray<AActor*> SpawnTiles(int amount);

	UFUNCTION(BLueprintNativeEvent, Category = RoomManager)
	AActor* SpawnTile(FVector2D _Loc, FVector2D _Scale);

	
	
public:	
	// Sets default values for this actor's properties
	ASPRoomManagerCPP();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = RoomManager)
		void CreateRoom();

	UFUNCTION(BlueprintCallable, Category = RoomManager)
	bool GetActive() { return Active;  }
	

	UFUNCTION(BlueprintCallable, Category = RoomManager)
		bool Activate(FSPRoomActivatorData1 _Data);

	UFUNCTION(BlueprintCallable, Category = RoomManager)
		void SetConnectionInfo(int _EntraceRoomID, int _EntraceDoorID, int _ExitAmount, TArray<int> _ExitTypes);

	UFUNCTION(BlueprintCallable, Category = RoomManager)
		void SetRoomManagersTable(TArray<ASPRoomManagerCPP*> _RoomManagers);

	FSPRoomConnectionInfo *GetConnectionInfo() { return &RoomConnectionInfo; }

	FSPRoomInfo* GetInfo() { return &Info; }
};
