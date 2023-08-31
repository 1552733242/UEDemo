// Fill out your copyright notice in the Description page of Project Settings.


#include "QInventoryManager.h"
#include "Components/ScrollBox.h"
#include "QInventory.h"
#include "QItemPanel.h"
bool UQInventoryManager::ItemExitParentWidget(TObjectPtr<UScrollBox> itemParent, TSharedPtr<FItemDataBase> itemPtr)
{
	for (auto& i : itemParent->GetAllChildren()) {
		if (TObjectPtr<UQItemPanel>itemPanel = Cast<UQItemPanel>(i)) {
			if (itemPanel->m_ItemData == itemPtr)return true;
		}
	}
	return false;
}


