// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Datatable.h"
#include "QInventory.generated.h"





UCLASS()
class WAR_API AQInventory : public AActor
{
	GENERATED_BODY()
	
public:	
	AQInventory();

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float m_Volume = 0;
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FName m_Name;
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FString m_Desc;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};

USTRUCT()
struct FItemDataBase : public FTableRowBase
{

	GENERATED_BODY()
public:
	float Volume;
	FName Name;
	FString Desc;

	virtual FItemDataBase& operator=(const AQInventory& inventory) {
		Name = inventory.m_Name;
		Desc = inventory.m_Desc;
		Volume = inventory.m_Volume;
		return *this;
	}
};