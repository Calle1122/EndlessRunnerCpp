// Fill out your copyright notice in the Description page of Project Settings.

#include "EndlessRunner/Public/RunTile.h"
#include "TimerManager.h"
#include "EndlessRunner/EndlessRunnerCharacter.h"
#include "EndlessRunner/Public/RunnerGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARunTile::ARunTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootSceneComponent;

	TileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	TileMeshComponent->SetupAttachment(RootSceneComponent);

	TileSeamPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("SeamPoint"));
	TileSeamPoint->SetupAttachment(RootSceneComponent);

	for (int i = 0; i < 10 + 1; i++)
	{
		UArrowComponent* NewArrow = CreateDefaultSubobject<UArrowComponent>(FName(FString::FromInt(i)));
		NewArrow->SetupAttachment(RootComponent);
		ProjectilePositions.Add( NewArrow );
	}
}

// Called when the game starts or when spawned
void ARunTile::BeginPlay()
{
	Super::BeginPlay();

	RunGameMode = Cast<ARunnerGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	check(RunGameMode);

	for (UArrowComponent* ProjectilePos : ProjectilePositions)
	{
		int RandomPick = FMath::RandRange(0, 100);
		if(RandomPick<RunGameMode->ProjectilePercentChance)
		{
			//Spawn projectile for target position
			AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(Projectile, ProjectilePos->GetComponentLocation(), FRotator(0,0,0));
			NewProjectile->AttachToComponent(ProjectilePos, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}
}

// Called every frame
void ARunTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RootSceneComponent->AddLocalOffset(FVector(-10,0,0));

	if(TileSeamPoint->GetComponentLocation().X < -750)
	{
		DestroyTile();
	}
}

void ARunTile::DestroyTile()
{
	RunGameMode->OnTileDestroy();
	this->Destroy();
}

