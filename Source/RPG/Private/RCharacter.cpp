// Fill out your copyright notice in the Description page of Project Settings.


#include "RCharacter.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework\SpringArmComponent.h"

// Sets default values
ARCharacter::ARCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));

	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	SpringArm->bUsePawnControlRotation = true;

	bUseControllerRotationYaw = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 600;

}

// Called when the game starts or when spawned
void ARCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

}

void ARCharacter::MoveForward(float Scale)
{
	if (!Controller)
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