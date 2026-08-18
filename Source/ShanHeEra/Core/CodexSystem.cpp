#include "Core/CodexSystem.h"
#include "Core/ShanHeLog.h"

void UCodexSystem::InitializeCodex()
{
    struct FCodexDef { FName ID; ECodexCategory Cat; const TCHAR* Name; const TCHAR* Desc; int32 Rarity; };
    FCodexDef Entries[] = {
        {TEXT("item_sword"), ECodexCategory::Item, TEXT("精钢剑"), TEXT("以精钢锻造的利剑，锋利无比。"), 2},
        {TEXT("item_jade"), ECodexCategory::Item, TEXT("和氏璧"), TEXT("传世美玉，价值连城。"), 5},
        {TEXT("char_libai"), ECodexCategory::Character, TEXT("李白"), TEXT("诗仙，斗酒诗百篇。"), 5},
        {TEXT("char_yuefei"), ECodexCategory::Character, TEXT("岳飞"), TEXT("精忠报国，民族英雄。"), 5},
        {TEXT("loc_greatwall"), ECodexCategory::Location, TEXT("长城"), TEXT("万里长城，中华民族的象征。"), 5},
        {TEXT("loc_westlake"), ECodexCategory::Location, TEXT("西湖"), TEXT("欲把西湖比西子，淡妆浓抹总相宜。"), 3},
        {TEXT("martial_taiji"), ECodexCategory::MartialArt, TEXT("太极拳"), TEXT("以柔克刚，四两拨千斤。"), 4},
        {TEXT("beast_qilin"), ECodexCategory::Beast, TEXT("麒麟"), TEXT("祥瑞之兽，圣人出则麒麟现。"), 5},
        {TEXT("beast_dragon"), ECodexCategory::Beast, TEXT("龙"), TEXT("中华民族的图腾，腾云驾雾。"), 5},
        {TEXT("plant_ginseng"), ECodexCategory::Plant, TEXT("千年人参"), TEXT("千年老参，起死回生。"), 4},
        {TEXT("history_unification"), ECodexCategory::History, TEXT("大一统"), TEXT("天下大势，分久必合。"), 5},
    };

    for (const auto& E : Entries)
    {
        FCodexEntry Entry;
        Entry.EntryID = E.ID; Entry.Category = E.Cat;
        Entry.Name = FText::FromString(UTF8_TO_TCHAR(E.Name));
        Entry.Description = FText::FromString(UTF8_TO_TCHAR(E.Desc));
        Entry.Rarity = E.Rarity;
        CodexDatabase.Add(E.ID, Entry);
    }

    TotalEntries = CodexDatabase.Num();
    UE_LOG(LogShanHe, Log, TEXT("图鉴系统初始化 - %d 条图鉴"), TotalEntries);
}

bool UCodexSystem::UnlockEntry(FName EntryID)
{
    if (FCodexEntry* Entry = CodexDatabase.Find(EntryID))
    {
        if (!Entry->bDiscovered)
        {
            Entry->bDiscovered = true;
            Entry->DiscoveredDate = FDateTime::Now();
            DiscoveredEntries++;
            OnEntryUnlocked.Broadcast(*Entry);
            UE_LOG(LogShanHe, Log, TEXT("解锁图鉴：%s (%d/%d)"), *EntryID.ToString(), DiscoveredEntries, TotalEntries);
            return true;
        }
    }
    return false;
}

TArray<FCodexEntry> UCodexSystem::GetEntriesByCategory(ECodexCategory Category) const
{
    TArray<FCodexEntry> Result;
    for (const auto& Pair : CodexDatabase)
    {
        if (Pair.Value.Category == Category)
            Result.Add(Pair.Value);
    }
    return Result;
}

int32 UCodexSystem::GetDiscoveryProgress(ECodexCategory Category) const
{
    int32 Total = 0, Discovered = 0;
    for (const auto& Pair : CodexDatabase)
    {
        if (Pair.Value.Category == Category)
        {
            Total++;
            if (Pair.Value.bDiscovered) Discovered++;
        }
    }
    return Total > 0 ? (Discovered * 100 / Total) : 0;
}

int32 UCodexSystem::GetTotalProgress() const
{
    return TotalEntries > 0 ? (DiscoveredEntries * 100 / TotalEntries) : 0;
}

bool UCodexSystem::CheckCollectionReward()
{
    int32 Progress = GetTotalProgress();
    if (Progress >= 100)
    {
        UE_LOG(LogShanHe, Log, TEXT("图鉴全收集！获得终极奖励"));
        return true;
    }
    if (Progress >= 50)
    {
        UE_LOG(LogShanHe, Log, TEXT("图鉴收集50%%！获得稀有奖励"));
        return true;
    }
    return false;
}
