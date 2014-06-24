#include "f2lib.h"
#include "ai.h"

static struct object_premade
{
	short			level;
	const char*		name[2];
	short			age;
	tags			gender;
	unsigned char	stats[LastStat-FirstStat+1];
	tags			traits[2];
	tags			tagged[4];
	tags			perks[8];
	struct gears	gears;
	const char*		text[2];
} objects[] =
{
	{1, {""}, 25, Male, {5, 5, 5, 5, 5, 5, 5}}, // empthy character
	{1, {"����"}, 21, Male, {8, 5, 9, 3, 4, 7, 4}, {HeavyHanded, Gifted}, {SkillSmallGuns, SkillMeleeWeapon, SkillThrowing}, {}, {}, {"�������������� ���� � ������������ ��������� ����� ����� ����� �� ������ ��������� �������. �� ������ ���� ������� ����, � ����� ��� �������, ��� �� ����� ��������. ��������� ���������� ���� ���� ������� ������������ �� ���� ����� ����������. �� ����� ����� ��������, ��� ������� ����� ������ ������� � ������ �� �������� ����� � ���� �� ����."}}, // combat
	{1, {"������"}, 32, Male, {5, 8, 4, 4, 5, 9, 5}, {SmallFrame, Skilled}, {SkillSneak, SkillLockpick, SkillSteal}, {}, {}, {"������ ������� � ������� ������������ ������� �������� ������� ������������� ���� �����������. ��������� ��� ����� ���� ������, ��� ����� ������� ����� ��������� ���� ������� �� �����-������ ������ �������. � ��� ��� ������ ���� �����������. ������ ������� �������� ������ ��� �� ������������ ��� ���� ��������� ���� ������������ � ������� ����."}}, // stealth
	{1, {"�����"}, 19, Female, {4, 5, 4, 10, 7, 6, 4}, {OneHanded, SexAppeal}, {SkillSpeech, SkillBarter, SkillFirstAid}, {}, {}, {"����� ����� �������� � ��������������. ������������� ������� ������ ������� � ���� ����������. ��� ������� �����, ��� ���-���� � �������, � �� ���������� ����� �� ���������� �������. ������ �� ������ ����� ������, � ����� ������ ��� ������� ������"}}, // diplomat
	{5, {"������"}, 40, Male, {9, 5, 10, 2, 2, 5, 1}, {HeavyHanded}, {SkillBigGuns, SkillUnarmed, SkillMeleeWeapon}, {}, {MachineGun}},
	{2, {"������"}, 30, Male, {6, 5, 5, 4, 4, 6, 4}, {}, {SkillSmallGuns, SkillGambling, SkillSteal}},
	{1, {"�����"}, 65, Male, {3, 2, 1, 3, 3, 3, 1}, {}, {SkillSpeech, SkillBarter, SkillGambling}},
};

int premade::get(int mid, int id)
{
	switch(id)
	{
	case Strenght:
	case Perception:
	case Endurance:
	case Charisma:
	case Intellegence:
	case Agility:
	case Luck:
		return objects[mid-FirstPremade].stats[id-FirstStat];
	case Age:
		return objects[mid-FirstPremade].age;
	case Gender:
		return objects[mid-FirstPremade].gender;
	default:
		return 0;
	}
}

tags premade::tag(int mid, int pos)
{
	return objects[mid-FirstPremade].tagged[pos];
}

tags premade::traits(int mid, int pos)
{
	return objects[mid-FirstPremade].traits[pos];
}

const char* premade::gets(int mid, int id)
{
	switch(id)
	{
	case Name:
		return objects[mid-FirstPremade].name[locale];
	case Description:
		return objects[mid-FirstPremade].text[locale];
	default:
		return "premade";
	}
}