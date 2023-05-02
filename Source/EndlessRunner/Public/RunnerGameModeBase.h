// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EndScreenHUD.h"
#include "RunGameHUD.h"
#include "RunTile.h"
#include "EndlessRunner/EndlessRunnerCharacter.h"
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
	int Player1Health = 3;
	UPROPERTY(EditAnywhere, Category="GameConfig")
	int Player2Health = 3;

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
	void ReduceHealth(int PlayerIndex);

	UFUNCTION(BlueprintCallable)
	void EndRun();

	UFUNCTION(BlueprintCallable)
	void ChangeMultiplier(float NewMultiplier);
	float MultiplierAddTimer = 0.f;

	UFUNCTION(BlueprintCallable)
	void OnTileDestroy();

	UFUNCTION(BlueprintCallable)
	void P1Dodge();
	UFUNCTION(BlueprintCallable)
	void P2Dodge();

	URunGameHUD* RunWidget;
	UEndScreenHUD* EndScreenWidget;
	
	float Score;

	UPROPERTY(EditAnywhere, Category="Player2")
	AEndlessRunnerCharacter* Player2;
	
	//I Frame Handling
	USkeletalMeshComponent* Player1Mesh;
	USkeletalMeshComponent* Player2Mesh;
	
	bool P1IFrameMode;
	bool P2IFrameMode;

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
	UPROPERTY(EditAnywhere, Category="GameConfig")
	UMaterialInterface* PinkBaseMat1;
	UPROPERTY(EditAnywhere, Category="GameConfig")
	UMaterialInterface* PinkBaseMat2;
	UPROPERTY(EditAnywhere, Category="GameConfig")
	UMaterialInterface* PinkBaseMat3;
	
	UPROPERTY()
	FTimerHandle P1IFrameHandle;
	UPROPERTY()
	FTimerHandle P2IFrameHandle;

	UFUNCTION()
	void P1EnableDamageTaking();
	void P2EnableDamageTaking();
	
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	bool canGenerateTile = true;
};
