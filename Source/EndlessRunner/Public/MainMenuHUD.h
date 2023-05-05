// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuHUD.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ENDLESSRUNNER_API UMainMenuHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* MenuHeaderText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UEditableTextBox* Name1Box;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UEditableTextBox* Name2Box;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* StartBtn;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* QuitBtn;

	void NativeConstruct() override;

	void InitializeMainMenu();

	UFUNCTION()
	void StartGame();
	void QuitGame();

protected:
	UPROPERTY(VisibleInstanceOnly)
	class ARunnerGameModeBase* RunGameMode;
};
