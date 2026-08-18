#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SocietySystem.generated.h"

USTRUCT(BlueprintType)
struct FOrganizationData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName OrgID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText OrgName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Type; // 宗族/行会/商帮/书院/镖局/武馆
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName BaseCity;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Influence = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Members = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Wealth = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
};

UCLASS()
class SHANHEERA_API USocietySystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|社会")
    void InitializeSociety();

    UFUNCTION(BlueprintCallable, Category="山河纪元|社会")
    void Simulate(int32 Days);

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|社会")
    TArray<FOrganizationData> Organizations;
};
