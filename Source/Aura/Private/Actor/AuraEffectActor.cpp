// Dop Practice Project.


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"


AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComponent.Get());
	Level = 1;
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> InGameEffectClass)
{
	/*IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Target);
	UAbilitySystemComponent* ASCInterface = AbilitySystemInterface->GetAbilitySystemComponent();*/
	//可以直接用ASC中的静态函数库去拿到ASC;
	UAbilitySystemComponent* ASC_Target = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (ASC_Target==nullptr) return;
	check(InGameEffectClass);
	FGameplayEffectContextHandle GEContext = ASC_Target->MakeEffectContext();
	const FGameplayEffectSpecHandle GameplayEffectSpecHandle = ASC_Target->MakeOutgoingSpec(InGameEffectClass,Level,GEContext);
	const FActiveGameplayEffectHandle ActiveEffectHandle =
		ASC_Target->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());

	//只收集无限效果的
	const bool bIsInfinite = GameplayEffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy==EGameplayEffectDurationType::Infinite;

	if (bIsInfinite)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle,ASC_Target);
	}
	
}

void AAuraEffectActor::OnstartOverlap(AActor* InActor)
{
	if (InstantGEApplicationPolicy==EEffectApplicationPolicy::StartOverlapApply)
	{
		ApplyEffectToTarget(InActor,InstantGamePlayEffectClass);
	}
	if (DurativeGEApplicationPolicy==EEffectApplicationPolicy::StartOverlapApply)
	{
		ApplyEffectToTarget(InActor,DurativeGamePlayEffectClass);
	}
	if (InfiniteGEApplicationPolicy==EEffectApplicationPolicy::StartOverlapApply)
	{
		ApplyEffectToTarget(InActor,InfiniteGamePlayEffectClass);
	}
	
}

void AAuraEffectActor::OnEndOverlap(AActor* InActor)
{
	if (InfiniteGEApplicationPolicy==EEffectApplicationPolicy::EndOverlapApply)
	{
		ApplyEffectToTarget(InActor,InfiniteGamePlayEffectClass);
	}

	
	if (InfiniteGEApplicationRemove==EEffectRemovePolicy::EffectRemoved)
	{
		UAbilitySystemComponent* Target_ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor);
		if (!Target_ASC)return;
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		if (Target_ASC)
		{
			for (auto Temp : ActiveEffectHandles)
			{
				if (Temp.Value==Target_ASC)
				{
					Target_ASC->RemoveActiveGameplayEffect(Temp.Key,1);
					HandlesToRemove.Add(Temp.Key);
				}
			}
		}
		for (auto& Temp : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Temp);
		}
	}
}

/*void AAuraEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO:临时改变AuraAttributeSet的属性，const_cast
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		//在这个地方生成了属性列表的实例，返回的是这个实例。
		//GetAttributeSet(UAuraAttributeSet::StaticClass() 这里其实是便利ABC中的ABS并返回。
		//什么时候ABS什么时候被ABC所拥有？
		//ABS在创建的时候发生了隐式注册，在创建的时候会被隐式注册到ASC中。
		const UAuraAttributeSet* AuraAttributeSet =
			Cast<UAuraAttributeSet>( ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UAuraAttributeSet::StaticClass()));
		if (AuraAttributeSet)
		{
			UAuraAttributeSet* MutableUAuraAttributeSet = const_cast<UAuraAttributeSet*>(AuraAttributeSet);
			MutableUAuraAttributeSet->SetHealth(AuraAttributeSet->GetHealth()+25.f);
			Destroy();
		}
	}

}*/


void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

