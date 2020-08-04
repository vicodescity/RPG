// Fill out your copyright notice in the Description page of Project Settings.


#include "RCharacter.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "RStatsComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework\SpringArmComponent.h"

// Sets default values
ARCharacter::ARCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	Stats = CreateDefaultSubobject<URStatsComponent>(TEXT("Stats"));

	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	SpringArm->bUsePawnControlRotation = true;

	bUseControllerRotationYaw = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 300;

}

// Called when the game starts or when spawned
void ARCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayState = EPlayerState::Idle;
	
	if (Weapon)
	{
		FActorSpawnParameters params;
		params.Owner = this;
		params.Instigator = Instigator;

		WeaponSlot = GetWorld()->SpawnActor<AWeapon>(Weapon, params);
		WeaponSlot->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Weapon");

	}
}

// Called every frame
void ARCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	PlayerStateManageMent(DeltaTime);

}

// Called to bind functionality to input
void ARCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ARCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &ARCharacter::Turn);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ARCharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ARCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ARCharacter::StopSprint);

	PlayerInputComponent->BindAction("LightAttack", IE_Pressed, this, &ARCharacter::LightAttack);
	PlayerInputComponent->BindAction("HeavyAttack", IE_Pressed, this, &ARCharacter::HeavyAttack);
}

void ARCharacter::MoveForward(float Scale)
{
	if (!Controller)
	{
		return;
	}

	if (PlayState == EPlayerState::Attacking)
	{
		return;
	}

	FRotator Rotation = Controller->GetControlRotation();

	FRotator Yaw(0, Rotation.Yaw, 0);

	FVector Direction = FRotationMatrix(Yaw).GetUnitAxis(EAxis::X);

	AddMovementInput(Direction, Scale);


}

void ARCharacter::MoveRight(float Scale)
{
	if (!Controller)
	{
		return;
	}

	if (PlayState == EPlayerState::Attacking)
	{
		return;
	}

	FRotator Rotation = Controller->GetControlRotation();

	FRotator Yaw(0, Rotation.Yaw, 0);

	FVector Direction = FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y);

	AddMovementInput(Direction, Scale);

}

void ARCharacter::LookUp(float Axis)
{
	AddControllerPitchInput(Axis);
}

void ARCharacter::Turn(float Axis)
{
	AddControllerYawInput(Axis);
}

float ARCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* DamageInstigator, AActor* DamageCauser)
{
	float RealDamage = Super::TakeDamage(Damage, DamageEvent, DamageInstigator, DamageCauser);
	
	if (RealDamage <= 0)
	{
		return RealDamage;
	}

	Stats->SetHealth(RealDamage, false);

	return RealDamage;
}

void ARCharacter::Dead()
{
	Destroy();
}

void ARCharacter::Sprint()
{
	PlayState = EPlayerState::Sprint;
}

void ARCharacter::StopSprint()
{
	PlayState = EPlayerState::Idle;
}

void ARCharacter::PlayerStateManageMent(float DeltaSec)
{
	float Drain;
	switch (PlayState)
	{

	case (EPlayerState::Idle):
		GetCharacterMovement()->bOrientRotationToMovement = false;
		bUseControllerRotationYaw = true;

		GetCharacterMovement()->MaxWalkSpeed = FMath::FInterpTo(GetCharacterMovement()->GetMaxSpeed(), WalkSpeed, DeltaSec, 0.2);

		if (Stats->GetStamina() < Stats->MaxStamina)
		{
			Stats->SetStamina(5, true);
		}
		break;

	case (EPlayerState::Sprint):
		if (Stats->GetStamina() <= 0)
		{
			return;
		}

		GetCharacterMovement()->bOrientRotationToMovement = true;
		bUseControllerRotationYaw = false;

		GetCharacterMovement()->MaxWalkSpeed = FMath::FInterpTo(GetCharacterMovement()->GetMaxSpeed(), SprintSpeed, DeltaSec, 0.2);

		Stats->SetStamina(40, false);
		break;
	
	case (EPlayerState::Attacking):
		Drain = WeaponSlot->GetStaminaDrain(EDamageStrength::LightAttack);
		Stats->SetStamina(Drain, false);
		bUseControllerRotationYaw = false;
		break;

	default:
		break;
	}

}

void ARCharacter::LightAttack()
{
	if (PlayState != EPlayerState::Attacking && WeaponSlot)
	{
		
		PlayAnimMontage(WeaponSlot->GetLightMontage(), 1.3);
	
	}
}

void ARCharacter::HeavyAttack()
{
	if (PlayState != EPlayerState::Attacking && WeaponSlot)
	{
		if (Stats->GetStamina() > WeaponSlot->GetStaminaDrain(EDamageStrength::HeavyAttack))
		{
			PlayAnimMontage(WeaponSlot->GetHeavyMontage(), 1.3);
			float Drain = WeaponSlot->GetStaminaDrain(EDamageStrength::HeavyAttack);
			Stats->SetStamina(Drain, false);
		}

	}
}