#pragma once

#include "GameFramework/Character.h"
#include "Enemy.h"
#include "GunMan.generated.h"

UCLASS()
class GAMEPROJECT_API AGunMan : public AEnemy
{
	GENERATED_BODY()

public:

	AGunMan();
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite")
	class UPaperFlipbook* IdleAnimation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite")
	class UPaperFlipbook* RunAnimation;

	UFUNCTION()
	virtual void OverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OverlapStop(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void SetFire();
	UFUNCTION()
	void Fire();

	FTimerHandle FireHandle;

	// ¿À¹ö·¦ È®ÀÎ
	AActor* OverlapCharacter;
};
