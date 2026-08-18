#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ShanHeCultureSystem.generated.h"

USTRUCT(BlueprintType)
struct FFestivalData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName FestivalID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Month = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Day = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Customs;
};

UCLASS()
class SHANHEERA_API UCultureSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|文化")
    void InitializeCulture();

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|文化")
    TArray<FFestivalData> Festivals;
};
