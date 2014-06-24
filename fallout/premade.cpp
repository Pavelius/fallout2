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
	{1, {"Нарг"}, 21, Male, {8, 5, 9, 3, 4, 7, 4}, {HeavyHanded, Gifted}, {SkillSmallGuns, SkillMeleeWeapon, SkillThrowing}, {}, {}, {"Исключительная сила и телосложение позволили Наргу стать одним из лучших охотников племени. Но обычно Нарг сначала бьет, а потом уже смотрит, что он такое встретил. Постоянно тренируясь Нарг стал большим специалистом по всем видам разрушения. Он очень хочет доказать, что достоин стать вождем племени и ничему не позволит стать у себя на пути."}}, // combat
	{1, {"Минган"}, 32, Male, {5, 8, 4, 4, 5, 9, 5}, {SmallFrame, Skilled}, {SkillSneak, SkillLockpick, SkillSteal}, {}, {}, {"Мягкая поступь и быстрый хватательный рефлекс помогают Мингану удовлетворять свои потребности. Несколько лет назад люди решили, что этому воришке лучше применять свои таланты на каком-нибудь другом племени. С тех пор Минган стал разведчиком. Однако палатки соседних племен уже не представляют для него интереса… Пора отправляться в дальний путь."}}, // stealth
	{1, {"Читса"}, 19, Female, {4, 5, 4, 10, 7, 6, 4}, {OneHanded, SexAppeal}, {SkillSpeech, SkillBarter, SkillFirstAid}, {}, {}, {"Читса умеет убеждать и договариваться. Ослепительная красота служит девушке в этом подспорьем. Она торгует лучше, чем кто-либо в племени, и не понаслышке знает об опасностях пустыни. Сейчас ее народу нужна помощь, и Читса слышит зов дальней дороги…"}}, // diplomat
	{5, {"Мутант"}, 40, Male, {9, 5, 10, 2, 2, 5, 1}, {HeavyHanded}, {SkillBigGuns, SkillUnarmed, SkillMeleeWeapon}, {}, {MachineGun}},
	{2, {"Рейдер"}, 30, Male, {6, 5, 5, 4, 4, 6, 4}, {}, {SkillSmallGuns, SkillGambling, SkillSteal}},
	{1, {"Нищий"}, 65, Male, {3, 2, 1, 3, 3, 3, 1}, {}, {SkillSpeech, SkillBarter, SkillGambling}},
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