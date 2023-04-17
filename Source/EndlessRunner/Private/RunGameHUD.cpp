// Fill out your copyright notice in the Description page of Project Settings.


#include "RunGameHUD.h"

#include "Components/Image.h"

void URunGameHUD::ToggleHealth(UImage* targetImage)
{
	if(targetImage->IsVisible())
	{
		targetImage->SetBrushFromTexture(EmptyHeartTexture);
	}
	else
	{
		targetImage->SetBrushFromTexture(FullHeartTexture);
	}
}
