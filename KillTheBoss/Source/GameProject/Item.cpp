// Fill out your copyright notice in the Description page of Project Settings.

#include "GameProject.h"
#include "Item.h"
#include "Enemy.h"
#include "PlayerCharacter.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AItem::OverlapStart);
	CapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &AItem::OverlapStop);
	CapsuleComponent->SetCollisionProfileName("Weapon");

	// Y축 이동 제한, X,Y축 회전 제한
	CapsuleComponent->GetBodyInstance()->bLockYTranslation = true;
	CapsuleComponent->GetBodyInstance()->bLockXRotation = true;
	CapsuleComponent->GetBodyInstance()->bLockYRotation = false;
	CapsuleComponent->GetBodyInstance()->bLockZRotation = true;

	EmptyComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EmptyComponent"));
	EmptyComponent->SetCollisionProfileName("Sprite");

	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SpriteComponent->SetCollisionProfileName("Sprite");

	UMaterialInterface* Material = ConstructorHelpers::FObjectFinderOptional<UMaterialInterface>(TEXT("MaterialInstanceConstant'/Paper2D/TranslucentUnlitSpriteMaterial.TranslucentUnlitSpriteMaterial'")).Get();
	SpriteComponent->SetMaterial(0, Material);

	Texture = CreateDefaultSubobject<UTexture2D>(TEXT("Texture"));
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
}

void AItem::OverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (OtherActor->GetActorClass()->IsChildOf(APlayerCharacter::StaticClass()))
		{
			SpriteComponent->SetSprite(HighlightedSprite);
		}

		if (OtherActor->GetActorClass()->IsChildOf(AEnemy::StaticClass()))
		{
			GetCapsuleComponent()->SetCollisionProfileName(TEXT("HoldWeapon"));
		}
	}
}

void AItem::OverlapStop(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		SpriteComponent->SetSprite(Sprite);
	}
}

void AArmor::DestroyArmor()
{
	if (!GetWorldTimerManager().IsTimerActive(DestroyTimerHandle))
	{
		this->GetCapsuleComponent()->SetSimulatePhysics(true);
		GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AArmor::DestroyArmorTimer, 0.2f, true);
	}
}

void AArmor::DestroyArmorTimer()
{
	if (DestroyCount % 2 == 0)
		SpriteComponent->SetSpriteColor(FLinearColor(1.0, 1.0, 1.0, 0.6));
	else
		SpriteComponent->SetSpriteColor(FLinearColor(1.0, 1.0, 1.0, 1.0));

	DestroyCount++;

	if (DestroyCount >= 6)
	{

		GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
		DestroyCount = 0;
		this->Destroy();
	}
}

void AWeapon::DestroyWeapon()
{
	if (!GetWorldTimerManager().IsTimerActive(DestroyTimerHandle_w))
	{

		this->GetCapsuleComponent()->SetSimulatePhysics(true);
		this->DetachRootComponentFromParent();
		GetWorldTimerManager().SetTimer(DestroyTimerHandle_w, this, &AWeapon::DestroyWeaponTimer, 0.2f, true);
	}
}

void AWeapon::DestroyWeaponTimer()
{
	if (DestroyCount_w % 2 == 0)
		SpriteComponent->SetSpriteColor(FLinearColor(1.0, 1.0, 1.0, 0.6));
	else
		SpriteComponent->SetSpriteColor(FLinearColor(1.0, 1.0, 1.0, 1.0));

	DestroyCount_w++;

	if (DestroyCount_w >= 6)
	{

		GetWorldTimerManager().ClearTimer(DestroyTimerHandle_w);
		DestroyCount_w = 0;
		this->Destroy();
	}
}