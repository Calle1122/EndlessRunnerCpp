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
	class UTexture2D* FullHeartTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Textures")
	class UTexture2D* EmptyHeartTexture; 
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ScoreTxt;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Health1Img;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Health2Img;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Health3Img;

	void ToggleHealth(UImage* targetImage);
};
