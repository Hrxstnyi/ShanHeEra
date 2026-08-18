#include "UI/AttributePanelWidget.h"
#include "Core/ShanHeLog.h"

void UAttributePanelWidget::NativeConstruct()
{
    Super::NativeConstruct();
    RefreshAttributes();
}

void UAttributePanelWidget::RefreshAttributes()
{
    // 实际项目中从AttributeSystem获取数据
    UE_LOG(LogShanHe, Log, TEXT("属性面板刷新 - 六维属性+状态条+技能"));
}
