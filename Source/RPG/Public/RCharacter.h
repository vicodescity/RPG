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

	Guarding,
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

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USphereComponent* LockOnSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Death")
		UAnimMontage* DeathMontage;

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

	//FOr the locked on enemy
	ARCharacter* LockedOnEnemy;

	//boolean to determine if the player/ai is locked on
	bool IsLockedOn;

	//Holds all characters that the player or AI can Lock On To
	TArray<ARCharacter*>SensedEnemies;

	//The slot for weapon
	AWeapon* WeaponSlot;

	void Sprint();

	void StopSprint();
	
	UFUNCTION(BlueprintCallable)
	void LightAttack();

	UFUNCTION(BlueprintCallable)
	void HeavyAttack();

	//This function manages the stamina in regards to the players current state
	void PlayerStateManageMent(float DeltaSec);

	UFUNCTION()
	void LockOnToEnemy();

	UFUNCTION()
	void GuardFromAttack();

	//This funnction will be run in the event tick function
	void HandleLockOn();

	//Overlap functions for LockOnBox
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

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

	UFUNCTION(BlueprintPure)
		AWeapon* GetWeaponSlot() { return WeaponSlot; }
	
};
