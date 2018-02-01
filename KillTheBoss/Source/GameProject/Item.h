// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "Item.generated.h"

UCLASS()
class GAMEPROJECT_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	

	AItem();
	virtual void BeginPlay() override;

	UFUNCTION()
	UTexture2D* GetTexture() { return Texture; }

	UFUNCTION()
	void OverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OverlapStop(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FORCEINLINE UCapsuleComponent* GetCapsuleComponent() { return CapsuleComponent; }

	UPROPERTY(VisibleAnywhere, Category = "Value")
	int32 Durability;

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Collision")
	UStaticMeshComponent* EmptyComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Collision")
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Sprite")
	class UPaperSpriteComponent* SpriteComponent;

	UPROPERTY(VisibleAnywhere, Category = "Sprite")
	class UPaperSprite* Sprite;

	UPROPERTY(VisibleAnywhere, Category = "Sprite")
	class UPaperSprite* HighlightedSprite;

	UPROPERTY(VisibleAnywhere, Category = "Texture")
	UTexture2D* Texture;

};

UCLASS()
class GAMEPROJECT_API AArmor : public AItem
{
	GENERATED_BODY()

public:

	int32 DestroyCount = 0;
	FTimerHandle DestroyTimerHandle;
	void DestroyArmor();
	void DestroyArmorTimer();
};

UCLASS()
class GAMEPROJECT_API AHelmet : public AArmor
{
	GENERATED_BODY()
};

UCLASS()
class GAMEPROJECT_API ASuit : public AArmor
{
	GENERATED_BODY()
};


UCLASS()
class GAMEPROJECT_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	int32 DestroyCount_w = 0;
	FTimerHandle DestroyTimerHandle_w;
	void DestroyWeapon();
	void DestroyWeaponTimer();

	UPROPERTY(VisibleAnywhere, Category = "Value")
	float Damage;
};
