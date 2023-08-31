// Fill out your copyright notice in the Description page of Project Settings.


#include "QEquipMent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"

AQEquipMent::AQEquipMent()
{
	m_EquipMentMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Sk_Mesh");
}
