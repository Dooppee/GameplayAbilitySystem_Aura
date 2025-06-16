// Dop Practice Project.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()
public:
	AAuraCharacter();
	
protected:
	//服务器调用 服务器分配Pawn给PlayerController
	virtual void PossessedBy(AController* NewController) override;
	//本地调用
	virtual void OnRep_PlayerState() override;
private:
	virtual void InitAbilityActorInfo() override; 
};
