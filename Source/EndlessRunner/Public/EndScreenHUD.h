// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndScreenHUD.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ENDLESSRUNNER_API UEndScreenHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* FinalScoreTxt;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* RestartBtn;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* MainMenuBtn;

	void InitializeEndScreen();
	void RegisterScore();

protected:
	UPROPERTY(VisibleInstanceOnly)
	class ARunnerGameModeBase* RunGameMode;

	int FinalScore;
};
