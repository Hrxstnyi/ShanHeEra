#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/ShanHeStructs.h"
#include "Core/ShanHeEnums.h"
#include "ShanHeNPCManager.generated.h"

USTRUCT(BlueprintType)
struct FNPData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FGuid NPCID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FPersonName Name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FCharacterStats Stats;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ESocialClass SocialClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<ECharacterTrait> Traits;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Age = 30;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsAlive = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName CurrentCity;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName Occupation;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName FamilyID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Gold = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TMap<FGuid, int32> Relations; // 关系值 -100~100
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText LifeGoal;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsHistoricalFigure = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText HistoricalNote;
};

UCLASS()
class SHANHEERA_API UNPCManager : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|NPC")
    void InitializeNPCs();

    UFUNCTION(BlueprintCallable, Category="山河纪元|NPC")
    void SimulateNPCs(int32 Days);

    UFUNCTION(BlueprintCallable, Category="山河纪元|NPC")
    TArray<FNPData> GetNPCsInCity(FName CityID) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|NPC")
    FNPData GetNPC(FGuid NPCID) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|NPC")
    int32 GetRelation(FGuid A, FGuid B) const;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|NPC")
    TArray<FNPData> AllNPCs;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|NPC")
    int32 TotalNPCs = 0;

private:
    void SimulateDailyLife(FNPData& NPC, int32 Days);
    void SimulateCareer(FNPData& NPC, int32 Days);
    void SimulateRelations(FNPData& NPC, int32 Days);
    void CheckBirthDeath(FNPData& NPC, int32 Days);
};
