// Fill out your copyright notice in the Description page of Project Settings.


#include "RunHitBox.h"

#include "RunnerGameModeBase.h"
#include "EndlessRunner/EndlessRunnerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARunHitBox::ARunHitBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	DamageBox->SetBoxExtent(FVector(50.f, 500.f, 250.f));
	DamageBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

void ARunHitBox::LuckyDestroy()
{
	TArray<UActorComponent*> ActorsToDestroy;
	
	for (UActorComponent* connectedActor : ConnectedObjects)
	{
		ActorsToDestroy.Add(connectedActor);
	}

	for(UActorComponent* destroyActor : ActorsToDestroy)
	{
		destroyActor->DestroyComponent();
	}
	
	this->Destroy();
}

void ARunHitBox::OnDamageBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEndlessRunnerCharacter* PlayerCharacter = Cast<AEndlessRunnerCharacter>(OtherActor);

	if(PlayerCharacter)
	{
		if(PlayerCharacter->Tags.Contains(FName("p1")))
		{
			RunGameMode->ReduceHealth(0);
		}
		else
		{
			RunGameMode->ReduceHealth(1);
		}
	}
}

// Called when the game starts or when spawned
void ARunHitBox::BeginPlay()
{
	Super::BeginPlay();
	
	RunGameMode = Cast<ARunnerGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	check(RunGameMode);

	DamageBox->OnComponentBeginOverlap.AddDynamic(this, &ARunHitBox::OnDamageBoxOverlap);
}

// Called every frame
void ARunHitBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

