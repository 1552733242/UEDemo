// Fill out your copyright notice in the Description page of Project Settings.


#include "QInventory.h"

// Sets default values
AQInventory::AQInventory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AQInventory::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AQInventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

