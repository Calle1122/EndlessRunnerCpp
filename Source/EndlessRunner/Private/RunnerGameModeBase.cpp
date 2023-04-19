// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessRunner/Public/RunnerGameModeBase.h"
#include "RunGameHUD.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void ARunnerGameModeBase::EnableDamageTaking()
{
	IFrameMode=false;

	CharacterMesh->SetMaterial(0, BaseMat1);
	CharacterMesh->SetMaterial(1, BaseMat2);
}

void ARunnerGameModeBase::BeginPlay()
{
	NewWidget = CreateWidget<URunGameHUD>(GetWorld()->GetFirstPlayerController(), UserInterface);
	NewWidget->AddToViewport(9999);

	CreateInitialTiles();

	ACharacter* MyCharacter = Cast<ACharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	CharacterMesh = MyCharacter->GetMesh();
}

void ARunnerGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	Score+=DeltaSeconds*ScoreMultiplier;
	NewWidget->ScoreTxt->SetText(FText::FromString(FString::FromInt((int)Score)));

	MultiplierAddTimer+=DeltaSeconds/5;
	if(MultiplierAddTimer>=1.f)
	{
		MultiplierAddTimer=0.f;
		ChangeMultiplier(ScoreMultiplier+.1f);
	}

	ProjectileChanceFactor += DeltaSeconds/5;
	if(ProjectileChanceFactor>100.f)
	{
		ProjectileChanceFactor = 100.f;
	}
	ProjectilePercentChance = (int)ProjectileChanceFactor;
}

ARunnerGameModeBase::ARunnerGameModeBase()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void ARunnerGameModeBase::CreateInitialTiles()
{
	for(int i=0; i<NumberOfInitialTiles; i++)
	{
		AddTile();
	}
}

void ARunnerGameModeBase::AddTile()
{
	UWorld* World = GetWorld();

	if(World)
	{
		ARunTile* Tile;
		
		if(!NextTileArrow)
		{
			Tile = GetWorld()->SpawnActor<ARunTile>(BaseTileClass, FVector(0,0,0), FRotator(0,0,0));
		}
		else
		{
			int32 index = FMath::RandRange(0, EasyTiles.Num() - 1);
			Tile = GetWorld()->SpawnActor<ARunTile>(EasyTiles[index], NextTileArrow->GetComponentTransform());
		}
		if(Tile)
		{
			NextTileArrow = Tile->GetAttachArrow();
		}
	}
}

void ARunnerGameModeBase::ReduceHealth()
{
	if(IFrameMode==false)
	{
		PlayerHealth--;
		IFrameMode=true;

		CharacterMesh->SetMaterial(0, IMaterial);
		CharacterMesh->SetMaterial(1, IMaterial);
		
		GetWorldTimerManager().SetTimer(IFrameHandle, this, &ARunnerGameModeBase::EnableDamageTaking, IFrameTime, false);
		
		switch (PlayerHealth)
		{
		case 2:
			NewWidget->RemoveHealth(NewWidget->Health3Img);
			break;
		case 1:
			NewWidget->RemoveHealth(NewWidget->Health2Img);
			break;
		case 0:
			NewWidget->RemoveHealth(NewWidget->Health1Img);
			break;
		}
	
		if(PlayerHealth<=0)
		{
			EndRun();
		}
	}
}

void ARunnerGameModeBase::EndRun()
{
	//TODO: Implement Game Over and Restart

	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Player is DEAD"));
	}
}

void ARunnerGameModeBase::ChangeMultiplier(float NewMultiplier)
{
	NewMultiplier*=10;
	NewMultiplier = FMath::CeilToInt(NewMultiplier);
	NewMultiplier/=10;
	
	ScoreMultiplier = NewMultiplier;
	NewWidget->UpdateMultiplierTxt(NewMultiplier);
}



