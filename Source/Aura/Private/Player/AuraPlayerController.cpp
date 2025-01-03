// Dop Practice Project.


#include "Player/AuraPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}
void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorResult;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility,false,CursorResult);
	if (!CursorResult.bBlockingHit)return;
	LastEnemy = ThisEnemy;
	ThisEnemy = Cast<IEnemyInterface>(CursorResult.GetActor());
	/**
	 *  Line trace from cursor. There have several scenery.
	 *  A. LastEnemy = null && ThisEnemy = null
	 *		->  Do nothing.
	 *	B. LastEnemy = null && ThisEnemy != null
	 *		->  Highlight ThisEnemy
	 *	C. LastEnemy != null && ThisEnemy = null
	 *		->  UnHighlihgt LastEnemy
	 *	D. LastEnemy!= null && ThisEnemy!= null , LastEnemy == ThisEnemy
	 *		->  Do nothing.
	 *	E. LastEnemy!= null && ThisEnemy!= null, LastEnemy != ThisEnemy
	 *		->  UnHighlihgt LastEnemy , Highlight ThisEnemy;
	 */
	if (LastEnemy == nullptr)
	{
		if (ThisEnemy == nullptr)
		{
			//Case: Do nothing
		}
		else
		{	//Cast : B
			ThisEnemy->HighLightActor();
		}
	}
	else
	{
		if (ThisEnemy == nullptr)
		{
			//Case : C
			LastEnemy->UnHighLightActor();
		}
		else
		{
			if (LastEnemy == ThisEnemy)
			{
				//Case : D
			}
			else
			{
				//Case : E
				LastEnemy->UnHighLightActor();
				ThisEnemy->HighLightActor();
			}
		}
	}
}
void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);
	//玩家子系统;
	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	//玩家子系统添加映射
	Subsystem->AddMappingContext(AuraContext,0);
	//设置鼠标状态
	bShowMouseCursor = true;
	//箭头种类
	DefaultMouseCursor = EMouseCursor::Type::Default;
	//设置光标，不要锁定再视口
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
	
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue) 
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotator = GetControlRotation();
	const FRotator YawRotator(0,Rotator.Yaw,0);

	const FVector ForwardDirection = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(Rotator).GetUnitAxis(EAxis::Y);
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection,InputAxisVector.X);
	}
}
