// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EDamageStrength : uint8
{
	LightAttack,

	HeavyAttack,
};

UCLASS()
class RPG_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UCapsuleComponent* DetectionCapsule;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Details")
		float WeaponDamage = 35;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
		UAnimMontage* LightAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
		UAnimMontage* HeavyAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Guarding")
		UAnimMontage* GuardMontage;

	EDamageStrength DamageStrength;

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//The damage that Weapon inflicts on the enemy
	float AppliedDamage;

	//Responsible for Applying damage from the weapon to the enemy of its owner
	//@ Param The amount of damage to apply on enemy
	//@ Param The Actor to apply the damage to
	void ApplyDamage(float DamageToApply, AActor* DamagedActor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Gets Light Attack Montage
	UFUNCTION(BlueprintPure)
		float GetLightDamage() { return WeaponDamage; }

	//Gets Heavy attack animation Montage
	UFUNCTION(BlueprintPure)
		float GetHeavyDamage() { return WeaponDamage * 2; }
		
	//Gets Montage for the light attack
	UFUNCTION(BlueprintPure)
		UAnimMontage* GetLightMontage() { return LightAttack;}

	//Gets the Montage for heavy attack
	UFUNCTION(BlueprintPure)
		UAnimMontage* GetHeavyMontage() { return HeavyAttack; }

	//Gets the Montage for guard/blocking
	UFUNCTION(BlueprintPure)
		UAnimMontage* GetGuardMontage() { return GuardMontage; }


	//This function deducts from the player stamina during an attack
	//it takes a paramater of enum EDamageType 
	//and drains more stamina with an heavy attack then a light attack
	UFUNCTION(BlueprintCallable)
		float GetStaminaDrain(EDamageStrength Strength);

	UFUNCTION(BlueprintPure)
		UCapsuleComponent* GetWeaponCollisionBox() { return DetectionCapsule; }

	//Sets the variable of AppliedDamage
	//@ Param The Damage to be assigned to AppliedDamage float variable
	UFUNCTION(BlueprintCallable)
		void SetAppliedDamage(float NewDamage) { AppliedDamage = NewDamage; }
};
