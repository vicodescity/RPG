// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = WeaponMesh;
	
	DetectionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision_Detector"));
	DetectionCapsule->SetupAttachment(RootComponent);
	DetectionCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AWeapon::GetStaminaDrain(EDamageStrength Strength)
{
	float Initial;
	float Final;

	switch (Strength)
	{
	case (EDamageStrength::LightAttack):
		Initial = 100 / WeaponDamage;
		Final = Initial * 10;
		return Final;
		break;

	case (EDamageStrength::HeavyAttack):
		Initial = 100 / WeaponDamage;
		Final = Initial * 10;
		return Final * 2;
		break;

	default:
		return 0;
		break;
	}
}