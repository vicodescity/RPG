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

	EDamageStrength DamageStrength;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
		float GetLightDamage() { return WeaponDamage; }

	UFUNCTION(BlueprintPure)
		float GetHeavyDamage() { return WeaponDamage * 2; }
		
	UFUNCTION(BlueprintPure)
		UAnimMontage* GetLightMontage() { return LightAttack;}

	UFUNCTION(BlueprintPure)
		UAnimMontage* GetHeavyMontage() { return HeavyAttack; }


	//This function deducts from the player stamina during an attack
	//it takes a paramater of enum EDamageType 
	//and drains more stamina with an heavy attack then a light attack
	UFUNCTION(BlueprintCallable)
		float GetStaminaDrain(EDamageStrength Strength);
};
