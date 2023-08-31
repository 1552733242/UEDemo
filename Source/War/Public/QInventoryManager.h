// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Datatable.h"
#include "QInventoryManager.generated.h"

class UScrollBox;
struct FItemDataBase;

UCLASS()
class WAR_API UQInventoryManager : public UObject
{
	GENERATED_BODY()
	
public:
	static bool ItemExitParentWidget(TObjectPtr<UScrollBox>itemParent, TSharedPtr<FItemDataBase> itemPtr);


	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDataTable> m_DataBase;
};
