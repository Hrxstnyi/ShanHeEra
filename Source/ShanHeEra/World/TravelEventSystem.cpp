#include "World/TravelEventSystem.h"
#include "Core/ShanHeLog.h"

void UTravelEventSystem::InitializeEvents()
{
    struct FEventDef { FName ID; ETravelEventType Type; const TCHAR* Title; const TCHAR* Desc; float Weight; int32 MinLv; };
    FEventDef Events[] = {
        {TEXT("bandit_road"), ETravelEventType::Bandit, TEXT("路遇强盗"), TEXT("三个蒙面大汉拦住去路，刀光闪闪。"), 2.0f, 1},
        {TEXT("bandit_elite"), ETravelEventType::Bandit, TEXT("悍匪伏击"), TEXT("一队装备精良的强盗从林中杀出！"), 0.5f, 10},
        {TEXT("adventure_cave"), ETravelEventType::Adventure, TEXT("神秘山洞"), TEXT("路边发现一个隐蔽的山洞，深处似有微光。"), 0.8f, 3},
        {TEXT("adventure_oldman"), ETravelEventType::Adventure, TEXT("古怪老人"), TEXT("一个衣衫褴褛的老人坐在路边，盯着你笑。"), 0.6f, 5},
        {TEXT("merchant_caravan"), ETravelEventType::Merchant, TEXT("商队歇息"), TEXT("一支商队在路边休息，货物琳琅满目。"), 1.5f, 1},
        {TEXT("merchant_rare"), ETravelEventType::Merchant, TEXT("奇货可居"), TEXT("一个神秘商人兜售着罕见的物品。"), 0.5f, 8},
        {TEXT("refugee_family"), ETravelEventType::Refugee, TEXT("逃难一家"), TEXT("一对夫妇带着孩子跪在路边，乞求施舍。"), 1.2f, 1},
        {TEXT("refugee_army"), ETravelEventType::Refugee, TEXT("流民潮"), TEXT("大批流民涌来，秩序混乱。"), 0.8f, 5},
        {TEXT("swordsman_duel"), ETravelEventType::Swordsman, TEXT("侠客邀战"), TEXT("一位佩剑侠客拦住你，要求切磋武艺。"), 0.7f, 5},
        {TEXT("swordsman_help"), ETravelEventType::Swordsman, TEXT("侠客有难"), TEXT("一位侠客被仇家围攻，是否出手相助？"), 0.6f, 3},
        {TEXT("immortal_point"), ETravelEventType::Immortal, TEXT("仙人指路"), TEXT("一位仙风道骨的长者微笑着看着你。"), 0.2f, 10},
        {TEXT("treasure_chest"), ETravelEventType::Treasure, TEXT("发现宝箱"), TEXT("草丛中露出一个古老的箱子。"), 0.4f, 1},
        {TEXT("lost_mist"), ETravelEventType::Lost, TEXT("迷雾迷路"), TEXT("突然起了大雾，你迷失了方向。"), 1.0f, 1},
        {TEXT("weather_storm"), ETravelEventType::Weather, TEXT("突降暴雨"), TEXT("天空骤变，暴雨倾盆而下。"), 1.0f, 1},
    };

    for (const auto& E : Events)
    {
        FTravelEvent Ev;
        Ev.EventID = E.ID; Ev.Type = E.Type;
        Ev.Title = FText::FromString(UTF8_TO_TCHAR(E.Title));
        Ev.Description = FText::FromString(UTF8_TO_TCHAR(E.Desc));
        Ev.Weight = E.Weight; Ev.MinPlayerLevel = E.MinLv;
        EventDatabase.Add(E.ID, Ev);
    }

    UE_LOG(LogShanHe, Log, TEXT("旅行事件系统初始化 - %d 种随机事件"), EventDatabase.Num());
}

FTravelEvent UTravelEventSystem::RollRandomEvent(int32 PlayerLevel, FName CurrentRegion)
{
    TArray<FTravelEvent> ValidEvents;
    float TotalWeight = 0.0f;
    for (const auto& Pair : EventDatabase)
    {
        if (Pair.Value.MinPlayerLevel <= PlayerLevel)
        {
            ValidEvents.Add(Pair.Value);
            TotalWeight += Pair.Value.Weight;
        }
    }

    if (ValidEvents.Num() == 0) return FTravelEvent();

    float Roll = FMath::FRand() * TotalWeight;
    float Accum = 0.0f;
    for (const auto& Ev : ValidEvents)
    {
        Accum += Ev.Weight;
        if (Roll <= Accum)
        {
            OnEventTriggered.Broadcast(Ev);
            UE_LOG(LogShanHe, Log, TEXT("触发旅行事件：%s"), *Ev.EventID.ToString());
            return Ev;
        }
    }
    return ValidEvents[0];
}

TArray<FTravelEventChoice> UTravelEventSystem::GetEventChoices(FName EventID)
{
    if (const TArray<FTravelEventChoice>* Choices = ChoiceDatabase.Find(EventID))
        return *Choices;
    return TArray<FTravelEventChoice>();
}

void UTravelEventSystem::ResolveChoice(FName EventID, int32 ChoiceIndex)
{
    UE_LOG(LogShanHe, Log, TEXT("事件 %s 选择 %d"), *EventID.ToString(), ChoiceIndex);
}

float UTravelEventSystem::GetTravelEventChance() const
{
    return BaseEventChance;
}
