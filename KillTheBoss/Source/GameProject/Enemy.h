// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "PaperCharacter.h"
#include "Item.h"

#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbook.h"
#include "PaperSprite.h"

#include "Enemy.generated.h"

#define Rare_Drop 5
#define Armor_Drop 15
#define Weapon_Drop 20

UCLASS()
class GAMEPROJECT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

protected:

	AEnemy();
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite")
	class UPaperFlipbookComponent* FlipbookComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	UBoxComponent* Sight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Value")
	float HP;

	UFUNCTION()
	virtual void OverlapPawn(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OverlapStop(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	int32 DropPercentage;
	void DropItem();

	FTimerHandle TimerHandle;
	void SetTimer();
	int32 EndTimer = 0;

	FTimerHandle DeathHandle;
	void DeathTimer();
	int32 DeathCount = 0;

	UPROPERTY(EditAnywhere, Category = "Music")
	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, Category = "Music")
	USoundCue* Damaged_Cue;

};
