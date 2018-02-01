#pragma once

#include "GameFramework/Character.h"
#include "PaperCharacter.h"
#include "Item.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbook.h"
#include "PaperSprite.h"
#include "PlayerCharacter.generated.h"

#define MAX_WEAPON_ITEM 5
#define MAX_ARMOR_ITEM 5

UCLASS()
class GAMEPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	APlayerCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// ī�޶� ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraBoom;

	// ī�޶�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* Camera;

	// ĳ���� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite")
	class UPaperFlipbookComponent* CharacterComponent;

	// ���� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite")
	UStaticMeshComponent* WeaponComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite")
	UStaticMeshComponent* HelmetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite")
	UStaticMeshComponent* SuitComponent;

	// ��� �ִϸ��̼�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	class UPaperFlipbook* IdleAnimation;

	// �̵� �ִϸ��̼�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	class UPaperFlipbook* RunningAnimation;

	// ���� �ִϸ��̼�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	class UPaperFlipbook* JumpingAnimation;

	// ���� �ִϸ��̼�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	class UPaperFlipbook* AttackingAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	class UPaperFlipbook* DeathAnimation;

	// �κ��丮
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TArray<AItem*> WeaponInventory;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TArray<AItem*> ArmorInventory;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<int32> WeaponDurability;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<int32> ArmorDurability;

	// ������ �� True
	UFUNCTION()
	void OverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// ��ġ�� ���� �� False
	UFUNCTION()
	void OverlapStop(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	// �÷��̾� ������ �Լ�
	void Move(float Value);
	void Attack();
	void JumpStart();
	void JumpStop();

	// ĳ���� �ִϸ��̼� �Լ�
	void UpdateAnimation();
	void UpdateCharacter();

	// ���� ��� ���� 
	float DELTATIME;
	int32 JumpValue;

	// ���� ��������Ʈ �޴� ����
	class UPaperSprite* WeaponSprite;

	// ���� Ÿ�̸� ���
	void AttackTimer();
	FTimerHandle CountdownHandle;
	int32 CountTemp;
	bool SetAttackAnimation;

	///// �ӽ� ///////
	void GetWeapon();

	bool IsOverlapping;
	bool IsHoldingWeapon;
	float HoldKeyTime = 0.0f;

	float WeaponRotation[4] = { 0.0f, 0.0f, -50.0f, -75.0f };

	// �������� �ִ� ����
	UPROPERTY()
	AItem* OverlapActor;

	// ���� �κ��丮
	void OpenOption();

	FTimerHandle InvincibleTimer;
	void StartInvincibleTimer();
	void SetCharacterInvincibleTimer();
	int32 InvincibleTimeCount = 0;

	bool CameraStopOverlap;
	FVector PlayerLocation;

	void ShakeCamera();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite")
	class UPaperSpriteComponent* BackGroundComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sprite")
	class UPaperSprite* BackGround;

	///////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, Category = "Music")
	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, Category = "Music")
	USoundCue* Jump_Cue;

	UPROPERTY(EditAnywhere, Category = "Music")
	USoundCue* Equip_Sword_Cue;

	UPROPERTY(EditAnywhere, Category = "Music")
	USoundCue* Equip_Armor_Cue;

	UPROPERTY(EditAnywhere, Category = "Music")
	USoundCue* Damaged_Cue;

	UPROPERTY(EditAnywhere, Category = "Music")
	USoundCue* Full_Cue;

	FTimerHandle DeathTimer;
	void Death();

	bool IsDeath;
};