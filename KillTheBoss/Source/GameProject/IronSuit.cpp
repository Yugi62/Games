#include "GameProject.h"
#include "IronSuit.h"

AIronSuit::AIronSuit()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = EmptyComponent;
	CapsuleComponent->SetupAttachment(EmptyComponent);

	CapsuleComponent->SetCapsuleRadius(17.0f);
	CapsuleComponent->SetCapsuleHalfHeight(17.0f);
	CapsuleComponent->SetCollisionProfileName("Armor");

	Sprite = ConstructorHelpers::FObjectFinderOptional<UPaperSprite>(TEXT("PaperSprite'/Game/Item/Sprite/Ironsuit_H_Sprite.Ironsuit_H_Sprite'")).Get();
	HighlightedSprite = ConstructorHelpers::FObjectFinderOptional<UPaperSprite>(TEXT("PaperSprite'/Game/Item/Sprite/IronSuit_S_Sprite.IronSuit_S_Sprite'")).Get();

	SpriteComponent->SetSprite(Sprite);
	SpriteComponent->SetupAttachment(CapsuleComponent);


	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureTemp(TEXT("Texture2D'/Game/Item/Ironsuit_H.Ironsuit_H'"));
	Texture = TextureTemp.Object;


	Durability = 2;
}

void AIronSuit::BeginPlay()
{
	Super::BeginPlay();
	
}

void AIronSuit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}