// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LeaderboardItem.h"
#include "SaveFileHandler.generated.h"


/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API USaveFileHandler : public USaveGame
{
	GENERATED_BODY()
public:
	USaveFileHandler();
	
	UPROPERTY(EditAnywhere)
	TArray<FLeaderboardItem> LeaderboardItems;
};

