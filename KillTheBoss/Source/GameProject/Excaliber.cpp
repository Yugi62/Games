#include "GameProject.h"
#include "Excaliber.h"

AExcaliber::AExcaliber()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = EmptyComponent;
	CapsuleComponent->SetupAttachment(EmptyComponent);

	CapsuleComponent->SetCapsuleRadius(8.0f);
	CapsuleComponent->SetCapsuleHalfHeight(50.0f);
	CapsuleComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 42.5f));

	Sprite = ConstructorHelpers::FObjectFinderOptional<UPaperSprite>(TEXT("PaperSprite'/Game/Item/Sprite/Excaliber_H_Sprite.Excaliber_H_Sprite'")).Get();
	HighlightedSprite = ConstructorHelpers::FObjectFinderOptional<UPaperSprite>(TEXT("PaperSprite'/Game/Item/Sprite/Excaliber_S_Sprite.Excaliber_S_Sprite'")).Get();

	SpriteComponent->SetSprite(Sprite);

	SpriteComponent->SetupAttachment(CapsuleComponent);


	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureTemp(TEXT("Texture2D'/Game/Item/Excaliber_H.Excaliber_H'"));
	Texture = TextureTemp.Object;


	Durability = 1;
	Damage = 33.0f;

}
