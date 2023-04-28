// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "RunTile.generated.h"

UCLASS()
class ENDLESSRUNNER_API ARunTile : public AActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category="GameConfig")
	TArray<UArrowComponent*> ProjectilePositions;

	UPROPERTY(EditAnywhere, Category="GameConfig")
	TSubclassOf<AActor> Projectile;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	USceneComponent* RootSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	UStaticMeshComponent* TileMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	UArrowComponent* TileSeamPoint;
	
	// Sets default values for this actor's properties
	ARunTile();

	UArrowComponent* GetAttachArrow()
	{
		return TileSeamPoint;
	}

protected:
	UPROPERTY(VisibleInstanceOnly)
	class ARunnerGameModeBase* RunGameMode;

	UFUNCTION()
	void DestroyTile();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
