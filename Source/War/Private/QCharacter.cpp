// Fill out your copyright notice in the Description page of Project Settings.


#include "QCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "QInvertoryPanel.h"
#include "QInventory.h"
#include "QAbilityItme.h"

const int EquipMentSlots = 11;


AQCharacter::AQCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	m_Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	m_CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	m_CameraBoom->SetupAttachment(GetRootComponent());
	m_Camera->SetupAttachment(m_CameraBoom, USpringArmComponent::SocketName);
	m_Camera->bUsePawnControlRotation = false;
	m_CameraBoom->TargetArmLength = 300.0f;
	m_CameraBoom->bEnableCameraLag = false;
	m_CameraBoom->CameraLagSpeed = 0;
	m_CameraBoom->SetRelativeRotation(FRotator(-45.0, 0, 0));
	m_CameraBoom->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//Roator With Controller
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;

	//Max Speed
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 300.f;

	// enable crouching
	if (GetMovementComponent())
	{
		GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	}


	m_EquipMentsList.SetNum(EquipMentSlots);
	m_EquipMentsIsEquip.SetNum(EquipMentSlots);


	InitSkMeshConstruction();
}
void AQCharacter::BeginPlay()
{
	Super::BeginPlay();
	TObjectPtr<UWorld> const world = GetWorld();
	if (UClass* invertoryPanelClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Test/UI_InventoryPanel.UI_InventoryPanel_c'"))) {
		UE_LOG(LogTemp, Warning, TEXT("dasdasd hello dsadads"));
		if (APlayerController* pc = world->GetFirstPlayerController()) {
			m_InvertoryPanel = CreateWidget(pc, invertoryPanelClass);
			if (m_InvertoryPanel) {
				m_InvertoryPanel->AddToViewport();
				m_InvertoryPanel->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
	InitSkMeshBeginPlay();
}
void AQCharacter::OnViewYawInputActionTriggered(const FInputActionValue& Value)
{
	AddControllerYawInput(Value.Get<float>());
}
void AQCharacter::OnViewPitchInputActionTriggered(const FInputActionValue& Value)
{
	AddControllerPitchInput(Value.Get<float>());
}
void AQCharacter::OnMoveForwardInputActionTriggered(const FInputActionValue& Value)
{
	FRotator ControllerRotator = GetActorRotation();
	ControllerRotator.Yaw = GetControlRotation().Yaw;
	SetActorRotation(ControllerRotator);
	AddMovementInput(GetActorForwardVector(), Value.Get<float>());
}
void AQCharacter::OnMoveRightInputActionTriggered(const FInputActionValue& Value)
{
	AddMovementInput(GetActorRightVector(), Value.Get<float>());

}
void AQCharacter::OnMoveZoomInputActionTriggered(const FInputActionValue& Value)
{
	m_CameraBoom->TargetArmLength -= Value.Get<float>() * 10.f;
	m_CameraBoom->TargetArmLength = FMath::Clamp(m_CameraBoom->TargetArmLength, 300.0f, 800.0f);
}
void AQCharacter::OnJumpInputActionStarted(const FInputActionValue& Value)
{
	if (CanJump())
		Jump();
}
void AQCharacter::OnCrouchInputActionTriggered(const FInputActionValue& Value)
{
	Crouch();
}
void AQCharacter::OnCrouchInputActionCompleted(const FInputActionValue& Value)
{
	
	UnCrouch();
}
void AQCharacter::OnRunInputActionTriggered(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	m_IsRun = true;
}
void AQCharacter::OnRunInputActionCompleted(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	m_IsRun = false;
}
void AQCharacter::OnOpenInventoryPanelActionStarted(const FInputActionValue& Value)
{
	static bool IsOpen = false;
	if (IsOpen) {
		m_InvertoryPanel->SetVisibility(ESlateVisibility::Collapsed);
		IsOpen = false;
		CloseUIInput();
	}
	else {
		m_InvertoryPanel->SetVisibility(ESlateVisibility::Visible);
		IsOpen = true;
		OpenUIInput(m_InvertoryPanel);
	}
}
void AQCharacter::OnPcikUpActionStarted(const FInputActionValue& Value)
{
	FVector traceStartPosition = m_Camera->GetComponentToWorld().GetLocation();
	traceStartPosition += m_Camera->GetForwardVector() * m_CameraBoom->TargetArmLength;
	FVector traceEndPosition = traceStartPosition + m_Camera->GetForwardVector() * 700.f;
	FHitResult hitRes;
	UKismetSystemLibrary::SphereTraceSingle(this, traceStartPosition, traceEndPosition, 50.f,
		ETraceTypeQuery::TraceTypeQuery1, false, TArray<AActor*>(), EDrawDebugTrace::ForDuration,
		hitRes, true);
	if (TObjectPtr <AActor> hitActor = hitRes.GetActor()) {
		if (TObjectPtr <AQInventory> item = Cast<AQInventory>(hitActor)) {
			PickUpItem(item);
		}
	}
}
void AQCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void AQCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if(APlayerController * PlayerController = CastChecked<APlayerController>(GetController()))
	{
		 if(UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem =
		 	ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		 {
			 EnhancedInputLocalPlayerSubsystem->AddMappingContext(m_InputMappingContext,0);
		 }
	}
	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Locomotion Bind
		EnhancedInputComponent->BindAction(m_InputControllerYaw,ETriggerEvent::Triggered,this,&AQCharacter::OnViewYawInputActionTriggered);
		EnhancedInputComponent->BindAction(m_InputControllerPitch,ETriggerEvent::Triggered,this,&AQCharacter::OnViewPitchInputActionTriggered);
		EnhancedInputComponent->BindAction(m_InputMoveForward,ETriggerEvent::Triggered,this,&AQCharacter::OnMoveForwardInputActionTriggered);
		EnhancedInputComponent->BindAction(m_InputMoveRight,ETriggerEvent::Triggered,this,&AQCharacter::OnMoveRightInputActionTriggered);
		EnhancedInputComponent->BindAction(m_InputZoom, ETriggerEvent::Triggered, this, &AQCharacter::OnMoveZoomInputActionTriggered);
		EnhancedInputComponent->BindAction(m_InputJump, ETriggerEvent::Started, this, &AQCharacter::OnJumpInputActionStarted);
		EnhancedInputComponent->BindAction(m_InputCrouch, ETriggerEvent::Triggered, this, &AQCharacter::OnCrouchInputActionTriggered);
		EnhancedInputComponent->BindAction(m_InputCrouch, ETriggerEvent::Completed, this, &AQCharacter::OnCrouchInputActionCompleted);
		EnhancedInputComponent->BindAction(m_InputRun, ETriggerEvent::Triggered, this, &AQCharacter::OnRunInputActionTriggered);
		EnhancedInputComponent->BindAction(m_InputRun, ETriggerEvent::Completed, this, &AQCharacter::OnRunInputActionCompleted);

		EnhancedInputComponent->BindAction(m_InputOpenInventoryPanel, ETriggerEvent::Started, this, &AQCharacter::OnOpenInventoryPanelActionStarted);
		EnhancedInputComponent->BindAction(m_InputPickUp, ETriggerEvent::Started, this, &AQCharacter::OnPcikUpActionStarted);
	}
}
void AQCharacter::PickUpItem(TObjectPtr<AQInventory> inventory)
{
	if (TObjectPtr<AQEquipMent>equipMent = Cast<AQEquipMent>(inventory)) {
		EquipItem(equipMent);
	}
	else if (TObjectPtr <AQAbilityItme> item = Cast<AQAbilityItme>(inventory)) {
		TSharedRef<FAbilityItme> itemPtr = MakeShareable(new FAbilityItme());
		*itemPtr = *item;
		for (int i = 0; i < EquipMentSlots; i++) {
			if (m_EquipMentsIsEquip[i] && 
				m_EquipMentsList[i].CurrentCapacity + inventory->m_Volume <= m_EquipMentsList[i].MaxCapacity) {
				m_EquipMentsList[i].ItemsSet.Add(itemPtr);
				item->Destroy();
				return;
			}
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("ÎïÆ·À¸Âú")));

	}
}
void AQCharacter::EquipItem(TObjectPtr<AQEquipMent> equipMent)
{
	if (m_EquipMentsIsEquip[(int)equipMent->m_Type]) {
		for (auto& item : m_EquipMentsList[(int)equipMent->m_Type].ItemsSet) {
			DropItem(item);
		}
	}
	else {
		m_EquipMentsIsEquip[(int)equipMent->m_Type] = true;
	}
	m_EquipMentsList[(int)equipMent->m_Type] = *equipMent;
	
	switch (equipMent->m_Type)
	{
	case EEquipMent::EQM_ArmGuard:		OnPlayEquipMentSomething(EEquipMent::EQM_ArmGuard, equipMent->m_EquipMentMesh->GetSkeletalMeshAsset()); EquipItem(m_ArmGuard, equipMent); return;
	case EEquipMent::EQM_ThighGuard:	OnPlayEquipMentSomething(EEquipMent::EQM_ThighGuard, equipMent->m_EquipMentMesh->GetSkeletalMeshAsset()); EquipItem(m_ThighGuard, equipMent); return;
	case EEquipMent::EQM_CalfGuard:		OnPlayEquipMentSomething(EEquipMent::EQM_CalfGuard, equipMent->m_EquipMentMesh->GetSkeletalMeshAsset()); EquipItem(m_CalfGuard, equipMent); return;
	case EEquipMent::EQM_Corselet:		OnPlayEquipMentSomething(EEquipMent::EQM_Corselet, equipMent->m_EquipMentMesh->GetSkeletalMeshAsset()); EquipItem(m_Corselet, equipMent); return;
	case EEquipMent::EQM_ArmCorselet:	OnPlayEquipMentSomething(EEquipMent::EQM_ArmCorselet, equipMent->m_EquipMentMesh->GetSkeletalMeshAsset()); EquipItem(m_ArmCorselet, equipMent); return;
	case EEquipMent::EQM_Trousers:		OnPlayEquipMentSomething(EEquipMent::EQM_Trousers, equipMent->m_EquipMentMesh->GetSkeletalMeshAsset()); EquipItem(m_Trousers, equipMent); return;
	case EEquipMent::EQM_Girdle:		OnPlayEquipMentSomething(EEquipMent::EQM_Girdle, equipMent->m_EquipMentMesh->GetSkeletalMeshAsset()); EquipItem(m_Girdle, equipMent); return;
	case EEquipMent::EQM_Overcoat:		OnPlayEquipMentSomething(EEquipMent::EQM_Overcoat, equipMent->m_EquipMentMesh->GetSkeletalMeshAsset()); EquipItem(m_Overcoat, equipMent); return;
	case EEquipMent::EQM_Shoe:			OnPlayEquipMentSomething(EEquipMent::EQM_Shoe, equipMent->m_EquipMentMesh->GetSkeletalMeshAsset()); EquipItem(m_Shoe, equipMent); return;
	case EEquipMent::EQM_BodyDecoration:OnPlayEquipMentSomething(EEquipMent::EQM_BodyDecoration, equipMent->m_EquipMentMesh->GetSkeletalMeshAsset()); EquipItem(m_BodyDecoration, equipMent); return;
	}
}
void AQCharacter::EquipItem(TObjectPtr<USkeletalMeshComponent> SkMesh, TObjectPtr<AQEquipMent> equipMent)
{
	
	SkMesh->SetSkeletalMeshAsset(equipMent->m_EquipMentMesh->GetSkeletalMeshAsset());
	equipMent->Destroy(); 
	return;
}
void AQCharacter::InitSkMeshConstruction()
{
	m_Hair = CreateDefaultSubobject<USkeletalMeshComponent>("Hair");
	m_ArmGuard = CreateDefaultSubobject<USkeletalMeshComponent>("ArmGuard");
	m_ThighGuard = CreateDefaultSubobject<USkeletalMeshComponent>("ThighGuard");
	m_CalfGuard = CreateDefaultSubobject<USkeletalMeshComponent>("CalfGuard");
	m_Corselet = CreateDefaultSubobject<USkeletalMeshComponent>("Corselet");
	m_ArmCorselet = CreateDefaultSubobject<USkeletalMeshComponent>("ArmCorselet");
	m_Trousers = CreateDefaultSubobject<USkeletalMeshComponent>("Trousers");
	m_Girdle = CreateDefaultSubobject<USkeletalMeshComponent>("Girdle");
	m_Overcoat = CreateDefaultSubobject<USkeletalMeshComponent>("Overcoat");
	m_Shoe = CreateDefaultSubobject<USkeletalMeshComponent>("Shoe");
	m_BodyDecoration = CreateDefaultSubobject<USkeletalMeshComponent>("BodyDecoration");
	m_Hair->SetupAttachment(GetMesh());
	m_ArmGuard->SetupAttachment(GetMesh());
	m_ThighGuard->SetupAttachment(GetMesh());
	m_CalfGuard->SetupAttachment(GetMesh());
	m_Corselet->SetupAttachment(GetMesh());
	m_ArmCorselet->SetupAttachment(GetMesh());
	m_Trousers->SetupAttachment(GetMesh());
	m_Girdle->SetupAttachment(GetMesh());
	m_Overcoat->SetupAttachment(GetMesh());
	m_Shoe->SetupAttachment(GetMesh());
	m_BodyDecoration->SetupAttachment(GetMesh());
}
void AQCharacter::InitSkMeshBeginPlay()
{
	m_ArmGuard->SetAnimInstanceClass(GetMesh()->GetAnimClass());
	m_ThighGuard->SetAnimInstanceClass(GetMesh()->GetAnimClass());
	m_CalfGuard->SetAnimInstanceClass(GetMesh()->GetAnimClass());
	m_Corselet->SetAnimInstanceClass(GetMesh()->GetAnimClass());
	m_ArmCorselet->SetAnimInstanceClass(GetMesh()->GetAnimClass());
	m_Trousers->SetAnimInstanceClass(GetMesh()->GetAnimClass());
	m_Girdle->SetAnimInstanceClass(GetMesh()->GetAnimClass());
	m_Overcoat->SetAnimInstanceClass(GetMesh()->GetAnimClass());
	m_Shoe->SetAnimInstanceClass(GetMesh()->GetAnimClass());
	m_BodyDecoration->SetAnimInstanceClass(GetMesh()->GetAnimClass());
}
void AQCharacter::OpenUIInput(TObjectPtr<UUserWidget> WidgetFouces)
{
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(GetWorld()->GetFirstPlayerController(), WidgetFouces);
}
void AQCharacter::CloseUIInput()
{
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetWorld()->GetFirstPlayerController());
}
void AQCharacter::DropItem(TSharedPtr<FItemDataBase> item)
{
	UE_LOG(LogTemp, Warning, TEXT("hello"));
}
TArray<TSharedPtr<FItemDataBase>> AQCharacter::GetItemsList()
{
	TArray<TSharedPtr<FItemDataBase>> ItemsList;
	for (int i = 1; i < EquipMentSlots; i++) {
		if (m_EquipMentsIsEquip[i])ItemsList.Append(m_EquipMentsList[i].ItemsSet);
	}
	return ItemsList;
}
TArray<TSharedPtr<FItemDataBase>> AQCharacter::GetAroundPlayItems()
{
	TArray<FHitResult> hitMultiRes;
	TArray<TSharedPtr<FItemDataBase>>Items;
	TMap<TObjectPtr<AActor>, bool>completeActor;
	UKismetSystemLibrary::SphereTraceMulti(this, GetActorLocation(), GetActorLocation(), 100.f,
		ETraceTypeQuery::TraceTypeQuery1, false, TArray<AActor*>(), EDrawDebugTrace::None,
		hitMultiRes, true);
	for (auto& hitRes : hitMultiRes) {
		if (TObjectPtr <AActor> hitActor = hitRes.GetActor()) {
			if (completeActor.Contains(hitActor))continue;
			completeActor.Add(hitActor, true);
			if (Cast<AQInventory>(hitActor)) {
				if (TObjectPtr <AQEquipMent> item = Cast<AQEquipMent>(hitActor)) {
					TSharedPtr<FEqmentData> itemPtr(MakeShareable(new FEqmentData()));
					*itemPtr = *item;
					Items.Add(itemPtr);
				}
				if (TObjectPtr <AQAbilityItme> item = Cast<AQAbilityItme>(hitActor)) {
					TSharedPtr<FAbilityItme> itemPtr(MakeShareable(new FAbilityItme()));
					*itemPtr = *item;
					Items.Add(itemPtr);
				}
			}
		}
	}
	return Items;
}


