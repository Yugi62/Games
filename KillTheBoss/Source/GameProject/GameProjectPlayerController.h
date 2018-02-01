// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Option.h"
#include "Item.h"
#include "ItemWidget.h"
#include "ItemSlotWidget.h"
#include "ItemSelectWidget.h"
#include "GameProjectPlayerController.generated.h"

UCLASS()
class GAMEPROJECT_API AGameProjectPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void Possess(APawn* InPawn) override;

	// UFUNCTION

	UFUNCTION(BlueprintCallable, Category = "UMG")
	void ControlOptionWidget();

	UFUNCTION(BlueprintCallable, Category = "UMG")
	void ControlItemWidget(int32 InventorySelect);

	UFUNCTION(BlueprintCallable, Category = "UMG")
	void ControlItemSelectWidget(int32 FindInventorySelect);

	UFUNCTION(BlueprintCallable, Category = "UMG")
	void Drop(int32 InventoryLocation);

	UFUNCTION(BlueprintCallable, Category = "UMG")
	void Equip(int32 InventoryLocation);


	// UPROPERTY

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UOption> OptionWidgetBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UItemWidget> ItemWidgetBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UItemSelectWidget> ItemSelectWidgetBP;

	
	UPROPERTY(BlueprintReadWrite, Category = "UMG")
	UOption* OptionWidget;

	UPROPERTY(BlueprintReadWrite, Category = "UMG")
	UItemWidget* ItemWidget;

	UPROPERTY(BlueprintReadWrite, Category = "UMG")
	UItemSelectWidget* ItemSelect;

	// 0 = Weapon
	// 1 = Armor
	int32 InventorySelect;

	bool IsCharaterEquipWeapon;
	bool IsCharacterEquipHelmet;
	bool IsCharacterEquipSuit;

	UPROPERTY(EditAnywhere, Category = "CameraShake")
	TSubclassOf<UCameraShake> CameraShake;
	void ShakeCamera();


	/////////////////////////////////
};
