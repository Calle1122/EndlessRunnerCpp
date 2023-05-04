// Fill out your copyright notice in the Description page of Project Settings.


#include "RunGameHUD.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void URunGameHUD::RemoveHealth(UImage* targetImage)
{
	targetImage->SetBrushFromTexture(EmptyHeartTexture);
}

void URunGameHUD::RespawnHealthImages(int PlayerIndex)
{
	if(PlayerIndex == 0)
	{
		P1Health1Img->SetBrushFromTexture(P1FullHeartTexture);
		P1Health2Img->SetBrushFromTexture(P1FullHeartTexture);
		P1Health3Img->SetBrushFromTexture(P1FullHeartTexture);
	}
	else
	{
		P2Health1Img->SetBrushFromTexture(P2FullHeartTexture);
		P2Health2Img->SetBrushFromTexture(P2FullHeartTexture);
		P2Health3Img->SetBrushFromTexture(P2FullHeartTexture);
	}
}

void URunGameHUD::UpdateMultiplierTxt(float NewMultiplier)
{
	FString NewMultiplierString = FString::SanitizeFloat(NewMultiplier);
	NewMultiplierString.Append(TEXT("x"));
	ScoreMultiplierTxt->SetText(FText::FromString(NewMultiplierString));
}

void URunGameHUD::PlayLuckyAnimation(int PlayerIndex)
{
	if(PlayerIndex==0)
	{
		PlayAnimation(P1LuckyDodgeAnim);
	}
	else
	{
		PlayAnimation(P2LuckyDodgeAnim);
	}
}
