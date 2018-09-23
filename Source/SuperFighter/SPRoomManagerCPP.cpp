// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperFighter.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "SPRoomManagerCPP.h"


AActor * ASPRoomManagerCPP::SpawnTile_Implementation(FVector2D _Loc, FVector2D _Scale, int Material)
{
	return nullptr;
}

AActor * ASPRoomManagerCPP::SpawnRoomLoader_Implementation(FVector2D _Loc, FVector2D _Scale, ULevelStreamingKismet* _SubLevel, bool _Show)
{
	return nullptr;
}

bool ASPRoomManagerCPP::IsExitType(int type)
{
	for (int i = 0; i < RoomConnectionInfo.ExitType.Num() && i < 3; i++) {
		if (RoomConnectionInfo.ExitType[i] == type) {
			return true;
		}
	}
	return false;
}

// Sets default values
ASPRoomManagerCPP::ASPRoomManagerCPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Active = false;
	Created = false;
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
	
	for (int i = 0; i < 3; i++) {
		if (_ExitAmount > i) {
			RoomConnectionInfo.ExitType.Add(_ExitTypes[i]);
		}
		else {
			RoomConnectionInfo.ExitType.Add(-1);
		}
	}
}

void ASPRoomManagerCPP::SetExitRoom(int _Type, ULevelStreamingKismet * _SubLevel)
{
	FVector2D _location;
	FVector2D _scale;
	switch (_Type) {

	case 0:
		//ShowNextRoom
		_location.X = (Info.RoomWidth+0.1f) *20.0f * 5.0f;
		_location.Y = 2.0f * 20.0f * 5.0f;
		_scale.X = 1.0f;
		_scale.Y = 3.0f * 5.0f;
		//SpawnTile(_location, _scale, 0);
		SpawnRoomLoader(_location, _scale, _SubLevel, true);

		_location.X = (Info.RoomWidth -0.4f) *20.0f * 5.0f;
		_location.Y = 2.0f * 20.0f * 5.0f;
		//SpawnTile(_location, _scale, 0);
		SpawnRoomLoader(_location, _scale, _SubLevel, false);
		break;

	case 1:
		//ShowNextRoom
		_location.X = (Info.RoomWidth - 3.0f) *20.0f * 5.0f;
		_location.Y = -1.8f * 20.0f * 5.0f;
		_scale.X = 3.0f * 5.0f;
		_scale.Y = 1.0f;
		//SpawnTile(_location, _scale, 0);
		SpawnRoomLoader(_location, _scale, _SubLevel, true);

		_location.X = (Info.RoomWidth - 3.0f) *20.0f * 5.0f;
		_location.Y = -0.6f * 20.0f * 5.0f;
		//SpawnTile(_location, _scale, 0);
		SpawnRoomLoader(_location, _scale, _SubLevel, false);
		break;

	case 2:
		//ShowNextRoom
		_location.X = (Info.RoomWidth - 3.0f) *20.0f * 5.0f;
		_location.Y = (Info.RoomHeight+0.2f) * 20.0f * 5.0f;
		_scale.X = 3.0f * 5.0f;
		_scale.Y = 1.0f;
		//SpawnTile(_location, _scale, 0);
		SpawnRoomLoader(_location, _scale, _SubLevel, true);

		_location.X = (Info.RoomWidth - 3.0f) *20.0f * 5.0f;
		_location.Y = (Info.RoomHeight-0.5) * 20.0f * 5.0f;
		//SpawnTile(_location, _scale, 0);
		SpawnRoomLoader(_location, _scale, _SubLevel, false);
		break;
	}
}

void ASPRoomManagerCPP::SetRoomManagersTable(TArray<ASPRoomManagerCPP*> _RoomManagers)
{
	RoomManagers = _RoomManagers;
}

void ASPRoomManagerCPP::ClearAndDelete()
{
	TArray<AActor*> l_Childs;
	GetAttachedActors(l_Childs);

	for (int i = 0; i < l_Childs.Num(); i++) {
		l_Childs[i]->Destroy();
	}
	this->Destroy();
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
		l_wall_location.X = 0.0f * 5.0f;
		l_wall_location.Y = (((l_room_h-5.0f) / 2.0f)+4.0f)  * 20.0f * 5.0f;
		l_wall_scale.X = 1.0f * 5.0f;
		l_wall_scale.Y = (l_room_h - 4.0f) * 5.0f;
		//Locate this room
		l_room_location = RoomManagers[RoomConnectionInfo.EntraceRoomID]->GetActorLocation();
		l_room_location.X += (RoomManagers[RoomConnectionInfo.EntraceRoomID]->GetInfo()->RoomWidth-1) * 20.0f * 5.0f;
	}
	else {
		l_wall_location.X = 0.0f * 5.0f;
		l_wall_location.Y = ((l_room_h-1.0f)  / 2.0f)  * 20.0f * 5.0f;
		l_wall_scale.X = 1.0f * 5.0f;
		l_wall_scale.Y = l_room_h * 5.0f;
	}
	SpawnTile(l_wall_location, l_wall_scale, 0);

	//CreateRightWall, check if there is an Exit
	if (IsExitType(0)) {

		l_wall_location.X = (l_room_w - 1.0f)*20.0f * 5.0f;
		l_wall_location.Y = ((((l_room_h - 1.0f) - 4.0f) / 2.0f) + 4.0f)  * 20.0f * 5.0f;
		l_wall_scale.X = 1.0f * 5.0f;
		l_wall_scale.Y = (l_room_h - 4.0f) * 5.0f;
	}
	else {
		l_wall_location.X = (l_room_w - 1.0f)*20.0f * 5.0f;
		l_wall_location.Y = ((l_room_h - 1.0f) / 2.0f)  * 20.0f * 5.0f;
		l_wall_scale.X = 1.0f * 5.0f;
		l_wall_scale.Y = l_room_h * 5.0f;
	}
	SpawnTile(l_wall_location, l_wall_scale, 0);

	//CreateBootomWall, check if there is an Exit or/and Entrace
	if (RoomConnectionInfo.EntraceRoomID >= 0 && RoomConnectionInfo.EntraceDoorID == 2) {
		if (IsExitType(1)) {
			//Entrace and Exit

			l_wall_location.X = ((l_room_w-1) / 2.0f) *20.0f * 5.0f;
			l_wall_location.Y = 0.0f * 5.0f;
			l_wall_scale.X = (l_room_w - 8.0f) * 5.0f;
			l_wall_scale.Y = 1.0f * 5.0f;
		}
		else {
			//Just Entrace
			l_wall_location.X = (((l_room_w - 5.0f) / 2.0f)+4.0f)   * 20.0f * 5.0f;
			l_wall_location.Y = 0.0f * 5.0f;
			l_wall_scale.X = (l_room_w - 4.0f) * 5.0f;
			l_wall_scale.Y = 1.0f * 5.0f;
		}

		l_room_location = RoomManagers[RoomConnectionInfo.EntraceRoomID]->GetActorLocation();
		l_room_location.X += (RoomManagers[RoomConnectionInfo.EntraceRoomID]->GetInfo()->RoomWidth - 5) * 20.0f * 5.0f;
		l_room_location.Z += ((RoomManagers[RoomConnectionInfo.EntraceRoomID]->GetInfo()->RoomHeight-1.0f)  * 20.0f) * 5.0f;
	}
	else if (IsExitType(1)) {
		//Just Exit
		l_wall_location.X = ((l_room_w - 5.0f) / 2.0f)   * 20.0f * 5.0f;
		l_wall_location.Y = 0.0f * 5.0f;
		l_wall_scale.X = (l_room_w - 4.0f) * 5.0f;
		l_wall_scale.Y = 1.0f * 5.0f;
	}
	else {	
		l_wall_location.X = ((l_room_w - 1) /2.0f) *20.0f * 5.0f;
		l_wall_location.Y = 0.0f * 5.0f;
		l_wall_scale.X = l_room_w * 5.0f;
		l_wall_scale.Y = 1.0f * 5.0f;
	}
	SpawnTile(l_wall_location, l_wall_scale, 0);

	//CreateTopWall, check if there is an Exit or/and Exit
	if (RoomConnectionInfo.EntraceRoomID >= 0 && RoomConnectionInfo.EntraceDoorID == 1) {
		if (IsExitType(2)) {
			//Entrace and Exit

			l_wall_location.X = ((l_room_w - 1) / 2.0f) *20.0f * 5.0f;
			l_wall_location.Y = (l_room_h-1)*20.0f * 5.0f;
			l_wall_scale.X = (l_room_w - 8.0f) * 5.0f;
			l_wall_scale.Y = 1.0f * 5.0f;
		}
		else {
			//Just Entrace
			l_wall_location.X = (((l_room_w - 5.0f) / 2.0f) + 4.0f)   * 20.0f  * 5.0f;
			l_wall_location.Y = (l_room_h - 1)*20.0f * 5.0f;
			l_wall_scale.X = (l_room_w - 4.0f) * 5.0f;
			l_wall_scale.Y = 1.0f * 5.0f;
		}

		l_room_location = RoomManagers[RoomConnectionInfo.EntraceRoomID]->GetActorLocation();
		l_room_location.X += (RoomManagers[RoomConnectionInfo.EntraceRoomID]->GetInfo()->RoomWidth - 5) * 20.0f * 5.0f;
		l_room_location.Z -= ((l_room_h-1.0f)  * 20.0f) * 5.0f;
	}
	else if (IsExitType(2)) {
		//Just Exit
		l_wall_location.X = ((l_room_w - 5.0f) / 2.0f)   * 20.0f * 5.0f;
		l_wall_location.Y = (l_room_h - 1)*20.0f * 5.0f;
		l_wall_scale.X = (l_room_w - 4.0f) * 5.0f;
		l_wall_scale.Y = 1.0f * 5.0f;
	}
	else {
		l_wall_location.X = ((l_room_w - 1) / 2.0f) *20.0f * 5.0f;
		l_wall_location.Y = (l_room_h - 1)*20.0f * 5.0f;
		l_wall_scale.X = l_room_w * 5.0f;
		l_wall_scale.Y = 1.0f * 5.0f;
	}
	SpawnTile(l_wall_location, l_wall_scale, 0);
	
	
	if (RoomConnectionInfo.ExitAmount > 0) {
		for (int i = 0; i < RoomConnectionInfo.ExitType.Num(); i++) {
			if (RoomConnectionInfo.ExitType[i] == 0) {
				//RightDoors
				l_wall_location.X = (l_room_w - 1.0f)*20.0f * 5.0f;
				l_wall_location.Y = 40.0f * 5.0f;
				l_wall_scale.X = 1.0f * 5.0f;
				l_wall_scale.Y = 3.0f * 5.0f;
				Doors.Add(SpawnTile(l_wall_location, l_wall_scale, 1));
				FVector2D _key_location;
				_key_location.X = FMath::RandRange(3, l_room_w - 3) * 20.0f * 5.0f;
				_key_location.Y = FMath::RandRange(1.0f, 3.0f) * 20.0f * 5.0f;
				SpawnDoorKey(1, Doors[i], _key_location);

				//Right Hallway
				l_wall_location.X = (l_room_w + 1.5f)*20.0f * 5.0f;
				l_wall_location.Y = 4*20.0f * 5.0f;
				l_wall_scale.X = 4.0f * 5.0f;
				l_wall_scale.Y = 1.0f * 5.0f;
				SpawnTile(l_wall_location, l_wall_scale, 0);

				l_wall_location.X = (l_room_w + 1.5f)*20.0f * 5.0f;
				l_wall_location.Y = 0.0f;
				l_wall_scale.X = 4.0f * 5.0f;
				l_wall_scale.Y = 1.0f * 5.0f;
				SpawnTile(l_wall_location, l_wall_scale, 0);
			}
			else if (RoomConnectionInfo.ExitType[i] == 1) {
				//BottomDoors
				l_wall_location.X = (l_room_w - 3.0f) *20.0f * 5.0f;
				l_wall_location.Y = 0.0f * 5.0f;
				l_wall_scale.X = 3.0f * 5.0f;
				l_wall_scale.Y = 1.0f * 5.0f;
				Doors.Add(SpawnTile(l_wall_location, l_wall_scale, 1));
				FVector2D _key_location;
				_key_location.X = FMath::RandRange(3, l_room_w - 3) * 20.0f * 5.0f;
				_key_location.Y = FMath::RandRange(1.0f, 3.0f) * 20.0f * 5.0f;
				SpawnDoorKey(2, Doors[i], _key_location);

				//Bottom Hallway
				l_wall_location.X = (l_room_w - 5.0f) *20.0f * 5.0f;
				l_wall_location.Y = -2.5f * 20.0f * 5.0f;
				l_wall_scale.X = 1.0f * 5.0f;
				l_wall_scale.Y = 4.0f * 5.0f;
				SpawnTile(l_wall_location, l_wall_scale, 0);

				l_wall_location.X = (l_room_w - 1.0f) *20.0f * 5.0f;
				l_wall_location.Y = -2.5f * 20.0f * 5.0f;
				l_wall_scale.X = 1.0f * 5.0f;
				l_wall_scale.Y = 4.0f * 5.0f;
				SpawnTile(l_wall_location, l_wall_scale, 0);

				//Stairs
				l_wall_location.X = (l_room_w - 3.5f) *20.0f * 5.0f;
				l_wall_location.Y = -2.0f * 20.0f * 5.0f;
				l_wall_scale.X = 2.0f * 5.0f;
				l_wall_scale.Y = 1.0f;
				SpawnTile(l_wall_location, l_wall_scale, 0);

				l_wall_location.X = (l_room_w - 2.5f) *20.0f * 5.0f;
				l_wall_location.Y = -4.0f * 20.0f * 5.0f;
				l_wall_scale.X = 2.0f * 5.0f;
				l_wall_scale.Y = 1.0f;
				SpawnTile(l_wall_location, l_wall_scale, 0);
				
			}
			else if (RoomConnectionInfo.ExitType[i] == 2) {
				//Top Doors
				l_wall_location.X = (l_room_w - 3.0f) *20.0f * 5.0f;
				l_wall_location.Y = (l_room_h - 1)*20.0f * 5.0f;
				l_wall_scale.X = 3.0f * 5.0f;
				l_wall_scale.Y = 1.0f * 5.0f;
				Doors.Add(SpawnTile(l_wall_location, l_wall_scale, 1));
				FVector2D _key_location;
				_key_location.X = FMath::RandRange(3, l_room_w - 3) * 20.0f * 5.0f;
				_key_location.Y = FMath::RandRange(1.0f, 3.0f) * 20.0f * 5.0f;
				SpawnDoorKey(2, Doors[i], _key_location);

				//Top Hallway
				l_wall_location.X = (l_room_w - 5.0f) *20.0f * 5.0f;
				l_wall_location.Y = (l_room_h+1.5f) * 20.0f * 5.0f;
				l_wall_scale.X = 1.0f * 5.0f;
				l_wall_scale.Y = 4.0f * 5.0f;
				SpawnTile(l_wall_location, l_wall_scale, 0);

				l_wall_location.X = (l_room_w - 1.0f) *20.0f * 5.0f;
				l_wall_location.Y = (l_room_h + 1.5f) * 20.0f * 5.0f;
				l_wall_scale.X = 1.0f * 5.0f;
				l_wall_scale.Y = 4.0f * 5.0f;
				SpawnTile(l_wall_location, l_wall_scale, 0);

				//Top Stairs
				l_wall_location.X = (l_room_w - 3.5f) *20.0f * 5.0f;
				l_wall_location.Y = (l_room_h + 2.0f) * 20.0f * 5.0f;
				l_wall_scale.X = 2.0f * 5.0f;
				l_wall_scale.Y = 1.0f;
				SpawnTile(l_wall_location, l_wall_scale, 0);

				l_wall_location.X = (l_room_w - 2.5f) *20.0f * 5.0f;
				l_wall_location.Y = (l_room_h) * 20.0f * 5.0f;
				l_wall_scale.X = 2.0f * 5.0f;
				l_wall_scale.Y = 1.0f;
				SpawnTile(l_wall_location, l_wall_scale, 0);
			}

			else {
				//ERROR, UNHANDLED DOOR TYPE
				i = 3;
			}
		}
	}

	if (RoomConnectionInfo.EntraceRoomID >= 0) {
		//l_room_location dodaj based on hallway
		if (RoomConnectionInfo.EntraceDoorID == 0) {
			//Left HighWall
			l_wall_location.X = (0.0f - 2.5f)*20.0f * 5.0f;
			l_wall_location.Y = 4 * 20.0f * 5.0f;
			l_wall_scale.X = 4.0f * 5.0f;
			l_wall_scale.Y = 1.0f * 5.0f;
			SpawnTile(l_wall_location, l_wall_scale, 0);

			l_wall_location.X = (0.0f - 2.5f)*20.0f * 5.0f;
			l_wall_location.Y = 0.0f;
			l_wall_scale.X = 4.0f * 5.0f;
			l_wall_scale.Y = 1.0f * 5.0f;
			SpawnTile(l_wall_location, l_wall_scale, 0);

			RoomManagers[RoomConnectionInfo.EntraceRoomID]->SetExitRoom(0, Data.ParentSubLevel);

			l_room_location.X += 9.0f * 20.0f*5.0f;
		}
		else if(RoomConnectionInfo.EntraceDoorID == 1){
			//Top Hallway
			l_wall_location.X = 0;
			l_wall_location.Y = (l_room_h + 1.5f) * 20.0f * 5.0f;
			l_wall_scale.X = 1.0f * 5.0f;
			l_wall_scale.Y = 4.0f * 5.0f;
			SpawnTile(l_wall_location, l_wall_scale, 0);

			l_wall_location.X = 4.0f * 20.0f * 5.0f;
			l_wall_location.Y = (l_room_h + 1.5f) * 20.0f * 5.0f;
			l_wall_scale.X = 1.0f * 5.0f;
			l_wall_scale.Y = 4.0f * 5.0f;
			SpawnTile(l_wall_location, l_wall_scale, 0);

			//Stairs
			l_wall_location.X = 1.5f * 20.0f * 5.0f;
			l_wall_location.Y = (l_room_h + 2.0f) * 20.0f * 5.0f;
			l_wall_scale.X = 2.0f * 5.0f;
			l_wall_scale.Y = 1.0f;
			SpawnTile(l_wall_location, l_wall_scale, 0);

			l_wall_location.X = 2.5f * 20.0f * 5.0f;
			l_wall_location.Y = (l_room_h) * 20.0f * 5.0f;
			l_wall_scale.X = 2.0f * 5.0f;
			l_wall_scale.Y = 1.0f;
			SpawnTile(l_wall_location, l_wall_scale, 0);

			RoomManagers[RoomConnectionInfo.EntraceRoomID]->SetExitRoom(1, Data.ParentSubLevel);

			l_room_location.Z -= 9.0f * 20.0f*5.0f;
		}
		else if (RoomConnectionInfo.EntraceDoorID == 2) {
			//Bottom Hallway
			l_wall_location.X = 0;
			l_wall_location.Y = (-2.5f) * 20.0f * 5.0f;
			l_wall_scale.X = 1.0f * 5.0f;
			l_wall_scale.Y = 4.0f * 5.0f;
			SpawnTile(l_wall_location, l_wall_scale, 0);

			l_wall_location.X = 4.0f * 20.0f * 5.0f;
			l_wall_location.Y = (-2.5f) * 20.0f * 5.0f;
			l_wall_scale.X = 1.0f * 5.0f;
			l_wall_scale.Y = 4.0f * 5.0f;
			SpawnTile(l_wall_location, l_wall_scale, 0);

			//Stairs
			l_wall_location.X = 1.5f * 20.0f * 5.0f;
			l_wall_location.Y = -2.0f * 20.0f * 5.0f;
			l_wall_scale.X = 2.0f * 5.0f;
			l_wall_scale.Y = 1.0f;
			SpawnTile(l_wall_location, l_wall_scale, 0);

			l_wall_location.X = 2.5f * 20.0f * 5.0f;
			l_wall_location.Y = -4.0f * 20.0f * 5.0f;
			l_wall_scale.X = 2.0f * 5.0f;
			l_wall_scale.Y = 1.0f;
			SpawnTile(l_wall_location, l_wall_scale, 0);

			RoomManagers[RoomConnectionInfo.EntraceRoomID]->SetExitRoom(2, Data.ParentSubLevel);

			l_room_location.Z += 9.0f * 20.0f*5.0f;
		}
		this->SetActorLocation(l_room_location);

		Data.ParentSubLevel->bShouldBeVisible = false;
	}	

	Created = true;
}

void ASPRoomManagerCPP::SpawnDoorKey_Implementation(int _Type, AActor * _DoorsActor, FVector2D _Location)
{

}

