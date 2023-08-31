// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QInventory.h"
#include "QInventoryManager.h"
#include "QEquipMent.generated.h"



UENUM(BlueprintType)
enum class EEquipMent : uint8
{
	EQM_None,
	EQM_ArmGuard			UMETA(DisplayName =	"ArmGuard"),
	EQM_ThighGuard			UMETA(DisplayName =	"ThighGuard"),
	EQM_CalfGuard			UMETA(DisplayName =	"CalfGuard"),
	EQM_Corselet			UMETA(DisplayName =	"Corselet"),
	EQM_ArmCorselet			UMETA(DisplayName ="ArmCorselet	"),
	EQM_Trousers			UMETA(DisplayName ="Trousers"),
	EQM_Girdle				UMETA(DisplayName ="Girdle"),
	EQM_Overcoat			UMETA(DisplayName ="Overcoat"),
	EQM_Shoe				UMETA(DisplayName ="Shoe"),
	EQM_BodyDecoration		UMETA(DisplayName ="BodyDecoration")
};





UCLASS()
class WAR_API AQEquipMent : public AQInventory
{
	GENERATED_BODY()
	
public:
	AQEquipMent();

public:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	float m_MaxCapacity = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Inventory")
	float m_CurrentCapacity = 0.0f;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> m_EquipMentMesh;
	UPROPERTY(EditAnywhere)
	EEquipMent m_Type = EEquipMent::EQM_None;
	TArray<TSharedRef<FItemDataBase>>m_ItemsSet;

};


USTRUCT()
struct FEqmentData : public FItemDataBase
{

	GENERATED_BODY()
public:
	float MaxCapacity;
	float CurrentCapacity;
	TObjectPtr <USkeletalMesh> EquipMentMesh;
	TArray<TSharedRef<FItemDataBase>>ItemsSet;

	virtual FEqmentData& operator=(const AQEquipMent& equipment) {
		MaxCapacity = equipment.m_MaxCapacity;
		CurrentCapacity = equipment.m_CurrentCapacity;
		EquipMentMesh = equipment.m_EquipMentMesh->GetSkeletalMeshAsset();
		ItemsSet = equipment.m_ItemsSet;
		Super::operator=(equipment);
		return *this;
	}
};
