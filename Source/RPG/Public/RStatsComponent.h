// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RStatsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API URStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URStatsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//The health Variable
	float Health;

	//The Stamina Variable
	float Stamina;

	//A direct reference(pointer) to the player
	class ARCharacter* Player;

	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	//Reduces or adds to the health depending on the boolean
	//True adds
	//False reduces
	void SetHealth(float Amount, bool Value);

	//Reduces or adds to the stamina depending on the boolean
	//True adds
	//False reduces
	void SetStamina(float Amount, bool Value);

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float MaxHealth = 100;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float MaxStamina = 100;


	UFUNCTION(BlueprintPure)
		float GetHealth() { return Health; }

	UFUNCTION(BlueprintPure)
		float GetStamina() { return Stamina; }
		

};
