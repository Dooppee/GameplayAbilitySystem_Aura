// Dop Practice Project.


#include "Character/AuraCharacterBase.h"

// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	
}
void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}
void AAuraCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

