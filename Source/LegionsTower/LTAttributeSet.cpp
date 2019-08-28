// Electrum Labs 2019


#include "LTAttributeSet.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MyCharacter.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"


ULTAttributeSet::ULTAttributeSet()
	: Health(1.f)
	, MaxHealth(1.f)
	, HealthRegenPerSecond(1.f)
	, Mana(1.f)
	, MaxMana(1.f)
	, ManaRegenPerSecond(1.f)

{ }

/*
PreGameplayEffectExecute
This function is still in research and documenting
*/
bool ULTAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("PreGameplayEffectExecute"));
	return true;
}

/*
PostGameplayEffectExecute
This function is still in research and documenting
*/
void ULTAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
}

/*
PreAttributeChange
This function is still in research and documenting
*/
void ULTAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	UE_LOG(LogTemp, Warning, TEXT("PreAttributeChange"));
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		AdjustAttributeForMaxChange(Mana, MaxMana, NewValue, GetManaAttribute());
	}
}

/*
PreAttributeBaseChange
This function is still in research and documenting.
*/
void ULTAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	UE_LOG(LogTemp, Warning, TEXT("PreAttributeBaseChange"));
}

void ULTAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComponent = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComponent)
	{
		//  Change current value to maintain the current Value / Max Percent
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComponent->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

/* DEFINE_ATTRIBUTE_FUNCTION will define the GetAttribute function and returning the FGameplayAttribute
*  e.g
*  FGameplayAttribute LTAttributeSet::GetHealthAttribute()
*  {
*      static UProperty* Property = FindFieldChecked<UProperty>(UGASAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UGASAttributeSet, Health));
*      return FGameplayAttribute(Property);
*
*/
DEFINE_ATTRIBUTE_FUNCTION(Health, ULTAttributeSet)

DEFINE_ATTRIBUTE_FUNCTION(MaxHealth, ULTAttributeSet)

DEFINE_ATTRIBUTE_FUNCTION(HealthRegenPerSecond, ULTAttributeSet)

DEFINE_ATTRIBUTE_FUNCTION(Mana, ULTAttributeSet)

DEFINE_ATTRIBUTE_FUNCTION(MaxMana, ULTAttributeSet)

DEFINE_ATTRIBUTE_FUNCTION(ManaRegenPerSecond, ULTAttributeSet)