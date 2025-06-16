// Dop Practice Project.


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(50.f);
	InitMaxHealth(100.f);
	InitMana(50.f);
	InitMaxMana(100.f);
}

/*
* 完整工作流程
	1、服务器修改 Health
	通过 AbilitySystemComponent->SetNumericAttributeBase() 或 GameplayEffect 修改属性。
	2、网络同步
	UE 自动将 Health 同步到所有客户端。
	3、客户端触发 OnRep_Health
	调用 GAMEPLAYATTRIBUTE_REPNOTIFY，通知GAS系统和蓝图逻辑。
	4、客户端响应
	更新UI、播放受击动画、处理预测错误等。
 */
/* 我的理解
 *1、那我是不是可以这样理解，重写GetLifetimeReplicateProps(Actor中的函数，所有的Actor都有的网络复制函数)重写这个函数
 *GetLifetimeReplicateProps这个函数会在Actor生成的时候、属性更新时候、网络重连的时候调用
 *在这个函数中通过DOREPLIFETIME_CONDITION_NOTIFY这个宏将Health这个属性进行注册，也就是说这个宏是必须和这个函数连用的？
 *当我将Health注册后Health这个数据具备了网络同步的条件？当这个数值发生改变的时候会通过Replicatedusing这个宏发生回调，
 *回调的函数是OnRep_Health这个函数，这个函数中通过GAMEPLAYATTRIBUTE_REPNOTIFY这个宏实现对整个GAS系统的通知，
 *通知GAS系统中与Health相关的UI和受击动画等？
 */

/* 修正后
在Unreal Engine的网络同步系统中，所有需要网络复制的Actor都需要重写AActor基类中的GetLifetimeReplicatedProps函数。
这个函数会在三个关键时机被自动调用：1)Actor初次生成时；2)属性值发生变化时；3)网络重连时。在这个函数中，
我们通过DOREPLIFETIME_CONDITION_NOTIFY宏将Health属性注册为需要网络同步的属性，这个宏必须在此函数内使用才能生效。

当Health属性被成功注册后，它就具备了网络同步的条件。此时需要确保：
1)属性已标记UPROPERTY(ReplicatedUsing=OnRep_Health)；
2)Actor在构造函数中设置了SetReplicates(true)。

当服务器端的Health值发生变化时，网络系统会自动将其同步到客户端，并触发通过ReplicatedUsing指定的OnRep_Health回调函数。在这个回调函数中：
1)我们使用GAMEPLAYATTRIBUTE_REPNOTIFY宏通知GAS系统属性已更新；
2)该宏会触发GAS的预测修正机制；
3)同时广播属性变更事件
 */
//必须重写此函数以注册同步属性
void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	//网络同步的宏
	//当Health发生变化的时候，会通过ReplicatedUsing这个宏实现对= OnRep_MaxHealth的回调
	//下面的这个宏是将Health这个数据进行注册，注册进行网络复制
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Health,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Mana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);
	
}

void UAuraAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	//一般用来限制传输的数据
	if (Attribute==GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}
	
}

void UAuraAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	//A向B进行了GE
	//拿到源的数据，数据有 ASC;PC;ACharacter;
	SetEffectProperties(Data,Properties);
}
void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data,
	FEffectProperties& Props) const
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			ACharacter* SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}
	
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		//拿到源的数据，数据有 ASC;PC;ACharacter;
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor); 
	}
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	//GAS提供的宏
	//当与 Gameplay Ability System 一起使用时，这种设置确保了属性变化能正确触发 GAS 的各种回调
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Health,OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,MaxHealth,OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Mana,OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,MaxMana,OldMaxMana);
}
