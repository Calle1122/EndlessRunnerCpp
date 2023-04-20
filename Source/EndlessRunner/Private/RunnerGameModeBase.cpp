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
	CharacterMesh->SetMaterial(2, BaseMat3);
}

void ARunnerGameModeBase::BeginPlay()
{
	RunWidget = CreateWidget<URunGameHUD>(GetWorld()->GetFirstPlayerController(), UserInterface);
	RunWidget->AddToViewport(9999);

	CreateInitialTiles();

	ACharacter* MyCharacter = Cast<ACharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	CharacterMesh = MyCharacter->GetMesh();
}

void ARunnerGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	Score+=DeltaSeconds*ScoreMultiplier;
	RunWidget->ScoreTxt->SetText(FText::FromString(FString::FromInt((int)Score)));

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
		CharacterMesh->SetMaterial(2, IMaterial);
		
		GetWorldTimerManager().SetTimer(IFrameHandle, this, &ARunnerGameModeBase::EnableDamageTaking, IFrameTime, false);
		
		switch (PlayerHealth)
		{
		case 2:
			RunWidget->RemoveHealth(RunWidget->Health3Img);
			break;
		case 1:
			RunWidget->RemoveHealth(RunWidget->Health2Img);
			break;
		case 0:
			RunWidget->RemoveHealth(RunWidget->Health1Img);
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
	RunWidget->RemoveFromParent();
	
	EndScreenWidget = CreateWidget<UEndScreenHUD>(GetWorld()->GetFirstPlayerController(), EndScreenInterface);
	EndScreenWidget->AddToViewport(9999);

	EndScreenWidget->InitializeEndScreen();
	EndScreenWidget->RegisterScore();
}

void ARunnerGameModeBase::ChangeMultiplier(float NewMultiplier)
{
	NewMultiplier*=10;
	NewMultiplier = FMath::CeilToInt(NewMultiplier);
	NewMultiplier/=10;
	
	ScoreMultiplier = NewMultiplier;
	RunWidget->UpdateMultiplierTxt(NewMultiplier);
}



