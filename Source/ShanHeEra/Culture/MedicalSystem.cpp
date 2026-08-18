#include "Culture/MedicalSystem.h"
#include "Core/ShanHeLog.h"

void UMedicalSystem::InitializeMedicine()
{
    // 初始化药材库
    struct FHerbDef { FName ID; const TCHAR* Name; EIllnessType Treat; int32 Price; int32 Rarity; };
    FHerbDef Herbs[] = {
        {TEXT("Ginseng"), NSLOCTEXT("Herb","Ginseng","人参"), EIllnessType::Chronic, 5000, 5},
        {TEXT("Licorice"), NSLOCTEXT("Herb","Licorice","甘草"), EIllnessType::Cold, 100, 1},
        {TEXT("Ginger"), NSLOCTEXT("Herb","Ginger","生姜"), EIllnessType::Cold, 20, 1},
        {TEXT("Astragalus"), NSLOCTEXT("Herb","Astragalus","黄芪"), EIllnessType::Chronic, 300, 3},
        {TEXT("Angelica"), NSLOCTEXT("Herb","Angelica","当归"), EIllnessType::Injury, 200, 2},
        {TEXT("Coptis"), NSLOCTEXT("Herb","Coptis","黄连"), EIllnessType::Fever, 150, 2},
        {TEXT("Musk"), NSLOCTEXT("Herb","Musk","麝香"), EIllnessType::Poison, 3000, 5},
        {TEXT("Isatis"), NSLOCTEXT("Herb","Isatis","板蓝根"), EIllnessType::Plague, 200, 2},
    };
    for (const auto& H : Herbs)
    {
        FHerbData D;
        D.HerbID = H.ID; D.HerbName = H.Name; D.Treats.Add(H.Treat);
        D.Price = H.Price; D.Rarity = H.Rarity;
        HerbDatabase.Add(D);
    }

    // 初始化方剂
    FPrescription ColdFormula;
    ColdFormula.PrescriptionID = TEXT("ColdFormula");
    ColdFormula.Name = NSLOCTEXT("Med","ColdFormula","桂枝汤");
    ColdFormula.Herbs = {TEXT("Licorice"), TEXT("Ginger")};
    ColdFormula.Treats = EIllnessType::Cold;
    ColdFormula.Effectiveness = 70;
    Prescriptions.Add(ColdFormula);

    UE_LOG(LogShanHe, Log, TEXT("医疗系统初始化 - %d 种药材，%d 个方剂"), HerbDatabase.Num(), Prescriptions.Num());
}

EIllnessType UMedicalSystem::Diagnose(FGuid PatientID)
{
    // 简化：随机诊断，实际应该根据症状判断
    EIllnessType Types[] = {EIllnessType::Cold, EIllnessType::Fever, EIllnessType::Stomach, EIllnessType::Injury};
    return Types[FMath::RandRange(0, 3)];
}

bool UMedicalSystem::TreatPatient(FGuid PatientID, FName PrescriptionID)
{
    for (FPatientCondition& P : Patients)
    {
        if (P.PatientID == PatientID)
        {
            for (const FPrescription& F : Prescriptions)
            {
                if (F.PrescriptionID == PrescriptionID && F.Treats == P.Illness)
                {
                    P.bIsRecovering = true;
                    P.Severity = FMath::Max(1, P.Severity - F.Effectiveness / 20);
                    UE_LOG(LogShanHe, Log, TEXT("治疗开始：%s，方剂：%s"), *PatientID.ToString(), *F.Name.ToString());
                    return true;
                }
            }
        }
    }
    return false;
}

TArray<FHerbData> UMedicalSystem::GetHerbsForIllness(EIllnessType Illness) const
{
    TArray<FHerbData> Result;
    for (const FHerbData& H : HerbDatabase)
        if (H.Treats.Contains(Illness)) Result.Add(H);
    return Result;
}

void UMedicalSystem::SimulatePatients(int32 Days)
{
    for (int32 i = Patients.Num() - 1; i >= 0; i--)
    {
        FPatientCondition& P = Patients[i];
        P.DaysSick += Days;
        if (P.bIsRecovering)
        {
            P.Severity = FMath::Max(0, P.Severity - Days / 3);
            if (P.Severity <= 0)
            {
                UE_LOG(LogShanHe, Log, TEXT("病人康复：%s"), *P.PatientID.ToString());
                Patients.RemoveAt(i);
            }
        }
        else if (P.DaysSick > 30 && P.Severity >= 4)
        {
            UE_LOG(LogShanHe, Warning, TEXT("病人病危：%s"), *P.PatientID.ToString());
        }
    }
    if (FMath::RandRange(0, 100) < 10) GenerateRandomPatients();
}

void UMedicalSystem::GenerateRandomPatients()
{
    FPatientCondition P;
    P.PatientID = FGuid::NewGuid();
    P.Illness = (EIllnessType)FMath::RandRange(0, 3);
    P.Severity = FMath::RandRange(1, 3);
    Patients.Add(P);
}
