// Fill out your copyright notice in the Description page of Project Settings.


#include "EndScreenHUD.h"
#include "RunnerGameModeBase.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UEndScreenHUD::InitializeEndScreen()
{
	RunGameMode = Cast<ARunnerGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	check(RunGameMode);

	GetWorld()->GetFirstPlayerController()->SetPause(true);
}

void UEndScreenHUD::RegisterScore()
{
	FinalScore = (int)RunGameMode->Score;
	FString FinalScoreString = ("Score: ");
	FinalScoreString.Append(FString::FromInt(FinalScore));
	FinalScoreTxt->SetText(FText::FromString(FinalScoreString));
}
