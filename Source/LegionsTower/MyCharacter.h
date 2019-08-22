// Electrum Labs 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h" //We add this include
#include "LTAttributeSet.h"
#include "MyCharacter.generated.h"


//It's very important that this enum is UENUM, because the code will look for UENUM by the given name and crash if the UENUM can't be found. BlueprintType is there so we can use these in blueprints, too. Just in case. Can be neat to define ability packages.
UENUM(BlueprintType)
enum class AbilityInput : uint8
{
	UseAbility1 UMETA(DisplayName = "Use Spell 1"), //This maps the first ability(input ID should be 0 in int) to the action mapping(which you define in the project settings) by the name of "UseAbility1". "Use Spell 1" is the blueprint name of the element.
	UseAbility2 UMETA(DisplayName = "Use Spell 2"), //Maps ability 2(input ID 1) to action mapping UseAbility2. "Use Spell 2" is mostly used for when the enum is a blueprint variable.
	UseAbility3 UMETA(DisplayName = "Use Spell 3"),
	UseAbility4 UMETA(DisplayName = "Use Spell 4"),
	WeaponAbility UMETA(DisplayName = "Use Weapon"), //This finally maps the fifth ability(here designated to be your weaponability, or auto-attack, or whatever) to action mapping "WeaponAbility".

};



UCLASS(config=Game)
class LEGIONSTOWER_API AMyCharacter : public ACharacter, public IAbilitySystemInterface
{

	//UAbilitySystemComponent* GetAbilitySystemComponent() const override {
	//	return AbilitySystem;
	//}

	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	AMyCharacter();

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	//TSubclassOf<class UGameplayAbility> Ability;

	UFUNCTION(BlueprintCallable)
	virtual float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	virtual float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable)
	virtual float GetHealthRegenPerSecond() const;

	UFUNCTION(BlueprintCallable)
	virtual float GetMana() const;

	UFUNCTION(BlueprintCallable)
	virtual float GetMaxMana() const;

	UFUNCTION(BlueprintCallable)
	virtual float GetManaRegenPerSecond() const;

	UFUNCTION(BlueprintCallable)
	virtual int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable)
	void RefreshSlottedAbilities();

	/** Our ability system */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	TSubclassOf<class UGameplayAbility> AbilityOne;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	TSubclassOf<class UGameplayAbility> AbilityTwo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	TSubclassOf<class UGameplayAbility> AbilityThree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	TSubclassOf<class UGameplayAbility> AbilityFour;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	/**  Attempt to activate abilities of tags
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation = true);
	**/

protected:  
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystem;

	// We add this function, overriding it from IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY()
	LTAttributeSet* AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	UDataTable* AttrDataTable;

	/** The level of this character, should not be modified directly once it has been spawned */
	UPROPERTY(EditAnywhere, Category = Abilities)
	int32 CharacterLevel;

	/** SlottedAbilities keeps track of the abilities that are granted onto the ability system component */
	/** Use to Retrieve the ability from the AbilitySystemComponent */
	TMap<int32, FGameplayAbilitySpecHandle> SlottedAbilities;

	/* Apply the startup gameplay abilities */
	void AddDefaultGameplayAbilities();

	/* Remove all abilities from the slot*/
	void RemoveSlottedAbilities();

	/* Add the abilities onto the slots */
	void AddSlottedAbilities();

	/* Fill the AbilitySpecs with the abilities given */
	void FillSlottedAbilitySpecs(TMap<int32, FGameplayAbilitySpec>& SlottedAbilitySpecs);

	/**
	 * Called when health is changed, either from healing or from being damaged
	 * For damage this is called in addition to OnDamaged/OnKilled
	 *
	 * @param DeltaValue Change in health value, positive for heal, negative for cost. If 0 the delta is unknown
	 * @param EventTags The gameplay tags of the event that changed mana
	 */
	UFUNCTION(BlueprintImplementableEvent)
		void OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	// Called from AttributeSet, these call BP events above
	virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	// Friended to allow access to handle functions
	friend LTAttributeSet;
};
