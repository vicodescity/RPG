// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "GameChangers.h"
#include "TimerManager.h"
#include "Components/BillboardComponent.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Volume = CreateDefaultSubobject<UBoxComponent>(TEXT("Volume"));
	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));

	RootComponent = Volume;
	Volume->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Billboard->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(SpawnHandle, this, &ASpawnVolume::SpawnChangers, SpawnRate, true);
	
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnVolume::SpawnChangers()
{
	if (GameChanger)
	{
		FVector RandomVector = Volume->GetScaledBoxExtent();

		float X = FMath::FRandRange(RandomVector.X * -1, RandomVector.X);
		float Y = FMath::FRandRange(RandomVector.Y * -1, RandomVector.Y);
		float Z = FMath::FRandRange(RandomVector.Z * -1, RandomVector.Z);

		FVector FakeVector = FVector(X, Y, Z);

		FVector SpawnVector = FakeVector + GetActorLocation();
		FActorSpawnParameters params;

		AGameChangers* SpawnedChanger = GetWorld()->SpawnActor<AGameChangers>(GameChanger, SpawnVector, FRotator(), params);
	}
}