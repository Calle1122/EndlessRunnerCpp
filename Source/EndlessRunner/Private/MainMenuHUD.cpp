// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuHUD.h"

#include "RunnerGameModeBase.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuHUD::NativeConstruct()
{
	Super::NativeConstruct();

	StartBtn->OnClicked.AddDynamic(this, &UMainMenuHUD::StartGame);
	QuitBtn->OnClicked.AddDynamic(this, &UMainMenuHUD::QuitGame);
}

void UMainMenuHUD::InitializeMainMenu()
{
	RunGameMode = Cast<ARunnerGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	check(RunGameMode);
}

void UMainMenuHUD::StartGame()
{
	RunGameMode->Player1Name = FString(Name1Box->GetText().ToString());
	RunGameMode->Player2Name = FString(Name2Box->GetText().ToString());
	
	RunGameMode->OnGameStart();
}

void UMainMenuHUD::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}
