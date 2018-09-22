// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperFighter.h"
#include "Runtime/Engine/Classes/Engine/LevelStreamingKismet.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "SPMapManagerCPP.h"


void ASPMapManagerCPP::FinishCreating()
{
	if (CreationPending) {
		int l_index = Rooms.RoomLevels.Num()-1;
		int l_activator_index = -1;

		TArray<ASPRoomActivator1CPP*> l_RoomActivators = GetActivators();

		if (l_RoomActivators.Num() > 0) {

			l_activator_index = Rooms.RoomActivators.AddUnique(l_RoomActivators[0]);

			if (l_activator_index >= 0) {
				Rooms.RoomActivators[l_activator_index]->Activate(l_index, Rooms.RoomLevels[l_index], PendingLocation,
					FVector(0.0f, 0.0f, 0.0f), 0);
			}
		}

		/*for (int i = 0; i < l_RoomActivators.Num(); i++) {

			l_activator_index = Rooms.RoomActivators.AddUnique(l_RoomActivators[i]);

			if (l_activator_index >= 0) {
				Rooms.RoomActivators[l_activator_index]->Activate(l_index, Rooms.RoomLevels[l_index], FVector(0.0f, 0.0f, 0.0f),
					FVector(0.0f, 0.0f, 0.0f), 0);
			}

			CreationPending = false;
		}*/
	}
}

// Sets default values
ASPMapManagerCPP::ASPMapManagerCPP()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CreationPending = false;
	NewMapData.Creating = false;
}

// Called when the game starts or when spawned
void ASPMapManagerCPP::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASPMapManagerCPP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ManageActivators();
	FinishCreating();
}

bool ASPMapManagerCPP::CreateRoom(FVector _Location)
{
	if (!CreationPending) {
		ULevelStreamingKismet *l_RoomLevel = CallLoadLevelInstance();
		if (IsValid(l_RoomLevel)) {

			Rooms.RoomLevels.Add(l_RoomLevel);
			PendingLocation = _Location;
			CreationPending = true;

			return true;
		}
		else {

			return false;
		}
	}
		
	return false;
}

void ASPMapManagerCPP::ManageActivators()
{
	for (int i = 0; i < Rooms.RoomActivators.Num(); i++) {
		if (Rooms.RoomActivators[i]->GetActive()) {
			if (IsValid(Rooms.RoomActivators[i]->RoomManager)) {
				int l_index = Rooms.RoomManagers.AddUnique(Cast<ASPRoomManagerCPP>(Rooms.RoomActivators[i]->RoomManager));

				for (int j = 0; j < Rooms.RoomManagers.Num(); j++) {
					Rooms.RoomManagers[j]->SetRoomManagersTable(Rooms.RoomManagers);
				}
				
			//	Rooms.RoomManagers[l_index]->SetConnectionInfo();
				if (l_index < 0) {
					//CALL ERROR WE HAVE ROOM MANAGER DUPLICATE
				}
				else if (Rooms.RoomActivators[i]->GetID() != l_index) {
					//CALL MISSDESING, WE WANT TO ROOMMANAGERS AND ROOMLEVELS HAS SAME INDEX IN THEIR TABLES
				}

				Rooms.RoomActivators[i]->Destroy();
				Rooms.RoomActivators.RemoveAt(i);
				CreationPending = false;
			}
		}
	}
}

void ASPMapManagerCPP::CreateNewMap()
{
	if (!NewMapData.Creating) {
		NewMapData.Creating = true;
		NewMapData.State = 0;
	}

	switch (NewMapData.State) {

	case 0:
		//Destroy All Previous Actors
		for (int i = 0; i < Rooms.RoomManagers.Num(); i++) {
			Rooms.RoomManagers[i]->ClearAndDelete();
		}

		for (int i = 0; i < Rooms.RoomLevels.Num(); i++) {
			Rooms.RoomLevels[i]->BeginDestroy();
			Rooms.RoomLevels[i]->FinishDestroy();
		}
		NewMapData.RoomAmount = FMath::RandRange(30, 50);
		NewMapData.State = 1;
		break;

	case 1:
		if (!CreationPending) {
			if (Rooms.RoomManagers.Num() < NewMapData.RoomAmount) {
				CreateRoom();
			}
			else {
				NewMapData.State = 2;
			}
		}
		break;

	case 2:
	//	Rooms.RoomManagers[0]->SetConnectionInfo(-1, -1, )
		break;

	}
	
}

TArray<ASPRoomActivator1CPP*> ASPMapManagerCPP::GetActivators_Implementation()
{
	TArray<ASPRoomActivator1CPP*> Output;

	return Output;
}


ULevelStreamingKismet* ASPMapManagerCPP::CallLoadLevelInstance_Implementation()
{
	return nullptr;
}


