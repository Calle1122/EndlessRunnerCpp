// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessRunner/Public/RunnerGameModeBase.h"

void ARunnerGameModeBase::BeginPlay()
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Doing Begin Play"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Engine not Initialized"));
	}
	
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
		ARunTile* Tile = GetWorld()->SpawnActor<ARunTile>(TileClass, NextTilePoint);
		if(Tile)
		{
			NextTilePoint = Tile->GetAttachTransform();
		}
	}
}



