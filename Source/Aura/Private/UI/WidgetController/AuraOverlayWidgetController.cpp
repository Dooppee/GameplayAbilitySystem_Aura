// Dop Practice Project.


#include "UI/WidgetController/AuraOverlayWidgetController.h"
#include "AttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
//初始化广播
void UAuraOverlayWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	//当广播发生时，执行调用的函数
	OnHealthChange.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChange.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChange.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChange.Broadcast(AuraAttributeSet->GetMaxMana());
	
}

void UAuraOverlayWidgetController::BindCallbacksToDependencies()
{
	/*
	 * GetGameplayAttributeValueChangeDelegate -> 返回的 ATTRIBUTE_ACCESSORS 这个宏注册的属性值的代理
	 * AbilitySystemComponent 会在被初始化的时候自动隐式创建 所有被 UPROPERTY标记的FGameplayAttributeData的属性
	 * AuraAttributeSet->GetHealthAttribute() -> 返回 ATTRIBUTE_ACCESSORS 的 属性
	 * 由于返回的普通多播所以用AddUObject()这个函数
	 * 由于代
	 */
	UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnHealthChange.Broadcast(Data.NewValue);
		});
	

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChange.Broadcast(Data.NewValue);
		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData & Data)
		{
			OnManaChange.Broadcast(Data.NewValue);
		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChange.Broadcast(Data.NewValue);
		});

	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->GameEffectTagDelegate.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag: AssetTags)
			{
				FGameplayTag MessageTag =  FGameplayTag::RequestGameplayTag(FName("Message"));

				//"A.1".MatchesTag("A") will return True, "A".MatchesTag("A.1") will return False
				if (Tag.MatchesTag(MessageTag))
				{
					FUIWidgetRow* Row =
					GetTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable,Tag);
					MessageWidgetRowSignature.Broadcast(*Row);
				}
			}
		});
}

