// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Enemy.h"
#include "Potato.generated.h"

UCLASS()
class GAMEPROJECT_API APotato : public AEnemy
{
	GENERATED_BODY()

public:

	APotato();
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite")
	class UPaperFlipbook* IdleAnimation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite")
	class UPaperFlipbook* RunAnimation;

	UFUNCTION()
	virtual void OverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OverlapStop(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// ¿À¹ö·¦ È®ÀÎ
	AActor* OverlapCharacter;
};


