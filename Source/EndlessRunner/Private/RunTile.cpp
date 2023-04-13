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

	DestroyTileTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	DestroyTileTriggerBox->SetupAttachment(RootSceneComponent);
	DestroyTileTriggerBox->SetBoxExtent(FVector(50.f, 500.f, 250.f));
	DestroyTileTriggerBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

// Called when the game starts or when spawned
void ARunTile::BeginPlay()
{
	Super::BeginPlay();

	RunGameMode = Cast<ARunnerGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	check(RunGameMode);

	DestroyTileTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ARunTile::OnDestroyBoxOverlap);
}

// Called every frame
void ARunTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RootSceneComponent->AddLocalOffset(FVector(-10,0,0));
}

void ARunTile::OnDestroyBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEndlessRunnerCharacter* PlayerCharacter = Cast<AEndlessRunnerCharacter>(OtherActor);

	if(PlayerCharacter)
	{
		RunGameMode->AddTile();

		GetWorldTimerManager().SetTimer(DestroyTileHandle, this, &ARunTile::DestroyTile, 2.f, false);
	}
}

void ARunTile::DestroyTile()
{
	if(DestroyTileHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(DestroyTileHandle);
	}

	this->Destroy();
}

