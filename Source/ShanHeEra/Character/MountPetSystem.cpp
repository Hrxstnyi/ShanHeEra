#include "Character/MountPetSystem.h"
#include "Core/ShanHeLog.h"

void UMountPetSystem::InitializeMountsPets()
{
    UE_LOG(LogShanHe, Log, TEXT("坐骑宠物系统初始化"));
}

bool UMountPetSystem::AcquireMount(FMount NewMount)
{
    Mounts.Add(NewMount);
    UE_LOG(LogShanHe, Log, TEXT("获得坐骑：%s"), *NewMount.Name.ToString());
    return true;
}

bool UMountPetSystem::AcquirePet(FPet NewPet)
{
    Pets.Add(NewPet);
    UE_LOG(LogShanHe, Log, TEXT("获得宠物：%s"), *NewPet.Name.ToString());
    return true;
}

void UMountPetSystem::FeedMount(FName MountID)
{
    for (FMount& M : Mounts)
    {
        if (M.MountID == MountID)
        {
            M.Stamina = FMath::Min(100, M.Stamina + 20);
            M.Loyalty = FMath::Min(100, M.Loyalty + 2);
            UE_LOG(LogShanHe, Log, TEXT("喂养坐骑：%s，体力+20，忠诚+2"), *M.Name.ToString());
            return;
        }
    }
}

void UMountPetSystem::TrainMount(FName MountID, float Hours)
{
    for (FMount& M : Mounts)
    {
        if (M.MountID == MountID)
        {
            M.Speed = FMath::Min(100, M.Speed + (int32)(Hours * 0.5f));
            M.Stamina = FMath::Max(0, M.Stamina - (int32)(Hours * 5));
            M.Loyalty = FMath::Min(100, M.Loyalty + (int32)(Hours * 0.3f));
            UE_LOG(LogShanHe, Log, TEXT("训练坐骑：%s，%.1f小时"), *M.Name.ToString(), Hours);
            return;
        }
    }
}

void UMountPetSystem::PlayWithPet(FName PetID)
{
    for (FPet& P : Pets)
    {
        if (P.PetID == PetID)
        {
            P.Affection = FMath::Min(100, P.Affection + 5);
            UE_LOG(LogShanHe, Log, TEXT("陪伴宠物：%s，亲密度+5"), *P.Name.ToString());
            return;
        }
    }
}

int32 UMountPetSystem::HuntWithPet(FName PetID)
{
    for (FPet& P : Pets)
    {
        if (P.PetID == PetID)
        {
            int32 Gain = P.HuntingSkill + FMath::RandRange(0, 30);
            P.Affection = FMath::Max(0, P.Affection - 2);
            UE_LOG(LogShanHe, Log, TEXT("狩猎收益：%d"), Gain);
            return Gain;
        }
    }
    return 0;
}

bool UMountPetSystem::MountCombatBonus(FName MountID) const
{
    for (const FMount& M : Mounts)
    {
        if (M.MountID == MountID && M.bIsWarMount && M.Loyalty >= 50)
            return true;
    }
    return false;
}
