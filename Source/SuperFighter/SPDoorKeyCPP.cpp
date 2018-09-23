// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperFighter.h"
#include "SPDoorKeyCPP.h"


// Sets default values
ASPDoorKeyCPP::ASPDoorKeyCPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Open = false;
	Delta = 0;
}

// Called when the game starts or when spawned
void ASPDoorKeyCPP::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASPDoorKeyCPP::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (Open) {
		float l_delta = 60.0f * (DeltaTime / 1.0f);
		Delta += l_delta;
		if (IsValid(Doors)) {
			FVector l_doors_location = Doors->GetActorLocation();
			if (Type == 2) {
				l_doors_location.X -= l_delta;
				Doors->SetActorLocation(l_doors_location, false);
			}
			else if (Type == 1) {
				l_doors_location.Z += l_delta;
				Doors->SetActorLocation(l_doors_location, false);
			}


			if (Delta >= 60.0f * 5.0f) {
				this->Destroy();
			}
		}
	}

}

void ASPDoorKeyCPP::Set(int _Type, AActor * _Doors)
{
	Type = _Type;
	Doors = _Doors;
}

