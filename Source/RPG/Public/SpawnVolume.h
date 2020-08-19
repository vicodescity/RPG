// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class RPG_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBoxComponent* Volume;

	//This is used just to show the actor properly
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBillboardComponent* Billboard;
	
	//What to spawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Effects")
		TSubclassOf<class AGameChangers> GameChanger;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Effects")
		float SpawnRate = 5;

	FTimerHandle SpawnHandle; 

	UFUNCTION()
	void SpawnChangers();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
