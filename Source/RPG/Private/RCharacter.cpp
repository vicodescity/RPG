// Fill out your copyright notice in the Description page of Project Settings.


#include "RCharacter.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "RStatsComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Engine.h"
#include "GameFramework\Controller.h"
#include "Components/SphereComponent.h"
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
	LockOnSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LockOn"));
	
	LockOnSphere->SetupAttachment(RootComponent);
	LockOnSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LockOnSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	LockOnSphere->InitSphereRadius(700);
	LockOnSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	SpringArm->bUsePawnControlRotation = true;

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
	
	LockOnSphere->OnComponentBeginOverlap.AddDynamic(this, &ARCharacter::OnBeginOverlap);
	LockOnSphere->OnComponentEndOverlap.AddDynamic(this, &ARCharacter::EndOverlap);
}

// Called every frame
void ARCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	PlayerStateManageMent(DeltaTime);
	HandleLockOn();
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

	PlayerInputComponent->BindAction("LockOn", IE_Pressed, this, &ARCharacter::LockOnToEnemy);

	PlayerInputComponent->BindAction("Guard", IE_Pressed, this, &ARCharacter::GuardFromAttack);
	PlayerInputComponent->BindAction("Guard", IE_Released, this, &ARCharacter::GuardFromAttack);
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

	else
	{
		if (PlayState == EPlayerState::Guarding)
		{
			return RealDamage;
		}

		else
		{
			Stats->SetHealth(RealDamage, false);
			return RealDamage;
		}
	}
}

void ARCharacter::Dead()
{
	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage, 1);
	}
	GetCharacterMovement()->DisableMovement();

	
	WeaponSlot->SetActorLocation(this->GetActorLocation());
	WeaponSlot->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	WeaponSlot->DestroyWeapon();
	SetLifeSpan(3);
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

		GetCharacterMovement()->MaxWalkSpeed = FMath::FInterpTo(GetCharacterMovement()->GetMaxSpeed(), SprintSpeed, DeltaSec, 0.5);

		Stats->SetStamina(40, false);
		break;

	case (EPlayerState::Guarding):
		if (WeaponSlot->GetGuardMontage())
		{
			PlayAnimMontage(WeaponSlot->GetGuardMontage());
		}
		break;

	default:
		break;
	}
}

//Weapon Mechanics---------------------
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
			PlayAnimMontage(WeaponSlot->GetHeavyMontage(), 1.3);		
	}
}
//------------------------------------------------


//Lock On Mechanics------------------------------------
void ARCharacter::LockOnToEnemy()
{
	if (IsLockedOn)
	{
		IsLockedOn = false;
		LockedOnEnemy = NULL;
	}

	else
	{
		if (SensedEnemies.Num() >= 1)
		{
			IsLockedOn = true;
			LockedOnEnemy = SensedEnemies[0];
		}
	}
}

void ARCharacter::HandleLockOn()
{
	if (IsLockedOn)
	{
		if (LockedOnEnemy)
		{
			//We only need to modify the yaw of the actor rotation to make the player always face the enemy
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockedOnEnemy->GetActorLocation());

			FRotator NormalRotation = GetActorRotation();

			SetActorRotation(FRotator(NormalRotation.Pitch, LookAtRotation.Yaw, NormalRotation.Roll));
		}
	}
}

void ARCharacter::OnBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		ARCharacter* Temp = Cast<ARCharacter>(OtherActor);
		if (Temp)
		{
			SensedEnemies.AddUnique(Temp);
		}
	}
}

void ARCharacter::EndOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	ARCharacter* Temp = Cast<ARCharacter>(OtherActor);
	if (Temp)
	{
		if (SensedEnemies.Contains(Temp))
		{
			SensedEnemies.Remove(Temp);
		}
	}
}

//------------------------------

//Guard Mechanics---------------
void ARCharacter::GuardFromAttack()
{
	if (PlayState != EPlayerState::Guarding)
	{
		StopAnimMontage();
		PlayState = EPlayerState::Guarding;
	}

	else
	{
		PlayState = EPlayerState::Idle;
	}
}
//------------------------------