#include "GameProject.h"
#include "PlayerCharacter.h"

#include "GameProjectPlayerController.h"
#include "Projectile.h"
#include "Enemy.h"

#include "Matinee/MatineeActor.h"
#include "StopCamera.h"
#include "KingTeaser.h"

#include "Excaliber.h"
#include "GoldenHelmet.h"
#include "GoldenSuit.h"
#include "IronHelmet.h"
#include "IronSuit.h"
#include "ShortSword.h"
#include "Claymore.h"

#include "Ghost.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;				// 틱 작동 여부

	// BASIC SETTING //
	//
	////////////////////

	AutoPossessPlayer = EAutoReceiveInput::Player0;		// 시점 고정

	bUseControllerRotationPitch = false;				// Y축 회전
	bUseControllerRotationRoll = false;					// X축 회전
	bUseControllerRotationYaw = true;					// Z축 회전


	// FLIP BOOK //
	//
	////////////////////

	IdleAnimation = ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook>(TEXT("PaperFlipbook'/Game/Sprite/Flipbook/Character_IDE.Character_IDE'")).Get();
	RunningAnimation = ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook>(TEXT("PaperFlipbook'/Game/Sprite/Flipbook/Character_Running.Character_Running'")).Get();
	JumpingAnimation = ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook>(TEXT("PaperFlipbook'/Game/Sprite/Flipbook/Character_Jumping.Character_Jumping'")).Get();
	AttackingAnimation = ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook>(TEXT("PaperFlipbook'/Game/Sprite/Flipbook/Character_Attacking.Character_Attacking'")).Get();
	DeathAnimation = ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook>(TEXT("PaperFlipbook'/Game/Sprite/Flipbook/Character_Death.Character_Death'")).Get();

	// COLLISION //
	//
	////////////////////

	GetCapsuleComponent()->SetCapsuleRadius(20.0f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(30.0f);


	// CAMERA //
	//
	////////////////////

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));	// 카메라붐 세팅
	CameraBoom->TargetArmLength = 800.0f;											// 카메라붐 길이
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 200.0f);							// 카메라 소켓 위치
	CameraBoom->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);					// 카메라 회전 값
	CameraBoom->bAbsoluteRotation = true;											// 절대회전 값으로 전환
	CameraBoom->bDoCollisionTest = false;										    // 충돌 시험 설정
	CameraBoom->SetupAttachment(RootComponent);										// 카메라 부착

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->ProjectionMode = ECameraProjectionMode::Perspective;
	//Camera->OrthoWidth = 1500.0f;
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	Camera->bUsePawnControlRotation = false;
	Camera->bAutoActivate = true;


	// CHARACTER MOVEMENT //
	//
	////////////////////

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->AirControl = 1.0f;
	GetCharacterMovement()->GravityScale = 3.5f;
	GetCharacterMovement()->GroundFriction = 10.0f;
	GetCharacterMovement()->JumpZVelocity = 750.0f;
	GetCharacterMovement()->MaxFlySpeed = 450.0f;
	GetCharacterMovement()->MaxWalkSpeed = 450.0f;
	GetCharacterMovement()->FallingLateralFriction = 10.0f;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));


	// OTHER //
	//
	////////////////////

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OverlapStart);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OverlapStop);

	CharacterComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("SpriteComponent"));
	CharacterComponent->SetFlipbook(IdleAnimation);
	CharacterComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	CharacterComponent->SetupAttachment(GetCapsuleComponent());
	CharacterComponent->SetCollisionProfileName(TEXT("Sprite"));

	// Set Z-Order
	CharacterComponent->SetTranslucentSortPriority(-1);

	//static ConstructorHelpers::FObjectFinder<UMaterialInterface> ReferenceVariable(TEXT("MaterialInstanceConstant'/Paper2D/TranslucentUnlitSpriteMaterial.TranslucentUnlitSpriteMaterial'"));
	//ReferenceVariable.Succeeded();
	//CharacterComponent->SetMaterial(0, ReferenceVariable);


	UMaterialInterface* Material = ConstructorHelpers::FObjectFinderOptional<UMaterialInterface>(TEXT("MaterialInstanceConstant'/Paper2D/TranslucentUnlitSpriteMaterial.TranslucentUnlitSpriteMaterial'")).Get();
	CharacterComponent->SetMaterial(0, Material);


	WeaponComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponComponent"));
	WeaponComponent->SetupAttachment(CharacterComponent, (TEXT("Weapon")));
	WeaponComponent->SetSimulatePhysics(false);

	HelmetComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HelmetComponent"));
	HelmetComponent->SetupAttachment(CharacterComponent, (TEXT("Helmet")));
	HelmetComponent->SetSimulatePhysics(false);

	SuitComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SuitComponent"));
	SuitComponent->SetupAttachment(CharacterComponent, (TEXT("Suit")));
	SuitComponent->SetSimulatePhysics(false);

	// VARIABLE //
	//
	////////////////////

	JumpMaxHoldTime = 0.15f;			// 점프 최대치
	JumpValue = 0;						// 점프 횟수

	SetAttackAnimation = false;			// 공격 애니메이션 시작
	CountTemp = 0.0f;					// 공격 시간 초기화

	IsOverlapping = false;				// 캐릭터 겹침 여부
	IsHoldingWeapon = false;			// 캐릭터 무기 장착 여부
	OverlapActor = nullptr;				// 임시 액터 초기화

	WeaponInventory.SetNum(MAX_WEAPON_ITEM);	// 인벤토리 최대치 설정
	ArmorInventory.SetNum(MAX_ARMOR_ITEM);
	WeaponDurability.SetNum(MAX_WEAPON_ITEM);
	ArmorDurability.SetNum(MAX_ARMOR_ITEM);

	CameraStopOverlap = false;

	BackGroundComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BackgroundComponent"));
	BackGroundComponent->SetupAttachment(Camera);
	BackGroundComponent->SetRelativeRotation(FRotator(0.0f,90.0f,0.0f));

	BackGround = ConstructorHelpers::FObjectFinderOptional<UPaperSprite>(TEXT("PaperSprite'/Game/Sprite/clouds_landscapes_pixel_art_artwork_Wallpaper_HD_1920x1200_www_paperhi_com_Sprite.clouds_landscapes_pixel_art_artwork_Wallpaper_HD_1920x1200_www_paperhi_com_Sprite'")).Get();
	BackGroundComponent->SetSprite(BackGround);

	///////////////////////////////////////////////////////

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);

	AudioComponent->bAutoActivate = false;

	Jump_Cue = ConstructorHelpers::FObjectFinderOptional<USoundCue>(TEXT("SoundCue'/Game/Audio/Jump/173326__soundnimja__jump-1_Cue.173326__soundnimja__jump-1_Cue'")).Get();
	Equip_Sword_Cue = ConstructorHelpers::FObjectFinderOptional<USoundCue>(TEXT("SoundCue'/Game/Audio/Jump/107589__qat__unsheath-sword_Cue.107589__qat__unsheath-sword_Cue'")).Get();
	Equip_Armor_Cue = ConstructorHelpers::FObjectFinderOptional<USoundCue>(TEXT("SoundCue'/Game/Audio/Jump/180231_juryduty_armor_mp3cut_net__Cue.180231_juryduty_armor_mp3cut_net__Cue'")).Get();
	Damaged_Cue = ConstructorHelpers::FObjectFinderOptional<USoundCue>(TEXT("SoundCue'/Game/Audio/Jump/270326__littlerobotsoundfactory__hit-01_Cue.270326__littlerobotsoundfactory__hit-01_Cue'")).Get();
	Full_Cue = ConstructorHelpers::FObjectFinderOptional<USoundCue>(TEXT("SoundCue'/Game/Audio/Jump/124900__greencouch__beeps-18_Cue.124900__greencouch__beeps-18_Cue'")).Get();

	IsDeath = false;
}

void APlayerCharacter::OverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (OtherActor->GetActorClass()->IsChildOf(AItem::StaticClass()))
		{
			AItem* Item = Cast<AItem>(OtherActor);
			OverlapActor = Item;

			IsOverlapping = true;
		}

		if (OtherActor->GetActorClass()->IsChildOf(AStopCamera::StaticClass()))
		{
			PlayerLocation = RootComponent->GetComponentLocation();
			CameraStopOverlap = true;
		}

		if (OtherActor->GetActorClass()->IsChildOf(AKingTeaser::StaticClass()))
		{
			CameraBoom->DetachFromParent();
			CameraBoom->SetWorldLocation(FVector(47708.96875f, 700.0f, 2630.610107f));

			BackGroundComponent->DetachFromParent();
		}
	}
}

void APlayerCharacter::OverlapStop(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		IsOverlapping = false;
		OverlapActor = nullptr;

		if (CameraStopOverlap == true)
		{
			CameraBoom->SetRelativeLocation(FVector(1.0f, 0.0f, 0.0f));
			CameraStopOverlap = false;
		}
	}
}

void APlayerCharacter::UpdateAnimation()
{
	const float PlayerSpeedSqr = GetVelocity().SizeSquared();

	if (SetAttackAnimation != false)
		CharacterComponent->SetFlipbook(AttackingAnimation);

	else if (PlayerSpeedSqr == 0.0f)
	{

		//이걸로 고쳐볼 것
		//FVector SocketLocation;
		//SocketLocation = WeaponComponent->GetSocketLocation("Weapon");
		CharacterComponent->SetFlipbook(IdleAnimation);

		/*
		야매로 수정하다 포기한 무덤입니다
		FVector Temps = CharacterComponent->GetSocketLocation("Weapon");
		CharacterComponent->GetSocketLocation("Weapon") = Temps.Z - 10.0f;
		if(CharacterComponent->GetFlipbook() != IdleAnimation)
		WeaponComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -20.0f));
		if (IdleAnimation->GetSpriteAtTime(0.0f)->FindSocket(TEXT("Weapon")))
		*/
	}

	else if (PlayerSpeedSqr != 0.0f && !GetCharacterMovement()->IsFalling())
		CharacterComponent->SetFlipbook(RunningAnimation);

	else if (PlayerSpeedSqr != 0.0f && GetCharacterMovement()->IsFalling())
		CharacterComponent->SetFlipbook(JumpingAnimation);
}

void APlayerCharacter::UpdateCharacter()
{
	UpdateAnimation();

	const float TravelDirection = GetVelocity().X;

	// 움직임에 따라 캐릭터를 좌우 회전
	if (Controller != nullptr)
	{
		if (TravelDirection < 0.0f)
		{
			//FRotator ComponentRoataion = FRotator(180.0f, 180.0f, 180.0f);

			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
			//WeaponComponent->SetRelativeLocation(FVector(0.0f, -5.0f, 0.0f));
			//HelmetComponent->SetRelativeLocation(FVector(0.0f, -5.0f, 0.0f));
			//SuitComponent->SetRelativeLocation(FVector(0.0f, -5.0f, 0.0f));

		}
		else if (TravelDirection > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
			//WeaponComponent->SetRelativeLocation(FVector(0.0f, 5.0f, 0.0f));
			//HelmetComponent->SetRelativeLocation(FVector(0.0f, 5.0f, 0.0f));
			//SuitComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		}
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	AShortSword* New = ConstructObject<AShortSword>(AShortSword::StaticClass());
	WeaponInventory[0] = New;
	WeaponDurability[0] = New->Durability;

	New->Destroy();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsDeath)
	{
		UpdateCharacter();

		DELTATIME = DeltaTime;

		// 캐릭터가 땅에 닿을 경우 점프 횟수 초기화
		if (GetVelocity().Y == 0 && !GetCharacterMovement()->IsFalling())
			JumpValue = 0;

		if (CameraStopOverlap)
			CameraBoom->SetWorldLocation(FVector(PlayerLocation.X, 0.0f, RootComponent->RelativeLocation.Z));
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

		// Action Mapping
		InputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::Attack);
		InputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::JumpStart);
		InputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::JumpStop);

		InputComponent->BindAction("GetWeapon", IE_Pressed, this, &APlayerCharacter::GetWeapon);
		InputComponent->BindAction("OpenOption", IE_Pressed, this, &APlayerCharacter::OpenOption);

		// Axis Mapping
		InputComponent->BindAxis("Move", this, &APlayerCharacter::Move);

}

void APlayerCharacter::Move(float Value)
{
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
}

void APlayerCharacter::JumpStart()
{
		// 캐릭터가 공중에서 점프를 시도하는가?
		if (GetCharacterMovement()->IsFalling() && JumpValue < 1)
			JumpValue++;

		// 캐릭터의 점프 횟수가 2회 미만인가?
		if (JumpValue < 2)
		{
			bWasJumping = true;

			AudioComponent->SetSound(Jump_Cue);
			AudioComponent->Play();
		}

		JumpKeyHoldTime = 0.0f;
		bPressedJump = true;
		JumpValue++;
}

void APlayerCharacter::JumpStop()
{
		// 점프 종료
		bWasJumping = false;
		JumpKeyHoldTime = 0.0f;
}

void APlayerCharacter::Attack()
{
	if (!WeaponComponent->GetChildComponent(0))
		IsHoldingWeapon = false;

	if (!GetWorldTimerManager().IsTimerActive(CountdownHandle) && IsHoldingWeapon)
		GetWorldTimerManager().SetTimer(CountdownHandle, this, &APlayerCharacter::AttackTimer, 0.1f, true);
	
}

void APlayerCharacter::AttackTimer()
{
		if (WeaponComponent->GetChildComponent(0))
		{
			CountTemp++;

			AActor* ChildActor = WeaponComponent->GetChildComponent(0)->GetOwner();
			AWeapon* Weapon = Cast<AWeapon>(ChildActor);


			if (SetAttackAnimation == true && CountTemp >= 4)
			{
				CountTemp = 0;
				GetWorldTimerManager().ClearTimer(CountdownHandle);

				Weapon->GetCapsuleComponent()->SetCollisionProfileName("HoldWeapon");
				Weapon->SetActorRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

				SetAttackAnimation = false;
			}
			else
			{
				SetAttackAnimation = true;

				Weapon->GetCapsuleComponent()->SetCollisionProfileName("AttackWeapon");
				Weapon->SetActorRelativeRotation(FRotator(WeaponRotation[CountTemp], 0.0f, 0.0f));
			}
		}
		else
		{
			CountTemp = 0;
			GetWorldTimerManager().ClearTimer(CountdownHandle);
			SetAttackAnimation = false;
		}
}

void APlayerCharacter::GetWeapon()
{
	if (IsOverlapping == true)
	{
		int32 WeaponAvailableSlot = WeaponInventory.Find(NULL);
		int32 ArmorAvailableSlot = ArmorInventory.Find(NULL);

		if (OverlapActor->GetActorClass()->IsChildOf(AWeapon::StaticClass()))
		{
			if (WeaponAvailableSlot != INDEX_NONE)
			{
				if (OverlapActor->GetActorClass()->IsChildOf(AClaymore::StaticClass()))
				{
					AClaymore* New = ConstructObject<AClaymore>(AClaymore::StaticClass());
					New->Durability = OverlapActor->Durability;
					WeaponInventory[WeaponAvailableSlot] = New;

					New->Destroy();
				}
				else if (OverlapActor->GetActorClass()->IsChildOf(AShortSword::StaticClass()))
				{
					AShortSword* New = ConstructObject<AShortSword>(AShortSword::StaticClass());
					New->Durability = OverlapActor->Durability;
					WeaponInventory[WeaponAvailableSlot] = New;

					New->Destroy();
				}
				else if (OverlapActor->GetActorClass()->IsChildOf(AExcaliber::StaticClass()))
				{
					AExcaliber* New = ConstructObject<AExcaliber>(AExcaliber::StaticClass());
					New->Durability = OverlapActor->Durability;
					WeaponInventory[WeaponAvailableSlot] = New;

					New->Destroy();
				}

				WeaponDurability[WeaponAvailableSlot] = OverlapActor->Durability;

				OverlapActor->Destroy();
			}
			else
			{
				AudioComponent->SetSound(Full_Cue);
				AudioComponent->Play();
			}

		}
		else if (OverlapActor->GetActorClass()->IsChildOf(AArmor::StaticClass()))
		{
			if (ArmorAvailableSlot != INDEX_NONE)
			{
				if (OverlapActor->GetActorClass()->IsChildOf(AIronHelmet::StaticClass()))
				{
					AIronHelmet* New = ConstructObject<AIronHelmet>(AIronHelmet::StaticClass());
					New->Durability = OverlapActor->Durability;
					ArmorInventory[ArmorAvailableSlot] = New;

					New->Destroy();
				}
				else if (OverlapActor->GetActorClass()->IsChildOf(AIronSuit::StaticClass()))
				{
					AIronSuit* New = ConstructObject<AIronSuit>(AIronSuit::StaticClass());
					New->Durability = OverlapActor->Durability;
					ArmorInventory[ArmorAvailableSlot] = New;

					New->Destroy();
				}
				else if (OverlapActor->GetActorClass()->IsChildOf(AGoldenHelmet::StaticClass()))
				{
					AGoldenHelmet* New = ConstructObject<AGoldenHelmet>(AGoldenHelmet::StaticClass());
					New->Durability = OverlapActor->Durability;
					ArmorInventory[ArmorAvailableSlot] = New;

					New->Destroy();
				}
				else if (OverlapActor->GetActorClass()->IsChildOf(AGoldenSuit::StaticClass()))
				{
					AGoldenSuit* New = ConstructObject<AGoldenSuit>(AGoldenSuit::StaticClass());
					New->Durability = OverlapActor->Durability;
					ArmorInventory[ArmorAvailableSlot] = New;

					New->Destroy();
				}

				ArmorDurability[ArmorAvailableSlot] = OverlapActor->Durability;

				OverlapActor->Destroy();
			}
			else
			{
				AudioComponent->SetSound(Full_Cue);
				AudioComponent->Play();
			}
		}
		
	}
}

void APlayerCharacter::OpenOption()
{
	AGameProjectPlayerController* Controler = Cast<AGameProjectPlayerController>(GetController());

	// 플레이어가 이동 중인지 확인
	const float PlayerSpeedSqr = GetVelocity().SizeSquared();

	if (Controler && PlayerSpeedSqr == 0.0f)
		Controler->ControlOptionWidget();
}

void APlayerCharacter::StartInvincibleTimer()
{
	if (!GetWorldTimerManager().IsTimerActive(InvincibleTimer))
	{
		AGameProjectPlayerController* Controller = Cast<AGameProjectPlayerController>(GetController());

		if (HelmetComponent->GetChildComponent(0) && Controller->IsCharacterEquipHelmet == true)
		{
			AActor* ChildComponent = HelmetComponent->GetChildComponent(0)->GetOwner();
			AArmor* Helmet = Cast<AArmor>(ChildComponent);

			Helmet->Durability--;

			if (Helmet->Durability <= 0)
			{
				Helmet->DestroyArmor();
				Controller->IsCharacterEquipHelmet = false;
			}
		}
		else if (SuitComponent->GetChildComponent(0) && Controller->IsCharacterEquipSuit == true)
		{
			AActor* ChildComponent = SuitComponent->GetChildComponent(0)->GetOwner();
			AArmor* Suit = Cast<AArmor>(ChildComponent);

			Suit->Durability--;

			if (Suit->Durability <= 0)
			{
				Suit->DestroyArmor();
				Controller->IsCharacterEquipSuit = false;
			}
		}
		else
		{

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;

			FVector SpawnLocation = this->GetActorLocation();
			FRotator SpawnRotation = this->GetActorRotation();

			AGhost* Projectile = GetWorld()->SpawnActor<AGhost>(SpawnLocation, SpawnRotation, SpawnParams);

			GetWorldTimerManager().ClearTimer(InvincibleTimer);
			GetCapsuleComponent()->SetCollisionProfileName(TEXT("Invincible"));

			IsDeath = true;
			this->DisableInput(Controller);
			CharacterComponent->SetFlipbook(DeathAnimation);

			if (!GetWorldTimerManager().IsTimerActive(DeathTimer))
			{
				GetWorldTimerManager().SetTimer(DeathTimer, this, &APlayerCharacter::Death, 3.0f, true);
			}
		}

	}

	AudioComponent->SetSound(Damaged_Cue);
	AudioComponent->Play();

	GetWorldTimerManager().SetTimer(InvincibleTimer, this, &APlayerCharacter::SetCharacterInvincibleTimer, 0.1f, true);
}

void APlayerCharacter::SetCharacterInvincibleTimer()
{
	if (!IsDeath)
	{
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("Invincible"));
		CharacterComponent->SetSpriteColor(FLinearColor(1.0, 1.0, 1.0, 0.6));

		if (InvincibleTimeCount % 2 == 0)
			CharacterComponent->SetSpriteColor(FLinearColor(1.0, 1.0, 1.0, 0.6));
		else
			CharacterComponent->SetSpriteColor(FLinearColor(1.0, 1.0, 1.0, 1.0));

		InvincibleTimeCount++;

		if (InvincibleTimeCount >= 9)
		{
			InvincibleTimeCount = 0;
			GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
			CharacterComponent->SetSpriteColor(FLinearColor(1.0, 1.0, 1.0, 1.0));
			GetWorldTimerManager().ClearTimer(InvincibleTimer);
		}
	}
}

void APlayerCharacter::ShakeCamera()
{
	AGameProjectPlayerController* Controller = Cast<AGameProjectPlayerController>(GetController());
	Controller->ShakeCamera();
}

void APlayerCharacter::Death()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

/*

if (GEngine)
{
GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello World, this is FPSGameMode!"));
}

AActor* childActor = WeaponComponent->GetChildComponent(0)->GetOwner();

if (OverlapActor->GetClass()->IsChildOf(AShortSword::StaticClass()))
{
OverlapActor->Destroy();

FActorSpawnParameters SpawnParams;

AShortSword* ShortSword = GetWorld()->SpawnActor<AShortSword>(SocketLocation, SocketRotation, SpawnParams);
ShortSword->AttachRootComponentTo(WeaponComponent, (TEXT("Weapon")), EAttachLocation::SnapToTarget);

OverlapActor = ShortSword;
IsHoldingWeapon = true;
}


void APlayerCharacter::StartDropWeapon()
{
/*
HoldKeyTime += DELTATIME;

if (IsHoldingWeapon == true && HoldKeyTime >= 0.5f)
{
AActor* ChildActor = WeaponComponent->GetChildComponent(0)->GetOwner();
ChildActor->DetachRootComponentFromParent();
ChildActor->GetRootPrimitiveComponent()->SetSimulatePhysics(true);
ChildActor->GetRootPrimitiveComponent()->SetCollisionProfileName("Weapon");

IsHoldingWeapon = false;
HoldKeyTime = 0.0f;
}



}

void APlayerCharacter::StopDropWeapon()
{
	//HoldKeyTime = 0.0f;
}
*/

/*
if (IsOverlapping == true && IsHoldingWeapon == false)
{
AShortSword* WeaponTemp = Cast<AShortSword>(OverlapActor);
WeaponTemp->OverlapStop();

WeaponTemp->WeaponCollsion->SetCollisionProfileName("HoldingWeapon");

WeaponTemp->GetRootPrimitiveComponent()->SetSimulatePhysics(false);
WeaponTemp->AttachToComponent(WeaponComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Weapon"));
WeaponTemp->SetActorRelativeLocation(FVector(0.0f, 0.0f, 15.0f));

IsHoldingWeapon = true;
}
*/