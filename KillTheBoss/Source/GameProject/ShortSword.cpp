// Fill out your copyright notice in the Description page of Project Settings.

#include "GameProject.h"
#include "ShortSword.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"

AShortSword::AShortSword()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = EmptyComponent;
	CapsuleComponent->SetupAttachment(EmptyComponent);

	CapsuleComponent->SetCapsuleRadius(8.0f);
	CapsuleComponent->SetCapsuleHalfHeight(18.0f);
	CapsuleComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 13.0f));

	Sprite = ConstructorHelpers::FObjectFinderOptional<UPaperSprite>(TEXT("PaperSprite'/Game/Item/Sprite/ShortSword_H_Sprite.ShortSword_H_Sprite'")).Get();
	HighlightedSprite = ConstructorHelpers::FObjectFinderOptional<UPaperSprite>(TEXT("PaperSprite'/Game/Item/Sprite/ShortSword_S_Sprite.ShortSword_S_Sprite'")).Get();

	SpriteComponent->SetSprite(Sprite);

	SpriteComponent->SetupAttachment(CapsuleComponent);


	static ConstructorHelpers::FObjectFinder<UTexture2D> TextureTemp(TEXT("Texture2D'/Game/Item/ShortSword_H.ShortSword_H'"));
	Texture = TextureTemp.Object;

	// °«°« Ä®
	Durability = 9999;
	Damage = 2.5f;
}
