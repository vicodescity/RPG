// Fill out your copyright notice in the Description page of Project Settings.


#include "GameChangers.h"
#include "RCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AGameChangers::AGameChangers()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VisibleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visible Mesh"));
	EffectRadius = CreateDefaultSubobject<USphereComponent>(TEXT("Effect Radius"));

	RootComponent = EffectRadius;
	EffectRadius->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	EffectRadius->InitSphereRadius(150);
	EffectRadius->SetSimulatePhysics(true);

	VisibleMesh->SetupAttachment(RootComponent);
	VisibleMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	
}

// Called when the game starts or when spawned
void AGameChangers::BeginPlay()
{
	Super::BeginPlay();
	
	EffectRadius->OnComponentBeginOverlap.AddDynamic(this, &AGameChangers::BeginOverlap);
	EffectRadius->OnComponentEndOverlap.AddDynamic(this, &AGameChangers::EndOverlap);
}

// Called every frame
void AGameChangers::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameChangers::BeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & FromSweep)
{
	if (OtherActor && OtherActor != this)
	{
		
		ARCharacter* Target = Cast<ARCharacter>(OtherActor);
		if (Target)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found 1"));
			SensedCharacter = Target;
			GetWorld()->GetTimerManager().SetTimer(EffectHandle, this, &AGameChangers::EffectBegin, 1, true);
		}
	}
}

void AGameChangers::EndOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	GetWorld()->GetTimerManager().ClearTimer(EffectHandle);
}

void AGameChangers::EffectBegin()
{
	if (SensedCharacter)
	{
		switch (Effect)
		{

		case EEffectType::Health:
			SensedCharacter->TakeDamage(EffectorRate, FDamageEvent(), NULL, this);
			break;

		default:
			break;
		}
	}
}