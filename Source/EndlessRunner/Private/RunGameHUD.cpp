// Fill out your copyright notice in the Description page of Project Settings.


#include "RunGameHUD.h"

#include "Components/Image.h"

void URunGameHUD::RemoveHealth(UImage* targetImage)
{
	targetImage->SetBrushFromTexture(EmptyHeartTexture);
}
