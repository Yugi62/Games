#pragma once

#include "GameFramework/Actor.h"
#include "PlayerCharacter.h"
#include "PaperCharacter.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "Projectile.generated.h"

UCLASS()
class GAMEPROJECT_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	AProjectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	USphereComponent* ProjectileCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite")
	class UPaperSpriteComponent* SpriteComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite")
	class UPaperSprite* Sprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovementComponent;

	void FireDirection(const FVector& ShootDirection);

	UFUNCTION()
	void OverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
