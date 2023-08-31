// Fill out your copyright notice in the Description page of Project Settings.


#include "QInvertoryPanel.h"
#include "Components/ScrollBox.h"
#include "QCharacter.h"
#include "QInventoryManager.h"
#include "QItemPanel.h"
void UQInvertoryPanel::NativeConstruct()
{
	Super::NativeConstruct();
}

void UQInvertoryPanel::UpdatePlayItems()
{
	if (AQCharacter* character = Cast<AQCharacter>(GetOwningPlayerPawn())){

		m_PlayItemsList = character->GetItemsList();

		/*TArray<TSharedRef<FItemDataBase>> newItemsList = character->GetItemsList();
		m_PlayItemsList.RemoveAll([&](TSharedRef<FItemDataBase> Val) {
			return newItemsList.Find(Val) == INDEX_NONE;
			});
		for (auto& i : newItemsList) {
			if (m_PlayItemsList.Find(i) == INDEX_NONE) {
				m_PlayItemsList.Add(i);
			}
		}*/
		UClass* ItemPanelClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/Test/UI_ItemPanel.UI_ItemPanel_c'"));
		TObjectPtr<UScrollBox> palyItemWidgetList = Cast<UScrollBox>(GetWidgetFromName(FName("PlayerItems")));
		for (auto& i : palyItemWidgetList->GetAllChildren()) {
			if (TObjectPtr<UQItemPanel>ItemPanel = Cast<UQItemPanel>(i)) {
				if (m_PlayItemsList.Find(ItemPanel->m_ItemData) == INDEX_NONE) {
					palyItemWidgetList->RemoveChild(ItemPanel);
				}
			}
		}
		for (auto& i : m_PlayItemsList) {
			if (!UQInventoryManager::ItemExitParentWidget(palyItemWidgetList, i)) {
				TObjectPtr<UUserWidget>Panel = CreateWidget(this, ItemPanelClass);
				if (UQItemPanel* itemPanel = Cast<UQItemPanel>(Panel)) {
					itemPanel->m_ItemData = i;
					itemPanel->SetupUI();
					palyItemWidgetList->AddChild(itemPanel);
				}
			}
		}



	}
}

void UQInvertoryPanel::UpdateSceneItms()
{
	if (AQCharacter* character = Cast<AQCharacter>(GetOwningPlayerPawn())) {
		m_SceneItemsList = character->GetAroundPlayItems();
	/*	TArray<TSharedRef<FItemDataBase>> newItemsList = character->GetAroundPlayItems();
		m_SceneItemsList.RemoveAll([&](TSharedRef<FItemDataBase> Val) {
			return newItemsList.Find(Val) == INDEX_NONE;
			});
		for (auto& i : newItemsList) {
			if (m_SceneItemsList.Find(i) == INDEX_NONE) {
				m_SceneItemsList.Add(i);
			}
		}*/
		UClass* ItemPanelClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/Test/UI_ItemPanel.UI_ItemPanel_c'"));
		TObjectPtr<UScrollBox> sceneItemsList = Cast<UScrollBox>(GetWidgetFromName(FName("SceneItemCanPickUp")));
		for (auto& i : sceneItemsList->GetAllChildren()) {
			if (TObjectPtr<UQItemPanel>ItemPanel = Cast<UQItemPanel>(i)) {
				if (m_SceneItemsList.Find(ItemPanel->m_ItemData) == INDEX_NONE) {
					sceneItemsList->RemoveChild(ItemPanel);
				}
			}
		}
		for (auto& i : m_SceneItemsList) {
			if (!UQInventoryManager::ItemExitParentWidget(sceneItemsList, i)) {
				TObjectPtr<UUserWidget>Panel = CreateWidget(this, ItemPanelClass);
				if (UQItemPanel* itemPanel = Cast<UQItemPanel>(Panel)) {
					itemPanel->m_ItemData = i;
					itemPanel->SetupUI();
					sceneItemsList->AddChild(itemPanel);
				}
			}
		}


	}


	

}

TArray<FName> UQInvertoryPanel::GetPlayItemsStringListItems()
{
	TArray<FName>items;
	for (auto& i : m_PlayItemsList) {
		items.Add(i->Name);
	}
	return items;
}

TArray<FName> UQInvertoryPanel::GetSceneItemsStringListItems()
{
	TArray<FName>items;
	for (auto& i : m_SceneItemsList) {
		items.Add(i->Name);
	}
	return items;
}
