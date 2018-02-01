// Fill out your copyright notice in the Description page of Project Settings.

#include "GameProject.h"
#include "MouthTeaser.h"

#include "PlayerCharacter.h"
#include "SmallBullet.h"

AMouthTeaser::AMouthTeaser()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = GetCapsuleComponent();

	GetCapsuleComponent()->SetCapsuleRadius(20.0f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(20.0f);

	Sight->SetBoxExtent(FVector(850.0f, 1.0f, 500.0f));

	IdleAnimation = ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook>(TEXT("PaperFlipbook'/Game/Sprite/Enemy/Flipbook/MouthTeaser_IDE.MouthTeaser_IDE'")).Get();
	FlipbookComponent->SetFlipbook(IdleAnimation);
	FlipbookComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -5.0f));

	OverlapCharacter = nullptr;
	temp = 0.0f;
}

void AMouthTeaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OverlapCharacter)
	{
		temp += DeltaTime;

		if (temp >= 1.0f)
		{
			TurnCharacter();
			Fire();
			temp = 0.0f;
		}
	}
}

void AMouthTeaser::OverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (OtherActor->GetActorClass()->IsChildOf(APlayerCharacter::StaticClass()))
			OverlapCharacter = OtherActor;
	}
}

void AMouthTeaser::OverlapStop(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		OverlapCharacter = nullptr;
		temp = 0.0f;
	}
}

void AMouthTeaser::Fire()
{
	if (GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		FVector SpawnLocation = FlipbookComponent->GetSocketLocation("Bullet");
		FRotator SpawnRotation = FlipbookComponent->GetSocketRotation("Bullet");

		ASmallBullet* Projectile = GetWorld()->SpawnActor<ASmallBullet>(SpawnLocation, SpawnRotation, SpawnParams);

		if (Projectile)
		{
			FVector	PlayerLocation = OverlapCharacter->GetActorLocation();

			FVector Result = PlayerLocation - SpawnLocation;
			Result.Normalize();

			Projectile->FireDirection(Result);
		}
	}
}

void AMouthTeaser::TurnCharacter()
{
	if (OverlapCharacter->GetActorLocation().X - RootComponent->GetComponentLocation().X >= 0)
	{
		RootComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	}
	else
		RootComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
}