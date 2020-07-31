// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "GameFramework/Character.h"
#include "RCharacter.generated.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Idle,

	Sprint,

	Attacking,
};


UCLASS()
class RPG_API ARCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class URStatsComponent* Stats;

	//For Spawning purposes only
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<AWeapon>Weapon;

	//Handles the players current speed which includes idle, sprinting and attacking
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EPlayerState PlayState;
	
	//Is assigned to the max walk speed when sprinting
	float SprintSpeed = 600;

	//Is assigned to the max walk speed when walking
	float WalkSpeed = 300;

	//The slot for weapon
	AWeapon* WeaponSlot;

	void Sprint();

	void StopSprint();
	
	void LightAttack();

	void HeavyAttack();

	//This function manages the stamina in regards to the players current state
	void PlayerStateManageMent(float DeltaSec);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void MoveForward(float Value);

	UFUNCTION()
		void MoveRight(float Value);

	UFUNCTION()
		void LookUp(float Axis);

	UFUNCTION()
		void Turn(float Axis);

	UFUNCTION(BlueprintPure)
		URStatsComponent* GetStats() { return Stats; }

	UFUNCTION(BlueprintPure)
		EPlayerState GetPlayerState() { return PlayState; }

	//The Take damage function
	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* DamageInstigator, AActor* DamageCauser);

	void Dead();

};
