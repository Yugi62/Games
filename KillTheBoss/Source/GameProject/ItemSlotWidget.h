// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Item.h"
#include "ItemSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API UItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UMG")
	UTexture2D* Texture;

	UFUNCTION(BlueprintCallable, Category = "UMG")
	void SetItemTexture(AItem* Item);
};
