// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Item.h"
#include "ItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void Show();

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void Update();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AItem*> ItemArray;
};
