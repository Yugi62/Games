#pragma once

#include "GameFramework/Character.h"
#include "Enemy.h"
#include "KingTeaser.generated.h"

UCLASS()
class GAMEPROJECT_API AKingTeaser : public AEnemy
{
	GENERATED_BODY()

public:

	AKingTeaser();
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite")
	class UPaperFlipbook* IdleAnimation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite")
	class UPaperFlipbook* RunAnimation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite")
	class UPaperFlipbook* FreezeAnimation;

	UFUNCTION()
	virtual void OverlapPawn(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OverlapStop(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	int32 Pattern;

	// Pattern 1
	void Chase();
	void Fire();

	// Pattern 2
	void FourLineShoot();
	FVector PlayerLocation;

	// Pattern 3
	void WaveShoot();
	int32 WaveCount = 0;
	
	// Pattern 4
	void RoundShoot();
	int32 AngleCount = 0;


	// Pattern 5
	void Freeze();
	int32 PatternToFreeze = 0;

	FTimerHandle PatternHandle;
	float PatternCount;

	//////////////////////////

	// Overlap
	AActor* PlayerCharacter;

	FTimerHandle DelayTimerHandle;
	void DelayTimer();
	int32 DelayCount;

	FTimerHandle HitTimerHandle;
	void HitTimer();
	int32 HitCount;

	FTimerHandle DeathTimerHandle;
	void DeathTimer();
	int32 DeathCount;

	bool ActivateBoss;

	UPROPERTY(EditAnywhere, Category = "Music")
	USoundCue* Die_Cue;
};
