// Fill out your copyright notice in the Description page of Project Settings.


#include "RStatsComponent.h"
#include "Engine/World.h"
#include "RCharacter.h"


URStatsComponent::URStatsComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

}

void URStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	//The player pointer allows us to access the function of the owner as ARCharacter
	if (GetOwner())
	{
		Player = Cast<ARCharacter>(GetOwner());
	}

	Stamina = MaxStamina;
	Health = MaxHealth;
	
}

void URStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void URStatsComponent::SetHealth(float Amount, bool Value)
{
	if (Value)
	{
		if (Player->GetPlayerState() == EPlayerState::Idle)
		{
			float NewHealth = Health + Amount;
			Health = FMath::FInterpTo(Health, NewHealth, GetWorld()->GetDeltaSeconds(), 1.5);
		}

		else
		{
			Health += Amount;
		}
	}

	else
	{
		Health -= Amount;

		if (Health <= 0)
		{
			Player->Dead();
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("%f"), Health);
}

void URStatsComponent::SetStamina(float Amount, bool Value)
{
	float NewStamina = 0;

	if (Value)
	{
		switch (Player->GetPlayerState())
		{
		case (EPlayerState::Idle):
			NewStamina = Stamina + Amount;
			Stamina = FMath::FInterpTo(Stamina, NewStamina, GetWorld()->GetDeltaSeconds(), 0.5);

			break;

		default:
			return;
			break;
		}
	}

	else
	{
		switch (Player->GetPlayerState())
		{
		case (EPlayerState::Sprint):
			NewStamina = Stamina - Amount;

			Stamina = FMath::FInterpTo(Stamina, NewStamina, GetWorld()->GetDeltaSeconds(), 0.2);

			break;

		case (EPlayerState::Attacking):
			
			Stamina -= Amount;
			
			break;

		default:
			return;
			break;
		}

	}
}