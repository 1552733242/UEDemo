// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QInventory.h"
#include "QItemPanel.generated.h"

class UScrollBox;

UCLASS()
class WAR_API UQItemPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetupUI();
	TSharedPtr<FItemDataBase>m_ItemData;
};
