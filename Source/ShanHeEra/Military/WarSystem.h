#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/ShanHeStructs.h"
#include "Core/ShanHeEnums.h"
#include "WarSystem.generated.h"

USTRUCT(BlueprintType)
struct FArmyData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ArmyID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText ArmyName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName CommanderID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Infantry = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Cavalry = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Archers = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Navy = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Morale = 80;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Training = 50;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Supplies = 100;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName StationedCity;
};

USTRUCT(BlueprintType)
struct FWarData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName WarID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText WarName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName Attacker;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName Defender;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> TargetCities;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 DurationDays = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsActive = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EWarResult Result;
};

UCLASS()
class SHANHEERA_API UWarSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|军事")
    void InitializeMilitary();

    UFUNCTION(BlueprintCallable, Category="山河纪元|军事")
    void Simulate(int32 Days);

    UFUNCTION(BlueprintCallable, Category="山河纪元|军事")
    void DeclareWar(FName Attacker, FName Defender, FText WarName);

    UFUNCTION(BlueprintCallable, Category="山河纪元|军事")
    EWarResult ResolveBattle(FArmyData& Attacker, FArmyData& Defender, FName City);

    UFUNCTION(BlueprintCallable, Category="山河纪元|军事")
    void SiegeCity(FName CityID, FArmyData& Besieger);

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|军事")
    TArray<FArmyData> Armies;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|军事")
    TArray<FWarData> ActiveWars;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|军事")
    int32 TotalSoldiers = 0;
};
