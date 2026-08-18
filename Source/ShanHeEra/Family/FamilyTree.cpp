#include "Family/FamilyTree.h"
#include "Core/ShanHeLog.h"

FGuid UFamilyTree::AddMember(const FPersonName& Name, int32 Generation, FGuid FatherID, FGuid MotherID, bool bIsMale)
{
    FFamilyMember Member;
    Member.MemberID = FGuid::NewGuid();
    Member.Name = Name;
    Member.Generation = Generation;
    Member.FatherID = FatherID;
    Member.MotherID = MotherID;
    Member.bIsMale = bIsMale;
    Member.bIsAlive = true;
    Members.Add(Member);

    // 更新父母的子女列表
    for (FFamilyMember& M : Members)
    {
        if (M.MemberID == FatherID || M.MemberID == MotherID)
            M.ChildrenIDs.Add(Member.MemberID);
    }

    UE_LOG(LogShanHe, Log, TEXT("家族新成员: %s，第%d代"), *(Name.Surname + Name.GivenName), Generation);
    return Member.MemberID;
}

void UFamilyTree::RemoveMember(FGuid MemberID)
{
    Members.RemoveAll([&](const FFamilyMember& M) { return M.MemberID == MemberID; });
}

FFamilyMember UFamilyTree::GetMember(FGuid MemberID) const
{
    for (const FFamilyMember& M : Members)
        if (M.MemberID == MemberID) return M;
    return FFamilyMember();
}

TArray<FFamilyMember> UFamilyTree::GetChildren(FGuid ParentID) const
{
    TArray<FFamilyMember> Result;
    if (const FFamilyMember* Parent = Members.FindByPredicate([&](const FFamilyMember& M) { return M.MemberID == ParentID; }))
    {
        for (FGuid ChildID : Parent->ChildrenIDs)
            if (const FFamilyMember* Child = Members.FindByPredicate([&](const FFamilyMember& M) { return M.MemberID == ChildID; }))
                Result.Add(*Child);
    }
    return Result;
}

TArray<FFamilyMember> UFamilyTree::GetGeneration(int32 Generation) const
{
    TArray<FFamilyMember> Result;
    for (const FFamilyMember& M : Members)
        if (M.Generation == Generation) Result.Add(M);
    return Result;
}

int32 UFamilyTree::GetAliveCount() const
{
    int32 Count = 0;
    for (const FFamilyMember& M : Members)
        if (M.bIsAlive) Count++;
    return Count;
}

int32 UFamilyTree::GetHighestGeneration() const
{
    int32 MaxGen = 0;
    for (const FFamilyMember& M : Members)
        MaxGen = FMath::Max(MaxGen, M.Generation);
    return MaxGen;
}
