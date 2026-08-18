#include "Culture/CultureSystem.h"
#include "Core/ShanHeLog.h"

void UCultureSystem::InitializeCulture()
{
    FFestivalData Spring;
    Spring.FestivalID = TEXT("SpringFestival");
    Spring.Name = NSLOCTEXT("Festival","Spring","春节");
    Spring.Month = 1; Spring.Day = 1;
    Spring.Description = NSLOCTEXT("Festival","SpringDesc","一岁之首，万象更新");
    Spring.Customs = NSLOCTEXT("Festival","SpringCust","贴春联、放爆竹、拜年、发压岁钱、吃团圆饭");
    Festivals.Add(Spring);

    FFestivalData DragonBoat;
    DragonBoat.FestivalID = TEXT("DragonBoat");
    DragonBoat.Name = NSLOCTEXT("Festival","DragonBoat","端午节");
    DragonBoat.Month = 5; DragonBoat.Day = 5;
    DragonBoat.Description = NSLOCTEXT("Festival","DragonBoatDesc","纪念屈原，家国情怀");
    DragonBoat.Customs = NSLOCTEXT("Festival","DragonBoatCust","赛龙舟、吃粽子、挂艾草、佩香囊");
    Festivals.Add(DragonBoat);

    FFestivalData MidAutumn;
    MidAutumn.FestivalID = TEXT("MidAutumn");
    MidAutumn.Name = NSLOCTEXT("Festival","MidAutumn","中秋节");
    MidAutumn.Month = 8; MidAutumn.Day = 15;
    MidAutumn.Description = NSLOCTEXT("Festival","MidAutumnDesc","月圆人团圆，海峡两岸共此时");
    MidAutumn.Customs = NSLOCTEXT("Festival","MidAutumnCust","赏月、吃月饼、团圆、思念远方亲人");
    Festivals.Add(MidAutumn);

    UE_LOG(LogShanHe, Log, TEXT("文化系统初始化 - 节日、戏曲、民俗、医药、饮食、手艺、地方传统"));
}
