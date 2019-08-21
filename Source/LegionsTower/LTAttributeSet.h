// Electrum Labs 2019

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "LTAttributeSet.generated.h"

/*  DECLARE_ATTRIBUTE_FUNCTION Macro declearse a function in the header, returning a "FGameplayAttribute */
#define DECLARE_ATTRIBUTE_FUNCTION(PropertyName) static FGameplayAttribute PropertyName##Attribute();

/*  DEFINE_ATTRIBUTE_FUNCTION Macro defines the function that was declared in the header to the source file*/
#define DEFINE_ATTRIBUTE_FUNCTION(PropertyName, ClassName) \
FGameplayAttribute ClassName##::PropertyName##Attribute() \
{ \
	static UProperty* Property = FindFieldChecked<UProperty>(ClassName##::StaticClass(), GET_MEMBER_NAME_CHECKED(ClassName, PropertyName)); \
	return FGameplayAttribute(Property); \
}

/*  ATTRIBUTE_ACCESSORS Macro uses the 4 Macros that were given in the "AttributeSet.h" to create functions such as a getter, setter, init, and getting attribute. */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 *
 */
UCLASS()
class LEGIONSTOWER_API ULTAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	/*  Sets default values for attributes.  */
	LTAttributeSet();

	/*  Handles current health.  */
	UPROPERTY()
	FGameplayAttributeData Health;
	DECLARE_ATTRIBUTE_FUNCTION(Health)
	ATTRIBUTE_ACCESSORS(LTAttributeSet, Health)

	/*  Handles max health.  */
	UPROPERTY()
	FGameplayAttributeData MaxHealth;
	DECLARE_ATTRIBUTE_FUNCTION(MaxHealth)
	ATTRIBUTE_ACCESSORS(LTAttributeSet, MaxHealth)

	/*  Handles HP Regen.  */
	UPROPERTY()
	FGameplayAttributeData HealthRegenPerSecond;
	DECLARE_ATTRIBUTE_FUNCTION(HealthRegenPerSecond)
	ATTRIBUTE_ACCESSORS(LTAttributeSet, HealthRegenPerSecond)

	/*  Handles current mana.  */
	UPROPERTY()
	FGameplayAttributeData Mana;
	DECLARE_ATTRIBUTE_FUNCTION(Mana)
	ATTRIBUTE_ACCESSORS(LTAttributeSet, Mana)

	/*  Handles max mana.  */
	UPROPERTY()
	FGameplayAttributeData MaxMana;
	DECLARE_ATTRIBUTE_FUNCTION(MaxMana)
	ATTRIBUTE_ACCESSORS(LTAttributeSet, MaxMana)

	/*  Handles Mana Regen.  */
	UPROPERTY()
	FGameplayAttributeData ManaRegenPerSecond;
	DECLARE_ATTRIBUTE_FUNCTION(ManaRegenPerSecond)
	ATTRIBUTE_ACCESSORS(LTAttributeSet, ManaRegenPerSecond)

	/*
	Called just before modifying the value of an attribute.  AttributeSet can make additional modifcations here.  Return true to continue, or false to throw out the modification.
	NOTE:  This is only called during an 'execute'.
		E.g.  A modification to the 'base value' of an attribute.  It is NOT called during an application of a GameplayEffect, such as a temporary move speed buff.
	*/
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;

	/*
	Called just after a GameplayEffect is executed to modify the base value of an attribute.  No more changes can be made.
	NOTE:  This is only called during an 'execute'.
		E.g.  A modification to the 'base value' of an attribute.  It is NOT called during an application of a GameplayEffect, such as a temporary move speed buff.
	*/
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	/*
	Called just before any modification happens to an attribute. This is lower level than PreAttributeModify/PostAttribute modify.
	There is no additional context provided here since anything can trigger this. Executed effects, duration based effects, effects being removed, immunity being applied, stacking rules changing, etc.
	This function is meant to enforce things like "Health = Clamp(Health, 0, MaxHealth)" and NOT things like "trigger this extra thing if damage is applied, etc".
	
	NewValue is a mutable reference so you are able to clamp the newly applied value as well.
	 */
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	/*
	This is called just before any modification happens to an attribute's base value when an attribute aggregator exists.
	This function should enforce clamping (presuming you wish to clamp the base value along with the final value in PreAttributeChange)
	This function should NOT invoke gameplay related events or callbacks. Do those in PreAttributeChange() which will be called prior to the
	final value of the attribute actually changing.
	 */
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

protected:

	/* Helper function to proportionally adjust the value of an attribute when it's associated max attribute changes. 
	(i.e. When MaxHealth increases, Health increases by an amount that maintains the same percentage as before) */
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);

};
