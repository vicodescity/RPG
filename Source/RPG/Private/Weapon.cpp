// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "RCharacter.h"


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
	DetectionCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	DetectionCapsule->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::BeginOverlap);
	DetectionCapsule->OnComponentEndOverlap.AddDynamic(this, &AWeapon::EndOverlap);
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

void AWeapon::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& FromSweep)
{
	if (OtherActor && OtherActor != this && OtherActor != GetOwner())
	{
		if (AppliedDamage > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("%f"), AppliedDamage);
			ApplyDamage(AppliedDamage, OtherActor);
		}
	}
}

void AWeapon::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}


void AWeapon::ApplyDamage(float DamageToApply, AActor* DamagedActor)
{
	
	if (DamageToApply > 0 && DamagedActor)
	{
		ARCharacter* Character = Cast<ARCharacter>(GetOwner());
		
		if (Character)
		{
			DamagedActor->TakeDamage(DamageToApply, FDamageEvent(), Character->Controller, this);
		}

	}

}

void AWeapon::DestroyWeapon()
{
	this->SetOwner(NULL);
	DetectionCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	WeaponMesh->SetSimulatePhysics(true);
}