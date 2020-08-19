// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameChangers.generated.h"

UENUM(BlueprintType)
enum class EEffectType :uint8
{
	//Sucks away the players health
	Health,
};

UCLASS()
class RPG_API AGameChangers : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameChangers();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* VisibleMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* EffectRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect Modifiers")
		float EffectorRate = 20;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	EEffectType Effect;

	FTimerHandle EffectHandle;
	
	class ARCharacter* SensedCharacter;

	UFUNCTION()
	void EffectBegin();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& FromSweep);

	UFUNCTION()
		void EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
