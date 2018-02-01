// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UserWidget.h"
#include "Option.generated.h"


UCLASS()
class GAMEPROJECT_API UOption : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
	void Show();
};
