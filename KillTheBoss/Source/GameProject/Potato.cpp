// Fill out your copyright notice in the Description page of Project Settings.

#include "GameProject.h"
#include "Potato.h"

#include "PlayerCharacter.h"

APotato::APotato()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleRadius(25.0f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(40.0f);

	Sight->SetBoxExtent(FVector(900.0f, 1.0f, 200.0f));

	IdleAnimation = ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook>(TEXT("PaperFlipbook'/Game/Sprite/Enemy/Flipbook/PotatoIdle.PotatoIdle'")).Get();
	RunAnimation = ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook>(TEXT("PaperFlipbook'/Game/Sprite/Enemy/Flipbook/PotatoRun.PotatoRun'")).Get();

	FlipbookComponent->SetFlipbook(IdleAnimation);
	FlipbookComponent->SetRelativeLocation(FVector(-5.0f, 0.0f, 7.0f));

	// 점프 높이
	GetCharacterMovement()->JumpZVelocity = 500.0f;
}

void APotato::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (OverlapCharacter)
	{
		FlipbookComponent->SetFlipbook(RunAnimation);

		if (OverlapCharacter->GetActorLocation().Z - RootComponent->GetComponentLocation().Z >= 100)
			Jump();

		if (OverlapCharacter->GetActorLocation().X - RootComponent->GetComponentLocation().X >= 0)
		{
			RootComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
			AddMovementInput(FVector(1.0f, 0.0f, 0.0f), 0.7f);
		}
		else
		{
			RootComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
			AddMovementInput(FVector(-1.0f, 0.0f, 0.0f), 0.7f);
		}
	}

}

void APotato::OverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (OtherActor->GetActorClass()->IsChildOf(APlayerCharacter::StaticClass()))
			OverlapCharacter = OtherActor;
	}
}

void APotato::OverlapStop(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		OverlapCharacter = nullptr;
		FlipbookComponent->SetFlipbook(IdleAnimation);
	}
}
