// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SPDoorKeyCPP.generated.h"

UCLASS()
class SUPERFIGHTER_API ASPDoorKeyCPP : public AActor
{
	GENERATED_BODY()
protected:

	int Type;//1 for vertical, 2 for horizontal doors
	AActor *Doors;
	float Delta;
	
public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DoorKey)
	bool Open;

	// Sets default values for this actor's properties
	ASPDoorKeyCPP();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = DoorKey)
	void Set(int _Type, AActor *_Doors);
};
