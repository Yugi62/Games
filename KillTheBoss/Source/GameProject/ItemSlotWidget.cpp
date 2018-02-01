// Fill out your copyright notice in the Description page of Project Settings.

#include "GameProject.h"
#include "ItemSlotWidget.h"

void UItemSlotWidget::SetItemTexture(AItem* Item)
{
	
	if (Item != nullptr && Item->GetActorClass()->IsChildOf(AItem::StaticClass()))
		Texture = Item->GetTexture();

	else
		Texture = nullptr;
		
		
}


