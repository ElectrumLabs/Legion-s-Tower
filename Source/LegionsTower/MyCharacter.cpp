// Electrum Labs 2019


#include "MyCharacter.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GameplayAbility.h"

UAbilitySystemComponent* MyCharacter::GetAbilitySystemComponent() const { return AbilitySystem;  }

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));

	// Create the attribute set, the attribute is set up to communicate with the AbilitySystemComponent Automatically
	AttributeSet = CreateDefaultSubobject<LTAttributeSet>(TEXT("AttributeSet"));

	CharacterLevel = 1;

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	AddDefaultGameplayAbilities();

	/** Initialize the AttributeSet with the Database given */
	if (AbilitySystem && AttrDataTable)
	{
		const UAttributeSet* Attr = AbilitySystem->InitStats(LTAttributeSet::StaticClass(), AttrDataTable);
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	AbilitySystem->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbilityInputBinds("ConfirmInput", "CancelInput", "AbilityInput"));

}

void AMyCharacter::PossessedBy(AController * NewController) {
	Super::PossessedBy(NewController);

	AbilitySystem->RefreshAbilityActorInfo();
}

//  new stuff

void LTCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void LTCharacter::RefreshSlottedAbilities()
{
	RemoveSlottedAbilities();
	AddSlottedAbilities();
}

void LTCharacter::AddDefaultGameplayAbilities()
{
	AddSlottedAbilities();
}

void LTCharacter::RemoveSlottedAbilities()
{
	for (TPair<int32, FGameplayAbilitySpecHandle> SpecHandle : SlottedAbilities)
	{
		AbilitySystem->ClearAbility(SpecHandle.Value);
		SlottedAbilities.Remove(SpecHandle.Key);
	}
}

void LTCharacter::AddSlottedAbilities()
{
	TMap<int32, FGameplayAbilitySpec> SlottedAbilitySpecs;
	FillSlottedAbilitySpecs(SlottedAbilitySpecs);

	/** now adding the abilities onto the ability system and also keeping track of the SpecHandles */
	for (TPair<int32, FGameplayAbilitySpec>& SpecPair : SlottedAbilitySpecs)
	{
		FGameplayAbilitySpecHandle &SpecHandle = SlottedAbilities.FindOrAdd(SpecPair.Key);

		/* Check if it is not valid, it means that their is no skill or invalid skill */
		if (!SpecHandle.IsValid())
		{
			SpecHandle = AbilitySystem->GiveAbility(SpecPair.Value);
		}

	}
}

void LTCharacter::FillSlottedAbilitySpecs(TMap<int32, FGameplayAbilitySpec>& SlottedAbilitySpecs)
{
	if (AbilitySystem)
	{
		if (HasAuthority() && AbilityOne)
		{
			SlottedAbilitySpecs.Add(
				0,
				FGameplayAbilitySpec(AbilityOne.GetDefaultObject(), 1, 0)
			);
		}
		if (HasAuthority() && AbilityTwo)
		{
			SlottedAbilitySpecs.Add(
				1,
				FGameplayAbilitySpec(AbilityTwo.GetDefaultObject(), 1, 1)
			);
		}
		if (HasAuthority() && AbilityThree)
		{
			SlottedAbilitySpecs.Add(
				2,
				FGameplayAbilitySpec(AbilityThree.GetDefaultObject(), 1, 2)
			);
		}
		if (HasAuthority() && AbilityFour)
		{
			SlottedAbilitySpecs.Add(
				3,
				FGameplayAbilitySpec(AbilityFour.GetDefaultObject(), 1, 3)
			);
		}
	}
}

void LTCharacter::HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
	OnHealthChanged(DeltaValue, EventTags);
}

float LTCharacter::GetHealth() const
{
	return AttributeSet->GetHealth();
}

float LTCharacter::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

float LTCharacter::GetHealthRegenPerSecond() const
{
	return AttributeSet->GetHealthRegenPerSecond();
}

float LTCharacter::GetMana() const
{
	return AttributeSet->GetMana();
}

float LTCharacter::GetMaxMana() const
{
	return AttributeSet->GetMaxMana();
}

float LTCharacter::GetManaRegenPerSecond() const
{
	return AttributeSet->GetManaRegenPerSecond();
}

int32 LTCharacter::GetCharacterLevel() const
{
	return CharacterLevel;
}
