#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MedicalSystem.generated.h"

UENUM(BlueprintType)
enum class EIllnessType : uint8
{
    Cold        UMETA(DisplayName="风寒"),
    Fever       UMETA(DisplayName="发热"),
    Injury      UMETA(DisplayName="外伤"),
    Stomach     UMETA(DisplayName="肠胃"),
    Chronic     UMETA(DisplayName="痼疾"),
    Plague      UMETA(DisplayName="疫病"),
    Poison      UMETA(DisplayName="中毒")
};

USTRUCT(BlueprintType)
struct FHerbData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName HerbID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText HerbName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<EIllnessType> Treats;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Price = 100;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Rarity = 1; // 1-5
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName OriginRegion;
};

USTRUCT(BlueprintType)
struct FPrescription
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName PrescriptionID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> Herbs;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EIllnessType Treats;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Effectiveness = 50;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsSecret = false;
};

USTRUCT(BlueprintType)
struct FPatientCondition
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FGuid PatientID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EIllnessType Illness;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Severity = 1; // 1-5
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 DaysSick = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsRecovering = false;
};

/**
 * 医疗系统 - 中医诊断、药材、方剂、疫病治疗
 * 关联疫病系统和爱国彩蛋（医者仁心）
 */
UCLASS()
class SHANHEERA_API UMedicalSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|医疗")
    void InitializeMedicine();

    UFUNCTION(BlueprintCallable, Category="山河纪元|医疗")
    EIllnessType Diagnose(FGuid PatientID); // 望闻问切

    UFUNCTION(BlueprintCallable, Category="山河纪元|医疗")
    bool TreatPatient(FGuid PatientID, FName PrescriptionID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|医疗")
    TArray<FHerbData> GetHerbsForIllness(EIllnessType Illness) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|医疗")
    void SimulatePatients(int32 Days);

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|医疗")
    TArray<FHerbData> HerbDatabase;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|医疗")
    TArray<FPrescription> Prescriptions;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|医疗")
    TArray<FPatientCondition> Patients;

private:
    void GenerateRandomPatients();
};
