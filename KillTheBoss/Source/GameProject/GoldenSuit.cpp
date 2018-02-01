#include "GameProject.h"
#include "GoldenSuit.h"

AGoldenSuit::AGoldenSuit()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = EmptyComponent;
	CapsuleComponent->SetupAttachment(EmptyComponent);

	CapsuleComponent->SetCapsuleRadius(17.0f);
	CapsuleComponent->SetCapsuleHalfHeight(17.0f);
	CapsuleComponent->SetCollisionProfileName("Armor");

	Sprite = ConstructorHelpers::FObjectFinderOptional<UPaperSprite>(TEXT("PaperSprite'/Game/Item/Sprite/GoldenSuit_H_Sprite.GoldenSuit_H_Sprite'")).Get();
	HighlightedSprite = ConstructorHelpers::FObjectFinderOptional<UPaperSprite>(TEXT("PaperSprite'/Game/Item/Sprite/GoldenSuit_S_Sprite.GoldenSuit_S_Sprite'")).Get();

	SpriteComponent->SetSprite(Sprite);
	SpriteComponent->SetupAttachment(CapsuleComponent);


	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureTemp(TEXT("Texture2D'/Game/Item/GoldenSuit_H.GoldenSuit_H'"));
	Texture = TextureTemp.Object;


	Durability = 3;
}


