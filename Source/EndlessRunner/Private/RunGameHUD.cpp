// Fill out your copyright notice in the Description page of Project Settings.


#include "RunGameHUD.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void URunGameHUD::RemoveHealth(UImage* targetImage)
{
	targetImage->SetBrushFromTexture(EmptyHeartTexture);
}

void URunGameHUD::UpdateMultiplierTxt(float NewMultiplier)
{
	FString NewMultiplierString = FString::SanitizeFloat(NewMultiplier);
	NewMultiplierString.Append(TEXT("x"));
	ScoreMultiplierTxt->SetText(FText::FromString(NewMultiplierString));
}
