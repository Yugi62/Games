#include "GameProject.h"
#include "JumpPanel.h"

#include "PlayerCharacter.h"

AJumpPanel::AJumpPanel()
{
	PrimaryActorTick.bCanEverTick = true;

	OverlapLayer = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapLayer"));
	OverlapLayer->SetBoxExtent(FVector(30.0f, 1.0f, 10.0f));
	RootComponent = OverlapLayer;

	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	Sprite = ConstructorHelpers::FObjectFinderOptional<UPaperSprite>(TEXT("PaperSprite'/Game/Sprite/Object/JumpPanel_Sprite.JumpPanel_Sprite'")).Get();
	SpriteComponent->SetSprite(Sprite);

	SpriteComponent->SetupAttachment(OverlapLayer);
	SpriteComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -14.0f));

	OverlapLayer->OnComponentBeginOverlap.AddDynamic(this, &AJumpPanel::OverlapStart);

	JumpValue = 0.0f;
}

void AJumpPanel::BeginPlay()
{
	Super::BeginPlay();
	
}

void AJumpPanel::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AJumpPanel::OverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if(OtherActor->GetActorClass()->IsChildOf(APlayerCharacter::StaticClass()))
		{
			APlayerCharacter* Character = Cast<APlayerCharacter>(OtherActor);
			Character->LaunchCharacter(FVector(0.0f, 0.0f, 10.0f) * JumpValue, false, false);
		}
	}
}
