// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RunTile.h"
#include "GameFramework/GameModeBase.h"
#include "RunnerGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API ARunnerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="GameConfig")
	TSubclassOf<ARunTile> TileClass;
	
	UPROPERTY(EditAnywhere, Category="GameConfig")
	int NumberOfInitialTiles = 10;

	UPROPERTY(VisibleInstanceOnly, Category="Runtime")
	FTransform NextTilePoint;
	
	UFUNCTION(BlueprintCallable)
	void CreateInitialTiles();

	UFUNCTION(BlueprintCallable)
	void AddTile();

protected:
	virtual void BeginPlay() override;
};
