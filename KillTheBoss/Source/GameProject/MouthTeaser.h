// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Enemy.h"
#include "MouthTeaser.generated.h"

UCLASS()
class GAMEPROJECT_API AMouthTeaser : public AEnemy
{
	GENERATED_BODY()

public:
	
	AMouthTeaser();
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleAnywhere)
	AActor* OverlapCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite")
	class UPaperFlipbook* IdleAnimation;

	UFUNCTION()
	virtual void OverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OverlapStop(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void Fire();
	UFUNCTION()
	void TurnCharacter();

	float temp;
};
