// Dop Practice Project.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityEffectSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::GameplayEffect);
}

void UAuraAbilitySystemComponent::GameplayEffect(UAbilitySystemComponent* AbilitySystemComponent,
                                                 const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	//标签容器专门用来存放Tag的
	FGameplayTagContainer OwnedGameplayTags;
	GameplayEffectSpec.GetAllAssetTags(OwnedGameplayTags);
	GameEffectTagDelegate.Broadcast(OwnedGameplayTags);
}
