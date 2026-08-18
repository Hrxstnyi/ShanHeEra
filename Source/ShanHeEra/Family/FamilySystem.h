#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/ShanHeStructs.h"
#include "FamilySystem.generated.h"

USTRUCT(BlueprintType)
struct FFamilyData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName FamilyID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText FamilyName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText AncestralHome;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Prestige = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Wealth = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Generation = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FGuid> Members;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> FamilyTraditions;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText FamilyMotto;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsNoble = false;
};

UCLASS()
class SHANHEERA_API UFamilySystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|家族")
    void InitializeFamilies();

    UFUNCTION(BlueprintCallable, Category="山河纪元|家族")
    void Simulate(int32 Days);

    UFUNCTION(BlueprintCallable, Category="山河纪元|家族")
    void ArrangeMarriage(FGuid PersonA, FGuid PersonB);

    UFUNCTION(BlueprintCallable, Category="山河纪元|家族")
    FFamilyData GetFamily(FName FamilyID) const;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|家族")
    TArray<FFamilyData> Families;
};
