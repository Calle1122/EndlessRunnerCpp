// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "RunHitBox.generated.h"

UCLASS()
class ENDLESSRUNNER_API ARunHitBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	UBoxComponent* DamageBox;
	
	ARunHitBox();

protected:
	// Called when the game starts or when spawned
	UPROPERTY(VisibleInstanceOnly)
	class ARunnerGameModeBase* RunGameMode;
	
	UFUNCTION()
	void OnDamageBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
