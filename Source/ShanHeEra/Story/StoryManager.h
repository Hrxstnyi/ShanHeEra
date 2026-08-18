#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/ShanHeStructs.h"
#include "Core/ShanHeEnums.h"
#include "StoryManager.generated.h"

USTRUCT(BlueprintType)
struct FSpecialCharacter
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName CharID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FPersonName Name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Title;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<ECharacterTrait> Traits;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FCharacterStats Stats;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Backstory;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName Location;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsHistorical = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsEasterEgg = false;
};

USTRUCT(BlueprintType)
struct FStoryQuest
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName QuestID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Title;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> RelatedCharacters;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsEasterEgg = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsTaiwanRelated = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsPatriotic = false;
};

UCLASS()
class SHANHEERA_API UStoryManager : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|剧情")
    void InitializeStory();

    UFUNCTION(BlueprintCallable, Category="山河纪元|剧情")
    TArray<FSpecialCharacter> GetAllCharacters() const { return SpecialCharacters; }

    UFUNCTION(BlueprintCallable, Category="山河纪元|剧情")
    FSpecialCharacter GetCharacter(FName CharID) const;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|剧情")
    TArray<FSpecialCharacter> SpecialCharacters;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|剧情")
    TArray<FStoryQuest> Quests;
};
