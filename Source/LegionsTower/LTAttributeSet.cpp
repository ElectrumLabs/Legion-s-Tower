// Electrum Labs 2019


#include "LTAttributeSet.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MyCharacter.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"


LTAttributeSet::LTAttributeSet()
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
bool LTAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("PreGameplayEffectExecute"));
	return true;
}

/*
PostGameplayEffectExecute
This function is still in research and documenting
*/
void LTAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	/*
	*	Compute the delta between old and new, if it is available
	*/
	float DeltaValue = 0;
	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
	{
		// If this was additive, store the raw delta value to be passed along later
		DeltaValue = Data.EvaluatedData.Magnitude;
	}
	else if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Division)
	{
		// TODO DIVISION OPERATOR
		DeltaValue = Data.EvaluatedData.Magnitude;
	}
	else if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Max)
	{
		// TODO MAX
		DeltaValue = Data.EvaluatedData.Magnitude;
	}
	else if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Multiplicitive)
	{
		// TODO MULTIPLICITIVE
		DeltaValue = Data.EvaluatedData.Magnitude;
	}
	else if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Override)
	{
		// TODO OVERRIDE
		DeltaValue = Data.EvaluatedData.Magnitude;
	}

	/*
	*					GET THE TARGET OWNER, WHICH SHOULD BE OUR OWNER
	*/
	AActor* TargetActor = nullptr;
	APlayerController* TargetController = nullptr;
	MyCharacter* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<MyCharacter>(TargetActor);
	}

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// TODO implement how the value will affect the health attribute values

		// Handle other health changes such as from healing or direct modifiers
		// First clamp it
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

		if (TargetCharacter)
		{
			TargetCharacter->HandleHealthChanged(DeltaValue, SourceTags);
		}
	}
	else if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		// TODO implement how the value will affect the max health attribute values
	}
	else if (Data.EvaluatedData.Attribute == GetHealthRegenPerSecondAttribute())
	{
		// TODO implement how the value will affect the health regen per second attribute values
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		// TODO implement how the value will affect the mana attribute values
	}
	else if (Data.EvaluatedData.Attribute == GetMaxManaAttribute())
	{
		// TODO implement how the max mana attribute values will be affected
	}
	else if (Data.EvaluatedData.Attribute == GetManaRegenPerSecondAttribute())
	{
		// TODO implement how mana regen per second attribute value will be affected
	}

}

/*
PreAttributeChange
This function is still in research and documenting
*/
void LTAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
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
void LTAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	UE_LOG(LogTemp, Warning, TEXT("PreAttributeBaseChange"));
}

void LTAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComponent = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) ** AbilityComponent)
	{
		//  Change current value to maintain the current Value / Max Percent
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComponent->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGaemplayModOp::Additive, NewDelta);
	}
}

/* DEFINE_ATTRIBUTE_FUCNTION will define the GetAttribute function and returning the FGameplayAttribute
*  e.g
*  FGameplayAttribute LTAttributeSet::GetHealthAttribute()
*  {
*      static UProperty* Property = FindFieldChecked<UProperty>(UGASAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UGASAttributeSet, Health));
*      return FGameplayAttribute(Property);
*
*/
DEFINE_ATTRIBUTE_FUNCTION(Health, LTAttributeSet)

DEFINE_ATTRIBUTE_FUNCTION(MaxHealth, LTAttributeSet)

DEFINE_ATTRIBUTE_FUNCTION(HealthRegenPerSecond, LTAttributeSet)

DEFINE_ATTRIBUTE_FUNCTION(Mana, LTAttributeSet)

DEFINE_ATTRIBUTE_FUNCTION(MaxMana, LTAttributeSet)

DEFINE_ATTRIBUTE_FUNCTION(ManaRegenPerSecond, LTAttributeSet)