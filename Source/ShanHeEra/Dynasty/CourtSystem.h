#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/ShanHeStructs.h"
#include "Core/ShanHeEnums.h"
#include "ShanHeCourtSystem.generated.h"

USTRUCT(BlueprintType)
struct FOfficialData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FGuid OfficialID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FPersonName Name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Rank = 9; // 九品
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Position;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName AssignedCity;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 GovernanceScore = 50;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Corruption = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<ECharacterTrait> Traits;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName Faction;
};

USTRUCT(BlueprintType)
struct FFactionData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName FactionID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText FactionName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Ideology;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Influence = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FGuid> Members;
};

USTRUCT(BlueprintType)
struct FEmperorData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FPersonName Name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Age = 40;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FCharacterStats Stats;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<ECharacterTrait> Traits;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 ReignYears = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsRegent = false;
};

UCLASS()
class SHANHEERA_API UCourtSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|朝廷")
    void InitializeCourt();

    UFUNCTION(BlueprintCallable, Category="山河纪元|朝廷")
    void Simulate(int32 Days);

    UFUNCTION(BlueprintCallable, Category="山河纪元|朝廷")
    void ConductImperialExamination(); // 科举

    UFUNCTION(BlueprintCallable, Category="山河纪元|朝廷")
    void PromoteOfficial(FGuid OfficialID, int32 NewRank);

    UFUNCTION(BlueprintCallable, Category="山河纪元|朝廷")
    void StartReform(FText ReformName);

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|朝廷")
    FEmperorData Emperor;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|朝廷")
    TArray<FOfficialData> Officials;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|朝廷")
    TArray<FFactionData> Factions;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|朝廷")
    float ImperialAuthority = 70.0f;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|朝廷")
    float CourtStability = 60.0f;

private:
    void SimulatePolitics(int32 Days);
    void SimulateFactions(int32 Days);
    void SimulateOfficialPerformance(int32 Days);
    void CheckSuccession();
};
