// Fill out your copyright notice in the Description page of Project Settings.


#include "EndScreenHUD.h"
#include "RunnerGameModeBase.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UEndScreenHUD::NativeConstruct()
{
	Super::NativeConstruct();

	RestartBtn->OnClicked.AddDynamic(this, &UEndScreenHUD::RestartGame);
}

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

void UEndScreenHUD::RestartGame()
{
	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), TEXT("RestartLevel"));
}

void SetScoreRow(UTextBlock* NameTextBlock, int index, TArray<FLeaderboardItem> Scores, UTextBlock* ScoreTextBlock)
{
	if(index>=0)
	{
		FString newString = Scores[index].Name1;
		newString.Append(" & ");
		newString.Append(Scores[index].Name2);
		NameTextBlock->SetText(FText::FromString(newString));

		ScoreTextBlock->SetText(FText::FromString(FString::FromInt(Scores[index].Score)));
	}
}

void UEndScreenHUD::DisplayHighscores(TArray<FLeaderboardItem> Scores)
{
	if(Scores.IsEmpty())
	{
		return;
	}

	SetScoreRow(ScoreName1, Scores.Num()-1, Scores, ScoreScore1);
	SetScoreRow(ScoreName2, Scores.Num()-2, Scores, ScoreScore2);
	SetScoreRow(ScoreName3, Scores.Num()-3, Scores, ScoreScore3);
	SetScoreRow(ScoreName4, Scores.Num()-4, Scores, ScoreScore4);
	SetScoreRow(ScoreName5, Scores.Num()-5, Scores, ScoreScore5);
}
