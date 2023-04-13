// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "RunTile.generated.h"

UCLASS()
class ENDLESSRUNNER_API ARunTile : public AActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Component")
	USceneComponent* RootSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Component")
	UStaticMeshComponent* TileMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Component")
	UArrowComponent* TileSeamPoint;
	
	// Sets default values for this actor's properties
	ARunTile();

	FORCEINLINE const FTransform& GetAttachTransform() const
	{
		return TileSeamPoint->GetComponentTransform();
	}

protected:
	UPROPERTY(VisibleInstanceOnly)
	class ARunnerGameModeBase* RunGameMode;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
