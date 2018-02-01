// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Enemy.h"
#include "Slime.generated.h"

UCLASS()
class GAMEPROJECT_API ASlime : public AEnemy
{
	GENERATED_BODY()

public:

	ASlime();
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, Category = "Value")
	float Speed;

	UPROPERTY(EditAnywhere, Category = "Value")
	float TurnTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite")
	class UPaperFlipbook* MovingAnimation;

	float DELTATIME;
	float Direction;
	
	bool temp;
};
