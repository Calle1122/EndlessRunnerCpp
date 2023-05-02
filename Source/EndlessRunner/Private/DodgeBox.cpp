// Fill out your copyright notice in the Description page of Project Settings.


#include "DodgeBox.h"

#include "RunnerGameModeBase.h"
#include "EndlessRunner/EndlessRunnerCharacter.h"

// Sets default values
ADodgeBox::ADodgeBox()
{
	PrimaryActorTick.bCanEverTick = true;

	DodgeBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	DodgeBoxComp->SetBoxExtent(FVector(50.f, 500.f, 250.f));
	DodgeBoxComp->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

}

void ADodgeBox::OnDamageBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEndlessRunnerCharacter* PlayerCharacter = Cast<AEndlessRunnerCharacter>(OtherActor);

	if(PlayerCharacter)
	{
		if(PlayerCharacter->Tags.Contains(FName("p1")))
		{
			RunGameMode->P1Dodge();
		}
		else
		{
			RunGameMode->P2Dodge();
		}
	}
}

// Called when the game starts or when spawned
void ADodgeBox::BeginPlay()
{
	Super::BeginPlay();

	RunGameMode = Cast<ARunnerGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	check(RunGameMode);

	DodgeBoxComp->OnComponentBeginOverlap.AddDynamic(this, &ADodgeBox::OnDamageBoxOverlap);
}

// Called every frame
void ADodgeBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

