// Fill out your copyright notice in the Description page of Project Settings.

#include "GameProject.h"
#include "GameProjectPlayerController.h"
#include "PlayerCharacter.h"

#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbook.h"
#include "PaperSprite.h"

#include "Claymore.h"
#include "ShortSword.h"
#include "Excaliber.h"

#include "GoldenHelmet.h"
#include "GoldenSuit.h"
#include "IronHelmet.h"
#include "IronSuit.h"

#include "BossCameraShake.h"

void AGameProjectPlayerController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	if (OptionWidgetBP)
		OptionWidget = CreateWidget<UOption>(this, OptionWidgetBP);

	if (ItemWidgetBP)
		ItemWidget = CreateWidget<UItemWidget>(this, ItemWidgetBP);

	if (ItemSelectWidgetBP)
		ItemSelect = CreateWidget<UItemSelectWidget>(this, ItemSelectWidgetBP);

	IsCharaterEquipWeapon = false;
	IsCharacterEquipHelmet = false;
	IsCharacterEquipSuit = false;

	///////////////////////////////////////

}

void AGameProjectPlayerController::ControlOptionWidget()
{
	OptionWidget->Show();
}

void AGameProjectPlayerController::ControlItemWidget(int32 FindInventorySelect)
{
	APlayerCharacter* Character = Cast<APlayerCharacter>(GetCharacter());

	InventorySelect = FindInventorySelect;

	switch (InventorySelect)
	{
	case 0:
		ItemWidget->ItemArray = Character->WeaponInventory;
		break;

	case 1:
		ItemWidget->ItemArray = Character->ArmorInventory;
		break;

	default:
		break;
	}

	ItemWidget->Update();
	ItemWidget->Show();
}

void AGameProjectPlayerController::ControlItemSelectWidget(int32 InventoryLocation)
{
	ItemSelect->Show(InventoryLocation);
}

void AGameProjectPlayerController::Drop(int32 InventoryLocation)
{
	APlayerCharacter* Character = Cast<APlayerCharacter>(GetCharacter());

	if (Character->WeaponInventory[InventoryLocation] != nullptr && InventorySelect == 0)
	{
		FVector DropLocation = Character->GetActorLocation();

		FTransform Transform;
		Transform.SetLocation(DropLocation);

		FActorSpawnParameters SpawnParams;

		AWeapon* PickuptoSpawn = GetWorld()->SpawnActor<AWeapon>(Character->WeaponInventory[InventoryLocation]->GetClass(), Transform, SpawnParams);
		PickuptoSpawn->GetCapsuleComponent()->SetSimulatePhysics(true);

		Character->WeaponInventory[InventoryLocation] = nullptr;

		PickuptoSpawn->Durability = Character->WeaponDurability[InventoryLocation];
		Character->WeaponDurability[InventoryLocation] = NULL;
		
		ItemWidget->ItemArray = Character->WeaponInventory;
		ItemWidget->Update();
	}
	else if (Character->ArmorInventory[InventoryLocation] != nullptr && InventorySelect == 1)
	{
		FVector DropLocation = Character->GetActorLocation();

		FTransform Transform;
		Transform.SetLocation(DropLocation);

		FActorSpawnParameters SpawnParams;

		if (Character->ArmorInventory[InventoryLocation]->GetClass()->IsChildOf(AHelmet::StaticClass()))
		{
			AHelmet* PickuptoSpawn = GetWorld()->SpawnActor<AHelmet>(Character->ArmorInventory[InventoryLocation]->GetClass(), Transform, SpawnParams);
			PickuptoSpawn->GetCapsuleComponent()->SetSimulatePhysics(true);

			Character->ArmorInventory[InventoryLocation] = nullptr;

			PickuptoSpawn->Durability = Character->ArmorDurability[InventoryLocation];
			Character->ArmorDurability[InventoryLocation] = NULL;
		}

		else if (Character->ArmorInventory[InventoryLocation]->GetClass()->IsChildOf(ASuit::StaticClass()))
		{
			ASuit* PickuptoSpawn = GetWorld()->SpawnActor<ASuit>(Character->ArmorInventory[InventoryLocation]->GetClass(), Transform, SpawnParams);
			PickuptoSpawn->GetCapsuleComponent()->SetSimulatePhysics(true);

			Character->ArmorInventory[InventoryLocation] = nullptr;

			PickuptoSpawn->Durability = Character->ArmorDurability[InventoryLocation];
			Character->ArmorDurability[InventoryLocation] = NULL;
		}

		ItemWidget->ItemArray = Character->ArmorInventory;
		ItemWidget->Update();
	}
	
}

void AGameProjectPlayerController::Equip(int32 ItemInventoryLocation)
{
	APlayerCharacter* Character = Cast<APlayerCharacter>(GetCharacter());

	if (Character->WeaponInventory[ItemInventoryLocation] != nullptr && InventorySelect == 0)
	{
		FVector DropLocation = Character->CharacterComponent->GetSocketLocation("Weapon");
		FTransform Transform;

		Transform.SetLocation(DropLocation);

		FActorSpawnParameters SpawnParams;

		if (Character->WeaponInventory[ItemInventoryLocation]->GetClass()->IsChildOf(AWeapon::StaticClass()))
		{
			if (!Character->WeaponComponent->GetChildComponent(0))
				IsCharaterEquipWeapon = false;

			if (IsCharaterEquipWeapon == true)
			{
				AActor* ChildActor = Character->WeaponComponent->GetChildComponent(0)->GetOwner();
				AItem* ChildCasted = Cast<AItem>(ChildActor);

				AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(Character->WeaponInventory[ItemInventoryLocation]->GetClass(), Transform, SpawnParams);
				Weapon->DisableComponentsSimulatePhysics();

				Weapon->AttachRootComponentTo(Character->WeaponComponent, (TEXT("Weapon")), EAttachLocation::SnapToTarget);
				Weapon->GetCapsuleComponent()->SetCollisionProfileName("HoldWeapon");

				Weapon->Durability = Character->WeaponDurability[ItemInventoryLocation];

				if (ChildActor->GetActorClass()->IsChildOf(AClaymore::StaticClass()))
				{
					AClaymore* New = ConstructObject<AClaymore>(AClaymore::StaticClass());
					New->Durability = ChildCasted->Durability;
					Character->WeaponInventory[ItemInventoryLocation] = New;
					Character->WeaponDurability[ItemInventoryLocation] = New->Durability;

					New->Destroy();
				}
				else if (ChildActor->GetActorClass()->IsChildOf(AShortSword::StaticClass()))
				{
					AShortSword* New = ConstructObject<AShortSword>(AShortSword::StaticClass());
					New->Durability = ChildCasted->Durability;
					Character->WeaponInventory[ItemInventoryLocation] = New;
					Character->WeaponDurability[ItemInventoryLocation] = New->Durability;

					New->Destroy();
				}
				else if (ChildActor->GetActorClass()->IsChildOf(AExcaliber::StaticClass()))
				{
					AExcaliber* New = ConstructObject<AExcaliber>(AExcaliber::StaticClass());
					New->Durability = ChildCasted->Durability;
					Character->WeaponInventory[ItemInventoryLocation] = New;
					Character->WeaponDurability[ItemInventoryLocation] = New->Durability;

					New->Destroy();
				}

				ChildActor->Destroy();

				ItemWidget->ItemArray = Character->WeaponInventory;
				ItemWidget->Update();
			}

			else if (IsCharaterEquipWeapon == false)
			{
				if (Character->WeaponInventory[ItemInventoryLocation]->GetClass()->IsChildOf(AWeapon::StaticClass()))
				{
					IsCharaterEquipWeapon = true;
					Character->IsHoldingWeapon = true;

					AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(Character->WeaponInventory[ItemInventoryLocation]->GetClass(), Transform, SpawnParams);
					Weapon->DisableComponentsSimulatePhysics();

					Weapon->AttachRootComponentTo(Character->WeaponComponent, (TEXT("Weapon")), EAttachLocation::SnapToTarget);
					Weapon->GetCapsuleComponent()->SetCollisionProfileName("HoldWeapon");

					Character->WeaponInventory[ItemInventoryLocation] = nullptr;

					Weapon->Durability = Character->WeaponDurability[ItemInventoryLocation];
					Character->WeaponDurability[ItemInventoryLocation] = NULL;

					ItemWidget->ItemArray = Character->WeaponInventory;
					ItemWidget->Update();
				}
			}

			Character->AudioComponent->SetSound(Character->Equip_Sword_Cue);
			Character->AudioComponent->Play();
		}
	}

	else if (Character->ArmorInventory[ItemInventoryLocation] != nullptr && InventorySelect == 1)
	{
		if (!Character->HelmetComponent->GetChildComponent(0))
			IsCharacterEquipHelmet = false;

		if (!Character->SuitComponent->GetChildComponent(0))
			IsCharacterEquipSuit = false;

		if (Character->ArmorInventory[ItemInventoryLocation]->GetClass()->IsChildOf(AHelmet::StaticClass()))
		{
			FVector DropLocation = Character->CharacterComponent->GetSocketLocation("Helmet");
			FTransform Transform;

			Transform.SetLocation(DropLocation);

			FActorSpawnParameters SpawnParams;

			if (IsCharacterEquipHelmet == true)
			{
				AActor* ChildActor = Character->HelmetComponent->GetChildComponent(0)->GetOwner();
				AItem* ChildCasted = Cast<AItem>(ChildActor);

				AHelmet* Helmet = GetWorld()->SpawnActor<AHelmet>(Character->ArmorInventory[ItemInventoryLocation]->GetClass(), Transform, SpawnParams);

				Helmet->AttachRootComponentTo(Character->HelmetComponent, (TEXT("Helmet")), EAttachLocation::SnapToTarget);
				Helmet->GetCapsuleComponent()->SetCollisionProfileName("HoldWeapon");

				Helmet->Durability = Character->ArmorDurability[ItemInventoryLocation];

				if (ChildActor->GetActorClass()->IsChildOf(AIronHelmet::StaticClass()))
				{
					AIronHelmet* New = ConstructObject<AIronHelmet>(AIronHelmet::StaticClass());
					New->Durability = ChildCasted->Durability;
					Character->ArmorInventory[ItemInventoryLocation] = New;
					Character->ArmorDurability[ItemInventoryLocation] = New->Durability;

					New->Destroy();
				}
				else if (ChildActor->GetActorClass()->IsChildOf(AGoldenHelmet::StaticClass()))
				{
					AGoldenHelmet* New = ConstructObject<AGoldenHelmet>(AGoldenHelmet::StaticClass());
					New->Durability = ChildCasted->Durability;
					Character->ArmorInventory[ItemInventoryLocation] = New;
					Character->ArmorDurability[ItemInventoryLocation] = New->Durability;

					New->Destroy();
				}

				ChildActor->Destroy();

				ItemWidget->ItemArray = Character->ArmorInventory;
				ItemWidget->Update();
			}
			else if (IsCharacterEquipHelmet == false)
			{
				IsCharacterEquipHelmet = true;

				AHelmet* Helmet = GetWorld()->SpawnActor<AHelmet>(Character->ArmorInventory[ItemInventoryLocation]->GetClass(), Transform, SpawnParams);

				Helmet->AttachRootComponentTo(Character->HelmetComponent, (TEXT("Helmet")), EAttachLocation::SnapToTarget);
				Helmet->GetCapsuleComponent()->SetCollisionProfileName("HoldWeapon");

				Character->ArmorInventory[ItemInventoryLocation] = nullptr;

				Helmet->Durability = Character->ArmorDurability[ItemInventoryLocation];
				Character->ArmorDurability[ItemInventoryLocation] = NULL;

				ItemWidget->ItemArray = Character->ArmorInventory;
				ItemWidget->Update();
			}

			Character->AudioComponent->SetSound(Character->Equip_Armor_Cue);
			Character->AudioComponent->Play();
		}

		else if (Character->ArmorInventory[ItemInventoryLocation]->GetClass()->IsChildOf(ASuit::StaticClass()))
		{
			FVector DropLocation = Character->CharacterComponent->GetSocketLocation("Suit");
			FTransform Transform;

			Transform.SetLocation(DropLocation);

			FActorSpawnParameters SpawnParams;

			if (IsCharacterEquipSuit == true)
			{
				AActor* ChildActor = Character->SuitComponent->GetChildComponent(0)->GetOwner();
				AItem* ChildCasted = Cast<AItem>(ChildActor);

				ASuit* Suit = GetWorld()->SpawnActor<ASuit>(Character->ArmorInventory[ItemInventoryLocation]->GetClass(), Transform, SpawnParams);

				Suit->AttachRootComponentTo(Character->SuitComponent, (TEXT("Suit")), EAttachLocation::SnapToTarget);
				Suit->GetCapsuleComponent()->SetCollisionProfileName("HoldWeapon");
				Suit->Durability = Character->ArmorDurability[ItemInventoryLocation];

				if (ChildActor->GetActorClass()->IsChildOf(AIronSuit::StaticClass()))
				{
					AIronSuit* New = ConstructObject<AIronSuit>(AIronSuit::StaticClass());
					New->Durability = ChildCasted->Durability;
					Character->ArmorInventory[ItemInventoryLocation] = New;
					Character->ArmorDurability[ItemInventoryLocation] = New->Durability;

					New->Destroy();
				}
				else if (ChildActor->GetActorClass()->IsChildOf(AGoldenSuit::StaticClass()))
				{
					AGoldenSuit* New = ConstructObject<AGoldenSuit>(AGoldenSuit::StaticClass());
					New->Durability = ChildCasted->Durability;
					Character->ArmorInventory[ItemInventoryLocation] = New;
					Character->ArmorDurability[ItemInventoryLocation] = New->Durability;

					New->Destroy();
				}

				ChildActor->Destroy();

				ItemWidget->ItemArray = Character->ArmorInventory;
				ItemWidget->Update();
			}
			else if (IsCharacterEquipSuit == false)
			{
				IsCharacterEquipSuit = true;

				ASuit* Suit = GetWorld()->SpawnActor<ASuit>(Character->ArmorInventory[ItemInventoryLocation]->GetClass(), Transform, SpawnParams);

				Suit->AttachRootComponentTo(Character->SuitComponent, (TEXT("Suit")), EAttachLocation::SnapToTarget);
				Suit->GetCapsuleComponent()->SetCollisionProfileName("HoldWeapon");

				Character->ArmorInventory[ItemInventoryLocation] = nullptr;

				Suit->Durability = Character->ArmorDurability[ItemInventoryLocation];
				Character->ArmorDurability[ItemInventoryLocation] = NULL;

				ItemWidget->ItemArray = Character->ArmorInventory;
				ItemWidget->Update();
			}

			Character->AudioComponent->SetSound(Character->Equip_Armor_Cue);
			Character->AudioComponent->Play();
		}
	}
}

void AGameProjectPlayerController::ShakeCamera()
{
	this->ClientPlayCameraShake(CameraShake, 1.0f);
}