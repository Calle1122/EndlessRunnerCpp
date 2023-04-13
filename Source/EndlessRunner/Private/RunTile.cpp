// Fill out your copyright notice in the Description page of Project Settings.

#include "EndlessRunner/Public/RunTile.h"

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
}

// Called when the game starts or when spawned
void ARunTile::BeginPlay()
{
	Super::BeginPlay();

	RunGameMode = Cast<ARunnerGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	check(RunGameMode);
}

// Called every frame
void ARunTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

