#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ShanHeReligionSystem.generated.h"

UENUM(BlueprintType)
enum class EReligionType : uint8
{
    Buddhism    UMETA(DisplayName="佛教"),
    Taoism      UMETA(DisplayName="道教"),
    Confucianism UMETA(DisplayName="儒教"),
    Folk        UMETA(DisplayName="民间信仰"),
    None        UMETA(DisplayName="无信仰")
};

USTRUCT(BlueprintType)
struct FReligionData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EReligionType Type;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Followers = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Influence = 0.0f; // 影响力 0-100
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> Temples;
};

USTRUCT(BlueprintType)
struct FTempleData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName TempleID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText TempleName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EReligionType Religion;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName CityID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Incense = 0; // 香火钱
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Monks = 5;
};

/**
 * 宗教信仰系统 - 佛教/道教/儒教/民间信仰，寺庙香火，宗教影响民心
 */
UCLASS()
class SHANHEERA_API UReligionSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|宗教")
    void InitializeReligions();

    UFUNCTION(BlueprintCallable, Category="山河纪元|宗教")
    void DonateToTemple(FName TempleID, int32 Silver);

    UFUNCTION(BlueprintCallable, Category="山河纪元|宗教")
    void ConvertTo(EReligionType NewReligion);

    UFUNCTION(BlueprintCallable, Category="山河纪元|宗教")
    void SimulateReligion(int32 Days);

    UFUNCTION(BlueprintCallable, Category="山河纪元|宗教")
    float GetReligionInfluence(EReligionType Type) const;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|宗教")
    TMap<EReligionType, FReligionData> Religions;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|宗教")
    TArray<FTempleData> Temples;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|宗教")
    EReligionType PlayerFaith = EReligionType::None;
};
