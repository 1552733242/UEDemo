// Fill out your copyright notice in the Description page of Project Settings.


#include "QItemPanel.h"
#include "Components/TextBlock.h"
void UQItemPanel::SetupUI()
{
	if (TObjectPtr<UTextBlock>textLabel = Cast<UTextBlock>(GetWidgetFromName(FName("NameTextLabel")))) {
		textLabel->SetText(FText::FromName(m_ItemData->Name));
	}
}
