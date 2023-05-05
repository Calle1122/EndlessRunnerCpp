// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RunGameHUD.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ENDLESSRUNNER_API URunGameHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Textures")
	class UTexture2D* P1FullHeartTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Textures")
	class UTexture2D* P2FullHeartTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Textures")
	class UTexture2D* EmptyHeartTexture; 
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ScoreTxt;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ScoreMultiplierTxt;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* P1Health1Img;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* P1Health2Img;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* P1Health3Img;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* P2Health1Img;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* P2Health2Img;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* P2Health3Img;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* P1LuckyDodgeAnim;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* P2LuckyDodgeAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* LSMoveTutorial;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* RSMoveTutorial;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* LSJumpTutorial;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* RSJumpTutorial;

	UFUNCTION()
	void PlayTutorialAnimations();
	
	void RemoveHealth(UImage* targetImage);

	void RespawnHealthImages(int PlayerIndex);

	void UpdateMultiplierTxt(float NewMultiplier);

	void PlayLuckyAnimation(int PlayerIndex);
};
