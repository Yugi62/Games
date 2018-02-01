// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Enemy.h"
#include "Scarecrow.generated.h"

UCLASS()
class GAMEPROJECT_API AScarecrow : public AEnemy
{
	GENERATED_BODY()

public:

	AScarecrow();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite")
	class UPaperFlipbook* JumpingAnimation;

	UPROPERTY(EditAnywhere, Category = "Value")
	float JumpTime;

	UFUNCTION()
	virtual void OverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OverlapStop(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	AActor* OverlapCharacter;

	FTimerHandle TimerHandle;
	void SetJump();

};
