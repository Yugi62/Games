#include "GameProject.h"
#include "GoldenHelmet.h"


AGoldenHelmet::AGoldenHelmet()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = EmptyComponent;
	CapsuleComponent->SetupAttachment(EmptyComponent);

	CapsuleComponent->SetCapsuleRadius(17.0f);
	CapsuleComponent->SetCapsuleHalfHeight(17.0f);
	CapsuleComponent->SetCollisionProfileName("Armor");
	CapsuleComponent->SetRelativeLocation(FVector(0.0f,0.0f,4.0f));

	Sprite = ConstructorHelpers::FObjectFinderOptional<UPaperSprite>(TEXT("PaperSprite'/Game/Item/Sprite/GoldenHelmet_H_Sprite.GoldenHelmet_H_Sprite'")).Get();
	HighlightedSprite = ConstructorHelpers::FObjectFinderOptional<UPaperSprite>(TEXT("PaperSprite'/Game/Item/Sprite/GoldenHelmet_S_Sprite.GoldenHelmet_S_Sprite'")).Get();

	SpriteComponent->SetSprite(Sprite);

	SpriteComponent->SetupAttachment(CapsuleComponent);


	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureTemp(TEXT("Texture2D'/Game/Item/GoldenHelmet_H.GoldenHelmet_H'"));
	Texture = TextureTemp.Object;


	Durability = 3;
}

