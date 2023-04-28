// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EndScreenHUD.h"
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
	ARunnerGameModeBase();
	
	UPROPERTY(EditAnywhere, Category="GameConfig")
	TSubclassOf<ARunTile> BaseTileClass;

	UPROPERTY(EditAnywhere, Category="GameConfig")
	TArray<TSubclassOf<ARunTile>> EasyTiles;
	
	UPROPERTY(EditAnywhere, Category="GameConfig")
	int NumberOfInitialTiles = 10;

	UPROPERTY(EditAnywhere, Category="GameConfig")
	int PlayerHealth = 3;

	UPROPERTY(EditAnywhere, Category="GameConfig")
	int ProjectilePercentChance = 1;

	float ProjectileChanceFactor = 0;

	UPROPERTY(EditAnywhere, Category="GameConfig")
	float ScoreMultiplier = 1.f;

	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<URunGameHUD> UserInterface;

	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UEndScreenHUD> EndScreenInterface;

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

	UFUNCTION(BlueprintCallable)
	void ChangeMultiplier(float NewMultiplier);
	float MultiplierAddTimer = 0.f;

	UFUNCTION(BlueprintCallable)
	void OnTileDestroy();

	URunGameHUD* RunWidget;
	UEndScreenHUD* EndScreenWidget;
	
	float Score;

	//I Frame Handling
	USkeletalMeshComponent* CharacterMesh;
	
	bool IFrameMode;

	UPROPERTY(EditAnywhere, Category="GameConfig")
	float IFrameTime = 1.f;
	
	UPROPERTY(EditAnywhere, Category="GameConfig")
	UMaterialInterface* IMaterial;
	UPROPERTY(EditAnywhere, Category="GameConfig")
	UMaterialInterface* BaseMat1;
	UPROPERTY(EditAnywhere, Category="GameConfig")
	UMaterialInterface* BaseMat2;
	UPROPERTY(EditAnywhere, Category="GameConfig")
	UMaterialInterface* BaseMat3;
	
	UPROPERTY()
	FTimerHandle IFrameHandle;

	UFUNCTION()
	void EnableDamageTaking();
	
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	bool canGenerateTile = true;
};
