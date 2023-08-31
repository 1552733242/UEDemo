// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "QEquipMent.h"
#include "QCharacter.generated.h"

struct FInputActionValue;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UUserWidget;
class AQEquipMent;
class AQInventory;

UCLASS()
class WAR_API AQCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AQCharacter();
	friend class ACharacterPreview;

protected:
	virtual void BeginPlay() override;

	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> m_Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> m_CameraBoom;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsRun()const { return m_IsRun; }
	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayEquipMentSomething(EEquipMent slot, USkeletalMesh* equipMent);

	TArray<TSharedPtr<FItemDataBase>>GetItemsList();
	TArray<TSharedPtr<FItemDataBase>>GetAroundPlayItems();

private:
	void PickUpItem(TObjectPtr<AQInventory> inventory);
	void EquipItem(TObjectPtr<AQEquipMent> equipMent);
	void EquipItem(TObjectPtr<USkeletalMeshComponent>SkMesh, TObjectPtr<AQEquipMent> equipMent);
	void InitSkMeshConstruction();
	void InitSkMeshBeginPlay();	
	void OpenUIInput(TObjectPtr<UUserWidget> WidgetFouces);
	void CloseUIInput();

	void DropItem(TSharedPtr<FItemDataBase>item);

	

	//Input
	//Locomotion
	void OnViewYawInputActionTriggered(const FInputActionValue& Value);
	void OnViewPitchInputActionTriggered(const FInputActionValue& Value);
	void OnMoveForwardInputActionTriggered(const FInputActionValue& Value);
	void OnMoveRightInputActionTriggered(const FInputActionValue& Value);
	void OnMoveZoomInputActionTriggered(const FInputActionValue& Value);
	void OnJumpInputActionStarted(const FInputActionValue& Value);
	void OnCrouchInputActionTriggered(const FInputActionValue& Value);
	void OnCrouchInputActionCompleted(const FInputActionValue& Value);
	void OnRunInputActionTriggered(const FInputActionValue& Value);
	void OnRunInputActionCompleted(const FInputActionValue& Value);

	//Inventory
	void OnOpenInventoryPanelActionStarted(const FInputActionValue& Value);
	void OnPcikUpActionStarted(const FInputActionValue& Value);
private:
	//Input System
	UPROPERTY(EditAnywhere,Category="EnhancedInput")
	TObjectPtr<UInputMappingContext> m_InputMappingContext;
	UPROPERTY(EditAnywhere,Category="EnhancedInput|BasicLocomotion")
	TObjectPtr<UInputAction>  m_InputMoveForward;
	UPROPERTY(EditAnywhere,Category="EnhancedInput|BasicLocomotion")
	TObjectPtr<UInputAction>  m_InputMoveRight;
	UPROPERTY(EditAnywhere,Category="EnhancedInput|BasicLocomotion")
	TObjectPtr<UInputAction>  m_InputControllerYaw;
	UPROPERTY(EditAnywhere,Category="EnhancedInput|BasicLocomotion")
	TObjectPtr<UInputAction>  m_InputControllerPitch;
	UPROPERTY(EditAnywhere, Category = "EnhancedInput|BasicLocomotion")
	TObjectPtr<UInputAction>  m_InputZoom;
	UPROPERTY(EditAnywhere, Category = "EnhancedInput|BasicLocomotion")
	TObjectPtr<UInputAction>  m_InputJump;
	UPROPERTY(EditAnywhere, Category = "EnhancedInput|BasicLocomotion")
	TObjectPtr<UInputAction>  m_InputCrouch;
	UPROPERTY(EditAnywhere, Category = "EnhancedInput|BasicLocomotion")
	TObjectPtr<UInputAction>  m_InputRun;
	UPROPERTY(EditAnywhere, Category = "EnhancedInput|Inventory")
	TObjectPtr<UInputAction>  m_InputOpenInventoryPanel;
	UPROPERTY(EditAnywhere, Category = "EnhancedInput|Inventory")
	TObjectPtr<UInputAction>  m_InputPickUp;

	//Panels
	TObjectPtr<UUserWidget> m_InvertoryPanel;

	//Animation
	UPROPERTY()
	bool m_IsRun = false;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipMent")
	TObjectPtr<USkeletalMeshComponent> m_Hair;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipMent")
	TObjectPtr<USkeletalMeshComponent> m_ArmGuard;//护臂
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipMent")
	TObjectPtr<USkeletalMeshComponent> m_ThighGuard;//大腿护臂
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipMent")
	TObjectPtr<USkeletalMeshComponent> m_CalfGuard;//小腿护臂
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipMent")
	TObjectPtr<USkeletalMeshComponent> m_Corselet;//胸甲
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipMent")
	TObjectPtr<USkeletalMeshComponent> m_ArmCorselet;//肩甲
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipMent")
	TObjectPtr<USkeletalMeshComponent> m_Trousers;//裤子
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipMent")
	TObjectPtr<USkeletalMeshComponent> m_Girdle;//腰带
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipMent")
	TObjectPtr<USkeletalMeshComponent> m_Overcoat;//大衣
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipMent")
	TObjectPtr<USkeletalMeshComponent> m_Shoe;//鞋子
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipMent")
	TObjectPtr<USkeletalMeshComponent> m_BodyDecoration;//身体饰品
	
private:
	//EquipMent
	TArray<bool>m_EquipMentsIsEquip;
	TArray<FEqmentData> m_EquipMentsList;
};


