// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QInventory.h"
#include "QInvertoryPanel.generated.h"



UCLASS()
class WAR_API UQInvertoryPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct()override;

	UFUNCTION(BlueprintCallable)
	void UpdatePlayItems();
	UFUNCTION(BlueprintCallable)
	void UpdateSceneItms();

	UFUNCTION(BlueprintCallable)
	TArray<FName> GetPlayItemsStringListItems();
	UFUNCTION(BlueprintCallable)
	TArray<FName> GetSceneItemsStringListItems();
	//UFUNCTION(BlueprintCallable)
	//void GetIconStringListItems();


public:
	TArray<TSharedPtr<FItemDataBase>> m_PlayItemsList;
	TArray<TSharedPtr<FItemDataBase>> m_SceneItemsList;
};
