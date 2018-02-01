#include "GameProject.h"
#include "Scarecrow.h"

#include "PlayerCharacter.h"

AScarecrow::AScarecrow()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = GetCapsuleComponent();

	GetCapsuleComponent()->SetCapsuleRadius(25.0f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(35.0f);

	Sight->SetBoxExtent(FVector(800.0f, 1.0f, 400.0f));

	JumpingAnimation = ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook>(TEXT("PaperFlipbook'/Game/Sprite/Enemy/Flipbook/Scarecrow_Flipbook.Scarecrow_Flipbook'")).Get();
	FlipbookComponent->SetFlipbook(JumpingAnimation);

	FlipbookComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -8.0f));

	JumpTime = 1.0f;
}

void AScarecrow::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AScarecrow::SetJump, JumpTime, true);
	
}

void AScarecrow::SetJump()
{
	Jump();
}

void AScarecrow::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (OverlapCharacter)
	{
		if (OverlapCharacter->GetActorLocation().X - RootComponent->GetComponentLocation().X >= 0)
		{
			RootComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
		}
		else
		{
			RootComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}

}

void AScarecrow::OverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (OtherActor->GetActorClass()->IsChildOf(APlayerCharacter::StaticClass()))
			OverlapCharacter = OtherActor;
	}
}

void AScarecrow::OverlapStop(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		OverlapCharacter = nullptr;
	}
}
