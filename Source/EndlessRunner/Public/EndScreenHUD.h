// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LeaderboardItem.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
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
	class UButton* MainMenuBtn;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ScoreName1;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ScoreScore1;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ScoreName2;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ScoreScore2;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ScoreName3;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ScoreScore3;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ScoreName4;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ScoreScore4;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ScoreName5;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ScoreScore5;

	void NativeConstruct() override;
	
	void InitializeEndScreen();
	void RegisterScore();

	UFUNCTION()
	void RestartGame();
	
	void DisplayHighscores(TArray<FLeaderboardItem> Scores);

protected:
	UPROPERTY(VisibleInstanceOnly)
	class ARunnerGameModeBase* RunGameMode;

	int FinalScore;
};
