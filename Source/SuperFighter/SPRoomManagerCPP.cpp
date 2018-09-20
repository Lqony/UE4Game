// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperFighter.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "SPRoomManagerCPP.h"


AActor * ASPRoomManagerCPP::SpawnTile_Implementation(FVector2D _Loc, FVector2D _Scale)
{
	return nullptr;
}

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

	l_room_w = FMath::RandRange(20, 50);
	l_room_h = FMath::RandRange(10, 20);
	Info.RoomWidth = l_room_w;
	Info.RoomHeight = l_room_h;

	FVector l_room_location;
	FVector2D l_wall_location;
	FVector2D l_wall_scale;
	//CreateLeftWal, check if there is an Entrace
	if(RoomConnectionInfo.EntraceRoomID >= 0 && RoomConnectionInfo.EntraceDoorID == 0){
		l_wall_location.X = 0.0f;
		l_wall_location.Y = (((l_room_h-5.0f) / 2.0f)+4.0f)  * 20.0f;
		l_wall_scale.X = 1.0f;
		l_wall_scale.Y = (l_room_h - 4.0f);		
		//Locate this room
		l_room_location = RoomManagers[RoomConnectionInfo.EntraceRoomID]->GetActorLocation();
		l_room_location.X += RoomManagers[RoomConnectionInfo.EntraceRoomID]->GetInfo()->RoomWidth * 20.0f;
	}
	else {
		l_wall_location.X = 0.0f;
		l_wall_location.Y = ((l_room_h-1.0f)  / 2.0f)  * 20.0f;
		l_wall_scale.X = 1.0f;
		l_wall_scale.Y = l_room_h;
	}
	SpawnTile(l_wall_location, l_wall_scale);

	//CreateRightWall, check if there is an Exit
	if (RoomConnectionInfo.ExitType1 == 0 || RoomConnectionInfo.ExitType2 == 0 || RoomConnectionInfo.ExitType3 == 0) {

		l_wall_location.X = (l_room_w - 1.0f)*20.0f;
		l_wall_location.Y = ((((l_room_h - 1.0f) - 4.0f) / 2.0f) + 4.0f)  * 20.0f;
		l_wall_scale.X = 1.0f;
		l_wall_scale.Y = (l_room_h - 4.0f);
	}
	else {
		l_wall_location.X = (l_room_w - 1.0f)*20.0f;
		l_wall_location.Y = ((l_room_h - 1.0f) / 2.0f)  * 20.0f;
		l_wall_scale.X = 1.0f;
		l_wall_scale.Y = l_room_h;
	}
	SpawnTile(l_wall_location, l_wall_scale);

	//CreateBootomWall, check if there is an Exit or/and Entrace
	if (RoomConnectionInfo.EntraceRoomID >= 0 && RoomConnectionInfo.EntraceDoorID == 2) {
		if (RoomConnectionInfo.ExitType1 == 1 || RoomConnectionInfo.ExitType2 == 1 || RoomConnectionInfo.ExitType3 == 1) {
			//Entrace and Exit

			l_wall_location.X = ((l_room_w-1) / 2.0f) *20.0f;
			l_wall_location.Y = 0.0f;
			l_wall_scale.X = l_room_w - 8.0f;
			l_wall_scale.Y = 1.0f;
		}
		else {
			//Just Entrace
			l_wall_location.X = (((l_room_w - 5.0f) / 2.0f)+4.0f)   * 20.0f;
			l_wall_location.Y = 0.0f;
			l_wall_scale.X = l_room_w - 4.0f;
			l_wall_scale.Y = 1.0f;
		}

		l_room_location = RoomManagers[RoomConnectionInfo.EntraceRoomID]->GetActorLocation();
		l_room_location.X += (RoomManagers[RoomConnectionInfo.EntraceRoomID]->GetInfo()->RoomWidth - 5) * 20.0f;
		l_room_location.Z += (RoomManagers[RoomConnectionInfo.EntraceRoomID]->GetInfo()->RoomHeight  * 20.0f);
	}
	else if (RoomConnectionInfo.ExitType1 == 1 || RoomConnectionInfo.ExitType2 == 1 || RoomConnectionInfo.ExitType3 == 1) {
		//Just Exit
		l_wall_location.X = ((l_room_w - 5.0f) / 2.0f)   * 20.0f;
		l_wall_location.Y = 0.0f;
		l_wall_scale.X = l_room_w - 4.0f;
		l_wall_scale.Y = 1.0f;
	}
	else {	
		l_wall_location.X = ((l_room_w - 1) /2.0f) *20.0f;
		l_wall_location.Y = 0.0f;
		l_wall_scale.X = l_room_w;
		l_wall_scale.Y = 1.0f;
	}
	SpawnTile(l_wall_location, l_wall_scale);

	//CreateTopWall, check if there is an Exit or/and Exit
	if (RoomConnectionInfo.EntraceRoomID >= 0 && RoomConnectionInfo.EntraceDoorID == 1) {
		if (RoomConnectionInfo.ExitType1 == 2 || RoomConnectionInfo.ExitType2 == 2 || RoomConnectionInfo.ExitType3 == 2) {
			//Entrace and Exit

			l_wall_location.X = ((l_room_w - 1) / 2.0f) *20.0f;
			l_wall_location.Y = (l_room_h-1)*20.0f;
			l_wall_scale.X = l_room_w - 8.0f;
			l_wall_scale.Y = 1.0f;
		}
		else {
			//Just Entrace
			l_wall_location.X = (((l_room_w - 5.0f) / 2.0f) + 4.0f)   * 20.0f;
			l_wall_location.Y = (l_room_h - 1)*20.0f;
			l_wall_scale.X = l_room_w - 4.0f;
			l_wall_scale.Y = 1.0f;
		}

		l_room_location = RoomManagers[RoomConnectionInfo.EntraceRoomID]->GetActorLocation();
		l_room_location.X += (RoomManagers[RoomConnectionInfo.EntraceRoomID]->GetInfo()->RoomWidth - 5) * 20.0f;
		l_room_location.Z -= (l_room_h  * 20.0f);
	}
	else if (RoomConnectionInfo.ExitType1 == 2 || RoomConnectionInfo.ExitType2 == 2 || RoomConnectionInfo.ExitType3 == 2) {
		//Just Exit
		l_wall_location.X = ((l_room_w - 5.0f) / 2.0f)   * 20.0f;
		l_wall_location.Y = (l_room_h - 1)*20.0f;
		l_wall_scale.X = l_room_w - 4.0f;
		l_wall_scale.Y = 1.0f;
	}
	else {
		l_wall_location.X = ((l_room_w - 1) / 2.0f) *20.0f;
		l_wall_location.Y = (l_room_h - 1)*20.0f;
		l_wall_scale.X = l_room_w;
		l_wall_scale.Y = 1.0f;
	}
	SpawnTile(l_wall_location, l_wall_scale);

	if (RoomConnectionInfo.EntraceRoomID >= 0) {
		this->SetActorLocation(l_room_location);
	}	
}

