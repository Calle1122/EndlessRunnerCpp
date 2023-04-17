// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RunGameHUD.h"
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
	TSubclassOf<ARunTile> BaseTileClass;

	UPROPERTY(EditAnywhere, Category="GameConfig")
	TArray<TSubclassOf<ARunTile>> EasyTiles;
	
	UPROPERTY(EditAnywhere, Category="GameConfig")
	int NumberOfInitialTiles = 10;

	UPROPERTY(EditAnywhere, Category="GameConfig")
	int PlayerHealth = 3;

	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<URunGameHUD> UserInterface;

	UPROPERTY(VisibleInstanceOnly, Category="Runtime")
	UArrowComponent* NextTileArrow;
	
	UFUNCTION(BlueprintCallable)
	void CreateInitialTiles();

	UFUNCTION(BlueprintCallable)
	void AddTile();

	UFUNCTION(BlueprintCallable)
	void ReduceHealth();

	UFUNCTION(BlueprintCallable)
	void EndRun();
	
protected:
	virtual void BeginPlay() override;
};
