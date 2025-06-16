// Dop Practice Project.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"


UENUM(BlueprintType)
enum class EEffectApplicationPolicy
{
	StartOverlapApply,
	EndOverlapApply,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovePolicy
{
	EffectRemoved,
	DoNotRemove
};

class UGameplayEffect;
class USphereComponent;
class UAbilitySystemComponent;
struct FGameplayEffectContextHandle;

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();
	/*UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);*/
	UFUNCTION(BlueprintCallable,Category="Effect Actor")
	void ApplyEffectToTarget(AActor* TargetActor,TSubclassOf<UGameplayEffect> InGamePlayerEffectClass);
	UFUNCTION(BlueprintCallable,Category="Effect Actor")
	void OnstartOverlap(AActor* InActor);
	UFUNCTION(BlueprintCallable,Category="Effect Actor")
	void OnEndOverlap(AActor* InActor);
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere,Category="Effect Actor")
	TObjectPtr<USceneComponent> Root;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Effect Actor")
	TSubclassOf<UGameplayEffect> InstantGamePlayEffectClass;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Effect Actor")
	EEffectApplicationPolicy InstantGEApplicationPolicy  = EEffectApplicationPolicy::DoNotApply;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Effect Actor")
	TSubclassOf<UGameplayEffect> DurativeGamePlayEffectClass;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Effect Actor")
	EEffectApplicationPolicy DurativeGEApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Effect Actor")
	TSubclassOf<UGameplayEffect> InfiniteGamePlayEffectClass;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Effect Actor")
	EEffectApplicationPolicy InfiniteGEApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Effect Actor")
	EEffectRemovePolicy  InfiniteGEApplicationRemove = EEffectRemovePolicy::DoNotRemove;
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Effect Actor")
	float Level;
	
};
