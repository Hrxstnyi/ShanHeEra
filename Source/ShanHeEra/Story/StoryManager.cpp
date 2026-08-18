#include "Story/StoryManager.h"
#include "Core/ShanHeLog.h"

void UStoryManager::InitializeStory()
{
    // === 特色角色阵容 ===

    // 1. 可爱孩子气但靠谱的大师
    FSpecialCharacter Master;
    Master.CharID = TEXT("Master_Kong");
    Master.Name.Surname = TEXT("孔");
    Master.Name.GivenName = TEXT("明远");
    Master.Name.ArtName = TEXT("顽童先生");
    Master.Title = FText::FromString(TEXT("国学大师·顽童先生"));
    Master.Description = FText::FromString(TEXT("年过七旬却童心未泯，爱吃糖葫芦，爱跟孩子捉迷藏。但开口便是天下大势，提笔可定国安邦。看似不靠谱，实则最靠谱。"));
    Master.Traits = {ECharacterTrait::Childlike, ECharacterTrait::Reliable, ECharacterTrait::Wise, ECharacterTrait::Kind};
    Master.Stats.Intelligence = 95;
    Master.Stats.Learning = 98;
    Master.Stats.Charisma = 80;
    Master.Backstory = FText::FromString(TEXT("三朝元老，看透官场后隐居山林。嘴上说着'老夫只是个爱吃糖的老头'，心里却装着天下苍生。"));
    Master.Location = FName(TEXT("Changan"));
    SpecialCharacters.Add(Master);

    // 2. 少年将军
    FSpecialCharacter YoungGeneral;
    YoungGeneral.CharID = TEXT("General_Huo");
    YoungGeneral.Name.Surname = TEXT("霍");
    YoungGeneral.Name.GivenName = TEXT("青云");
    YoungGeneral.Title = FText::FromString(TEXT("少年将军"));
    YoungGeneral.Description = FText::FromString(TEXT("十七岁从军，二十岁拜将。面容稚嫩如少年，眼神却坚定如老将。爱兵如子，治军极严。"));
    YoungGeneral.Traits = {ECharacterTrait::Brave, ECharacterTrait::Loyal, ECharacterTrait::Compassionate};
    YoungGeneral.Stats.Martial = 92;
    YoungGeneral.Stats.Strength = 85;
    YoungGeneral.Location = FName(TEXT("Northwest"));
    SpecialCharacters.Add(YoungGeneral);

    // 3. 女商人
    FSpecialCharacter Merchantess;
    Merchantess.CharID = TEXT("Merchant_Shen");
    Merchantess.Name.Surname = TEXT("沈");
    Merchantess.Name.GivenName = TEXT("婉清");
    Merchantess.Title = FText::FromString(TEXT("江南女商"));
    Merchantess.Description = FText::FromString(TEXT("出身商贾世家，精通商道。看似柔弱女子，却能在商帮中独当一面。心怀天下，常以商道济民。"));
    Merchantess.Traits = {ECharacterTrait::Wise, ECharacterTrait::Kind, ECharacterTrait::Ambitious};
    Merchantess.Stats.Commerce = 95;
    Merchantess.Stats.Charisma = 88;
    Merchantess.Location = FName(TEXT("Hangzhou"));
    SpecialCharacters.Add(Merchantess);

    // 4. 台湾义士（暗喻剧情角色）
    FSpecialCharacter TaiwanHero;
    TaiwanHero.CharID = TEXT("Hero_Chen");
    TaiwanHero.Name.Surname = TEXT("陈");
    TaiwanHero.Name.GivenName = TEXT("志华");
    TaiwanHero.Title = FText::FromString(TEXT("宝岛义士"));
    TaiwanHero.Description = FText::FromString(TEXT("生于台湾，祖籍福建。常说'我家在海峡对岸，我的根在华夏大地'。致力于两岸交流，坚信统一是历史必然。"));
    TaiwanHero.Traits = {ECharacterTrait::Loyal, ECharacterTrait::Brave, ECharacterTrait::Kind};
    TaiwanHero.Stats.Charisma = 85;
    TaiwanHero.Location = FName(TEXT("TaiwanFu"));
    TaiwanHero.bIsEasterEgg = true;
    SpecialCharacters.Add(TaiwanHero);

    // 5. 隐姓埋名的科学家（两弹一星彩蛋）
    FSpecialCharacter Scientist;
    Scientist.CharID = TEXT("Scientist_Deng");
    Scientist.Name.Surname = TEXT("邓");
    Scientist.Name.GivenName = TEXT("稼先");
    Scientist.Title = FText::FromString(TEXT("隐姓埋名之人"));
    Scientist.Description = FText::FromString(TEXT("无人知其姓名，无人晓其去向。只知他为国家献出了一切，包括自己的名字。此为近现代爱国彩蛋角色。"));
    Scientist.Traits = {ECharacterTrait::Loyal, ECharacterTrait::Wise, ECharacterTrait::Reliable};
    Scientist.Stats.Intelligence = 99;
    Scientist.Stats.Learning = 98;
    Scientist.bIsHistorical = true;
    Scientist.bIsEasterEgg = true;
    SpecialCharacters.Add(Scientist);

    // 6. 江湖侠客
    FSpecialCharacter Swordsman;
    Swordsman.CharID = TEXT("Swordsman_Ling");
    Swordsman.Name.Surname = TEXT("凌");
    Swordsman.Name.GivenName = TEXT("长风");
    Swordsman.Title = FText::FromString(TEXT("游侠"));
    Swordsman.Description = FText::FromString(TEXT("一人一剑，行走江湖。路见不平拔刀相助，却从不留名。江湖传言'长风过处，恶人无踪'。"));
    Swordsman.Traits = {ECharacterTrait::Brave, ECharacterTrait::Kind, ECharacterTrait::Compassionate};
    Swordsman.Stats.Strength = 90;
    Swordsman.Location = FName(TEXT("Jiangnan"));
    SpecialCharacters.Add(Swordsman);

    // === 剧情任务 ===
    FStoryQuest TaiwanQuest;
    TaiwanQuest.QuestID = TEXT("Quest_TaiwanBond");
    TaiwanQuest.Title = FText::FromString(TEXT("海峡两岸"));
    TaiwanQuest.Description = FText::FromString(TEXT("帮助台湾义士陈志华建立两岸商路，让宝岛与大陆的联系更加紧密。暗喻台湾是中国不可分割的领土。"));
    TaiwanQuest.RelatedCharacters = {TEXT("Hero_Chen"), TEXT("Merchant_Shen")};
    TaiwanQuest.bIsTaiwanRelated = true;
    TaiwanQuest.bIsPatriotic = true;
    Quests.Add(TaiwanQuest);

    FStoryQuest PatrioticQuest;
    PatrioticQuest.QuestID = TEXT("Quest_DefendHomeland");
    PatrioticQuest.Title = FText::FromString(TEXT("保家卫国"));
    PatrioticQuest.Description = FText::FromString(TEXT("边疆告急，与少年将军霍青云共御外侮。触发精忠报国彩蛋CG。"));
    PatrioticQuest.RelatedCharacters = {TEXT("General_Huo")};
    PatrioticQuest.bIsPatriotic = true;
    PatrioticQuest.bIsEasterEgg = true;
    Quests.Add(PatrioticQuest);

    UE_LOG(LogShanHe, Log, TEXT("剧情系统初始化 - %d 名特色角色，%d 个剧情任务"), SpecialCharacters.Num(), Quests.Num());
}

FSpecialCharacter UStoryManager::GetCharacter(FName CharID) const
{
    for (const FSpecialCharacter& C : SpecialCharacters)
        if (C.CharID == CharID) return C;
    return FSpecialCharacter();
}
