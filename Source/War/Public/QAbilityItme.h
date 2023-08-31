// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QInventory.h"
#include "QAbilityItme.generated.h"




UCLASS()
class WAR_API AQAbilityItme : public AQInventory
{
	GENERATED_BODY()
public:
	AQAbilityItme();

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> m_Mesh;
};


USTRUCT()
struct FAbilityItme : public FItemDataBase
{

	GENERATED_BODY()
public:
	TObjectPtr<UStaticMeshComponent>Mesh;
	virtual FAbilityItme& operator=(const AQAbilityItme& ABItem) {
		Mesh = ABItem.m_Mesh;
		Super::operator=(ABItem);
		return *this;
	}
};