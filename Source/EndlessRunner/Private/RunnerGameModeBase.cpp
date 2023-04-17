// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessRunner/Public/RunnerGameModeBase.h"
#include "RunGameHUD.h"
#include "Blueprint/UserWidget.h"

void ARunnerGameModeBase::BeginPlay()
{
	URunGameHUD* NewWidget = CreateWidget<URunGameHUD>(GetWorld()->GetFirstPlayerController(), UserInterface);
	NewWidget->AddToViewport(9999);

	CreateInitialTiles();
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
	PlayerHealth--;

	if(PlayerHealth<=0)
	{
		EndRun();
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



