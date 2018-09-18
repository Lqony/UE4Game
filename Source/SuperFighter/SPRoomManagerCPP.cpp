// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperFighter.h"
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
				SetActorLocation(Data.Location);
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

void ASPRoomManagerCPP::SetConnectionInfo(int _EntraceRoomID, int _EntraceDoorID)
{
	RoomConnectionInfo.EntraceRoomID = _EntraceRoomID;
	RoomConnectionInfo.EntraceDoorID = _EntraceDoorID;
}

void ASPRoomManagerCPP::SetRoomManagersTable(TArray<ASPRoomManagerCPP*> _RoomManagers)
{
	RoomManagers = _RoomManagers;
}

