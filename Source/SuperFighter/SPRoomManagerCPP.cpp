// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperFighter.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "SPRoomManagerCPP.h"


// Sets default values
ASPRoomManagerCPP::ASPRoomManagerCPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Active = false;
}

// Called when the game starts or when spawned
void ASPRoomManagerCPP::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASPRoomManagerCPP::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
	if (!Active) {
		if(!ChildActivator){
			TArray<AActor*> l_Childs;
			int l_index = 0;
			GetAttachedActors(l_Childs);
			while (l_index < l_Childs.Num() && !ChildActivator) {
				if (Cast<ASPRoomActivator1CPP> (l_Childs[l_index]))
				{
					ChildActivator = Cast<ASPRoomActivator1CPP>(l_Childs[l_index]);
				}
				l_index++;
			}
		}
		else {
			if (ChildActivator->GetActive()) {
				Activate(ChildActivator->GetData(this));
			}
			else {
			//	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Yellow, "CHILD ERROR");
			}
		}
	}

}

bool ASPRoomManagerCPP::Activate(FSPRoomActivatorData1 _Data)
{
	Data = _Data;
	Active = true;
	return Active;
}

void ASPRoomManagerCPP::SetConnectionInfo(int _EntraceRoomID, int _EntraceDoorID, int _ExitAmount, TArray<int> _ExitTypes)
{
	RoomConnectionInfo.EntraceRoomID = _EntraceRoomID;
	RoomConnectionInfo.EntraceDoorID = _EntraceDoorID;
	RoomConnectionInfo.ExitAmount = _ExitAmount;
	
	if (_ExitTypes.Num() > 0)
		RoomConnectionInfo.ExitType1 = _ExitTypes[0];

	if (_ExitTypes.Num() > 1)
		RoomConnectionInfo.ExitType2 = _ExitTypes[1];

	if (_ExitTypes.Num() > 2)
		RoomConnectionInfo.ExitType3 = _ExitTypes[2];
}

void ASPRoomManagerCPP::SetRoomManagersTable(TArray<ASPRoomManagerCPP*> _RoomManagers)
{
	RoomManagers = _RoomManagers;
}

TArray<AActor*> ASPRoomManagerCPP::SpawnTiles_Implementation(int amount)
{
	return TArray<AActor*>();
}

void ASPRoomManagerCPP::CreateRoom()
{
	int l_room_w;
	int l_room_h;
	int l_tiles_amount;

	TArray<AActor*> l_Tiles;

	l_room_w = FMath::RandRange(20, 50);
	l_room_h = FMath::RandRange(10, 20);
	Info.RoomWidth = l_room_w;
	Info.RoomHeight = l_room_h;

	l_tiles_amount = ((l_room_w * 2) + (l_room_h * 2));

	l_Tiles = SpawnTiles(l_tiles_amount);

	for (int i = 1; i < l_room_w; i++) {
		FVector _TileLocation = l_Tiles[i]->GetActorLocation();
		_TileLocation.X += 20.0f * i;
		l_Tiles[i]->SetActorLocation(_TileLocation);
	
	}

	for (int i = 0; i <= l_room_h; i++) {
		FVector _TileLocation = l_Tiles[l_room_w-1]->GetActorLocation();
		_TileLocation.Z += (20.0f * i) + 20.0f;
		l_Tiles[l_room_w+i]->SetActorLocation(_TileLocation);
	}

	for (int i = 1; i < l_room_w; i++) {
		FVector _TileLocation = l_Tiles[l_room_w + l_room_h]->GetActorLocation();
		_TileLocation.X -= 20.0f * i;
		l_Tiles[l_room_w + l_room_h+i]->SetActorLocation(_TileLocation);
	}

	for (int i = 0; i < l_room_h; i++) {
		FVector _TileLocation = l_Tiles[l_room_w + l_room_h + l_room_w - 1]->GetActorLocation();
		_TileLocation.Z -= (20.0f * i) + 20.0f;
		l_Tiles[l_room_w + l_room_h + l_room_w + i]->SetActorLocation(_TileLocation);
	}

	if (RoomConnectionInfo.EntraceRoomID >= 0 && RoomManagers.Num() > 0) {
		
		if (RoomConnectionInfo.EntraceDoorID == 0) {
			if (IsValid(RoomManagers[RoomConnectionInfo.EntraceRoomID])) {
				FVector l_room_location = RoomManagers[RoomConnectionInfo.EntraceRoomID]->GetActorLocation();
				l_room_location.X += RoomManagers[RoomConnectionInfo.EntraceRoomID]->GetInfo()->RoomWidth * 20.0f;
				this->SetActorLocation(l_room_location);
			}
			
			l_Tiles[l_tiles_amount - 1]->Destroy();
			l_Tiles[l_tiles_amount - 2]->Destroy();
			l_Tiles[l_tiles_amount - 3]->Destroy();
		}
		else if (RoomConnectionInfo.EntraceDoorID == 1) {

			if (IsValid(RoomManagers[RoomConnectionInfo.EntraceRoomID])) {
				FVector l_room_location = RoomManagers[RoomConnectionInfo.EntraceRoomID]->GetActorLocation();
				GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Yellow, l_room_location.ToCompactString());
				l_room_location.X += (RoomManagers[RoomConnectionInfo.EntraceRoomID]->GetInfo()->RoomWidth-5) * 20.0f;
				l_room_location.Z -= ((l_room_h + 2) * 20.0f);
				this->SetActorLocation(l_room_location);
			}

			l_Tiles[l_room_w + l_room_h + l_room_w - 2]->Destroy();
			l_Tiles[l_room_w + l_room_h + l_room_w - 3]->Destroy();
			l_Tiles[l_room_w + l_room_h + l_room_w - 4]->Destroy();
		}
		else if (RoomConnectionInfo.EntraceDoorID == 2) {
			l_Tiles[1]->Destroy();
			l_Tiles[2]->Destroy();
			l_Tiles[3]->Destroy();

			if (IsValid(RoomManagers[RoomConnectionInfo.EntraceRoomID])) {
				FVector l_room_location = RoomManagers[RoomConnectionInfo.EntraceRoomID]->GetActorLocation();
				l_room_location.X += (RoomManagers[RoomConnectionInfo.EntraceRoomID]->GetInfo()->RoomWidth - 5) * 20.0f;
				l_room_location.Z += (RoomManagers[RoomConnectionInfo.EntraceRoomID]->GetInfo()->RoomHeight + 2) * 20.0f;
				this->SetActorLocation(l_room_location);
			}
		}


	}
	


	if (RoomConnectionInfo.ExitAmount > 0) {
		if (RoomConnectionInfo.ExitType1 != RoomConnectionInfo.ExitType2 && RoomConnectionInfo.ExitType1 != RoomConnectionInfo.ExitType3) {
			switch (RoomConnectionInfo.ExitType1) {
			case 0:
				l_Tiles[l_room_w]->Destroy();
				l_Tiles[l_room_w+1]->Destroy();
				l_Tiles[l_room_w+2]->Destroy();
				break;

			case 1:
				l_Tiles[l_room_w-2]->Destroy();
				l_Tiles[l_room_w-3]->Destroy();
				l_Tiles[l_room_w-4]->Destroy();
				break;

			case 2:
				l_Tiles[l_room_w + l_room_h +1]->Destroy();
				l_Tiles[l_room_w + l_room_h +2]->Destroy();
				l_Tiles[l_room_w + l_room_h +3]->Destroy();
				break;

			default:
				//CALL ERROR, NO EXIT TYPE HANDLED
				break;
			}
		}
	}
	if (RoomConnectionInfo.ExitAmount > 1) {
		if (RoomConnectionInfo.ExitType1 != RoomConnectionInfo.ExitType2 && RoomConnectionInfo.ExitType2 != RoomConnectionInfo.ExitType3) {
			switch (RoomConnectionInfo.ExitType2) {
			case 0:
				l_Tiles[l_room_w]->Destroy();
				l_Tiles[l_room_w + 1]->Destroy();
				l_Tiles[l_room_w + 2]->Destroy();
				break;

			case 1:
				l_Tiles[l_room_w - 2]->Destroy();
				l_Tiles[l_room_w - 3]->Destroy();
				l_Tiles[l_room_w - 4]->Destroy();
				break;

			case 2:
				l_Tiles[l_room_w + l_room_h + 1]->Destroy();
				l_Tiles[l_room_w + l_room_h + 2]->Destroy();
				l_Tiles[l_room_w + l_room_h + 3]->Destroy();
				break;

			default:
				//CALL ERROR, NO EXIT TYPE HANDLED
				break;
			}
		}
	}
	if (RoomConnectionInfo.ExitAmount > 2) {
		if (RoomConnectionInfo.ExitType1 != RoomConnectionInfo.ExitType3 && RoomConnectionInfo.ExitType2 != RoomConnectionInfo.ExitType3) {
			switch (RoomConnectionInfo.ExitType3) {
			case 0:
				l_Tiles[l_room_w]->Destroy();
				l_Tiles[l_room_w + 1]->Destroy();
				l_Tiles[l_room_w + 2]->Destroy();
				break;

			case 1:
				l_Tiles[l_room_w - 2]->Destroy();
				l_Tiles[l_room_w - 3]->Destroy();
				l_Tiles[l_room_w - 4]->Destroy();
				break;

			case 2:
				l_Tiles[l_room_w + l_room_h + 1]->Destroy();
				l_Tiles[l_room_w + l_room_h + 2]->Destroy();
				l_Tiles[l_room_w + l_room_h + 3]->Destroy();
				break;

			default:
				//CALL ERROR, NO EXIT TYPE HANDLED
				break;
			}
		}
	}
}

