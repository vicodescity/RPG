// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	EPlayerState PlayerState;

	float SprintSpeed = 600;
	float WalkSpeed = 300;

	void Sprint();

	void StopSprint();
	
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
		EPlayerState GetPlayerState() { return PlayerState; }

	//The Take damage function
	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* DamageInstigator, AActor* DamageCauser);

	void Dead();

};
