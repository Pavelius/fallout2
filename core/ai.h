enum tags
{
	Empthy,
	// Abilities
	Strenght, Perception, Endurance, Charisma, Intellegence, Agility, Luck,
	FirstStat = Strenght, LastStat = Luck,
	// Perks
	Awareness, BonusHtHAttacks, BonusHtHDamage, BonusMove, BonusRangedDamage, BonusROF,
	EarlierSequence, FasterHealing, MoreCriticals, NightVision, Presence, RadResistance,
	Toughness, StrongBack, SharpShooter, SilenRunnng, Survivalist, MasterTrader, Educated,
	Healer, FortuneFinder, BetterCriticals, Empathy, Slayer, Sniper, SilentDeath,
	ActionBoy, MentalBlock, Lifegiver, Dodger, SnakeEater, MrFixit, Medic, MasterTheif,
	Speaker, HeaveHo, FriendlyFoe, PickPocket, Ghost, CultOfPersonality, Scrounger,
	Explorer, FlowerChild, Pathfinder, AnimalFriend, Scout, MysteriousStranger,
	Ranger, QuickPockets, SmoothTalker, SwiftLearner, Tag, Mutate,
	AddictedNukeCola, AddictedBuffout, AddictedMentat, AddictedPsicho, AddictedRadaway,
	WeaponLongRange, WeaponAccuracy, WeaponPenetrating, WeaponKnockback,
	ArmourPowerPerk, ArmourCombatPerk,
	WeaponScopeRange, WeaponFastReload, WeaponNightSight, WeaponFlameboy,
	ArmourAdvance, ArmourAdvanceII,
	AddictedJet, AddictedGambling,
	ArmourCharisma,
	GeckoSkinning,
	DermalImpactArmour, DermalImpactAssaultEnchantments,
	PhoenixArmourImplants, PhoenixAssaultEnchantments, VaultCityInoculation,
	AdrenalineRush, CautiousNature, Comprehension, DemolitionExpert, Gambler,
	GainStrenght, GainPerception, GainEndurance, GainCharisma, GainIntellegence, GainAgility, GainLuck,
	Harmless, HereAndNow, HtHEvade, KamaSutraMaster, KarmaBeacon, LightStep, LivingAnatomy,
	MagneticPersonality, Negotiator, PackRat, Pyromaniac, QuickRecovery, Salesman,
	Stonewall, Thief, WeaponHandling, VaultCityTraining,
	AlcoholRaisedHP, AlcoholRaisedHP2, AlcoholLoweredHP, AlcoholLoweredHP2,
	AutodocRaisedHP, AutodocRaisedHP2, AutodocLoweredHP, AutodocLoweredHP2,
	ExpertExcrementExpeditor,
	WeaponEnchencedKnockout, MegaUnlucky,
	FirstPerk = Awareness, LastPerk = MegaUnlucky,
	// Skills
	SkillSmallGuns, SkillBigGuns, SkillEnergyWeapon,
	SkillUnarmed, SkillMeleeWeapon, SkillThrowing,
	SkillFirstAid, SkillDoctor,
	SkillSneak, SkillLockpick, SkillSteal, SkillTraps,
	SkillScience, SkillRepair,
	SkillSpeech, SkillBarter, SkillGambling,
	SkillOutdoorsman,
	FirstSkill = SkillSmallGuns, LastSkill = SkillOutdoorsman,
	// Impotant
	Race, Age, SkillPoints, Level, Experience, Reputation, Karma,
	// Health body status
	HP, PoisonLevel, RadiationLevel,
	WoundEye, WoundRightHand, WoundLeftHand, WoundRightLeg, WoundLeftLeg,
	// Traits
	FastMetabolism, Bruiser, SmallFrame, OneHanded, Finesse, Kamikaze, HeavyHanded, FastShoot,
	BloodyMess, Jinxed, GoodNatured, ChemReliant, ChemResistant, SexAppeal, Skilled, Gifted,
	FirstTraits = FastMetabolism, LastTraits = Gifted,
	// Additional abilities
	AC, MaxAP, CarryWeight, MeleeDamage, Initiative, HealthLevel,
	CriticalHitChance, CriticalHitMult,
	RadiationResistance, PoisonResistance,
	MaxHP, Gender,
	// Damage by types
	DamageNormal, DamageLaser, DamageFire, DamagePlasma, DamageElectrical, DamageEMP, DamageExplosive,
	DamageResistanceNormal, DamageResistanceLaser, DamageResistanceFire, DamageResistancePlasma, DamageResistanceElectrical, DamageResistanceEMP, DamageResistanceExplosive,
	// Command
	Intro, New, Load, Options, Credits, Exit, Inventory,
	Help, Next, Cancel,
	Change, Create,
	// Genders
	Male, Female, Mutant,
	// Stat string
	StatAwful, StatVeryBad, StatBad, StatNoSoBad, StatNormal, StatGood, StatVeryGood, StatExeptional,
	FirstStatStr = StatAwful, LastStatStr = FirstStatStr+10,
	// Properties
	Name, ShortName, VeryShortName, Description, Type, Formula, ItemCount, Count, LastValid,
	EquipmentWeight,
	Damage, Range, AmmoCapacity,
	Destination,
	Year, Month, Day, Hour, Minute, Round, CurrentRound,
	Traits, Stats, Tags, Skills,
	TraistLeft, StatsLeft, TagsLeft, SkillsLeft,
	// Attack modes (interface 40-45)
	AttackNo, AttackPunch, AttackKick, AttackSwing, AttackThrust, AttackThrow, AttackFireSingle, AttackFireBurst, AttackFlame,
	FirstAttackType = AttackNo, LastAttackType = AttackFlame,
	// Item type
	Armour, Container, Drug, Weapon, Ammo, Misc, Key,
	// Weapon types
	WeaponTypeDagger, WeaponTypeBatton, WeaponTypeHammer,WeaponTypeSpear,
	WeaponTypePistol, WeaponTypeAutomatic, WeaponTypeRifle,
	WeaponTypeBigGun, WeaponTypeMachineGun, WeaponTypeRocket,
	FirstWeaponType = WeaponTypeDagger, LastWeaponType = WeaponTypeRocket,
	// Materials
	Glass, Metal, Plastic, Wood, Dirt, Stone, Cement, Leather,
	// Animation
	Frame, Action, Animation, Direction,
	Position, PositionX, PositionY, Order,
	// Premades
	NewCharacter, PremadeCombat, PremadeStealth, PremadeDiplomat,
	FirstPremade = NewCharacter, LastPremade = PremadeDiplomat,
	// Units (some premade)
	Player = 0x1000,
	FirstCreature = Player, LastCreature = 0x1F00,
	// Items
	ArmourMetal = 0x2000, Pistol10mmAuto, ArmourLeather, Pistol10mm,
	WarHammer, MachineGun, RoketLauncher, AmmoRocket, Batton, FlameThrower,
	Rifle, Knife, Spear, ArmourPower, RiflePlasma, PistolLaser,
	ArmourCombat, PistolMagnum, StoneSmall, Crowbar, HandWeapon,
	Pistol14mm, RifleCombatStorm, PistolPlasma, Granade, GranadePlasma, GranadeEMP,
	GutlingLaser,
	Ammo10mmDM, Ammo10mmAP, Ammo44inAP, AmmoFuel, Ammo14mmAP, Ammo233inFM, Ammo5mmDM, Ammo5mmAP,
	AmmoRocketAP,
	AmmoEnergyCell, AmmoAtomicCell,
	Stimpack, Money,
	ThrowingKnife, KitFirstAid, RadAway, Antidote, ReservedItem, ExplosivesDinamite,
	GeygerCounter, Mentat, StealthBoy, WaterCheap, DogsMark, SpyGadget, HolodiscStuff,
	MovementDetector, Fruit, AttacheCase, BookScience, ArmourLeatherJacket, 
	Tools, Book, KitTheifElectrical, Painting, FlashStick, BookDoctor,
	FoodIguana, KeySimple, KeyRing, KitTheif, ExplosivesPlastic, BookScout,
	Buffout, Clock, Motor, KitDoctor, ScorpionTail, RifleShootgun, AmmoRifle,
	CardRedAccess, CardBlueAccess, Trash, GoldMedalion, Radio, CigarLighter,
	BookGunsAndWeapons, FoodIguanaOnStick, Recorder, KeyAdvanced, NukeCola,
	RadX, Psycho, Ammo44inDM, Urn, Robe, HandTangler, HammerVibro, KnifeVibro, Flower,
	RifleLaser, Jewelry, PistolAlienBlaster, Ammo9mm, PistolMauser9mm,
	PsychoNeutralizer, Beer, Buzz, Flask, Rope, CardAccess,
	FirstItems = ArmourMetal, LastItems = Buzz,
};
enum actions {
	ActionStand, ActionWalk, ActionClimb, ActionPickup, ActionUse, ActionDodge,
	ActionDamaged, ActionDamagedRear,
	ActionUnarmed1, ActionUnarmed2, ActionThrown, ActionRun, ActionHandsUp, ActionCloackOff,
	ActionKnockOutBack, ActionKnockOutForward,
};
struct damagei
{
	tags			type;
	short unsigned	min;
	short unsigned	max;
};
struct gears
{
	unsigned		body;
	unsigned		hands[2];
};
namespace items
{
	extern damagei	damage;
	void			add(unsigned& i1, unsigned& i2);
	unsigned		create(tags type, int count = 1, int param = 0);
	int				get(unsigned mid, int id);
	const char*		gets(unsigned mid, int id);
	unsigned		set(unsigned mid, int id, int value);
	unsigned		pick(unsigned& i1);
	inline tags		type(unsigned mid) { return tags(mid&0xFFFF); }
}
namespace perks
{
	int				get(int mid, int id);
	const char*		gets(int mid, int id);
}
namespace premade
{
	int				get(int mid, int id);
	const char*		gets(int mid, int id);
	tags			tag(int mid, int pos);
	tags			traits(int mid, int pos);
}
namespace creatures
{
	void			animate();
	bool			add(int mid, unsigned item, int id = -1, int pos = -1, bool run = true);
	void			animate();
	void			clear();
	void			check(int skill);
	void			create(int mid, int pid);
	void			damage(int mid, int weapon = 0);
	bool			isallow(int mid, int id);
	bool			istagged(int mid, int id);
	int				invertory(unsigned** reuslt, int count, int mid);
	unsigned*		gears(int mid, int id, int pos = 0);
	int				get(int id);
	int				get(int mid, int id);
	int				getr(int mid, int id);
	const char*		gets(int mid, int id);
	bool			set(int mid, int id, int value);
	void			setr(int mid, int id, int value);
	bool			set(int mid, int id, const char* value);
	void			settag(int mid, int id, int value);
}
namespace units
{
	int				add();
	int				find(int index);
	int				get(int mid, int id);
	const char*		gets(int mid, int id);
}
namespace world
{
	void			clear();
	int				day(unsigned round);
	int				year(unsigned round);
	int				month(unsigned round);
	int				hour(unsigned round);
	int				minute(unsigned round);
	void			msg(const char* text);
	int				get(int id);
	unsigned		round(int year, int month, int day);
	bool			set(int id, int value);
}
enum directions
{
	Left, LeftUp, Up, RightUp, Right, RightDown, Down, LeftDown, Center,
};
namespace map
{
	enum tokens
	{
		Waste, Dirt, Road, Causeway,
	};
	const int		tilew = 80;
	const int		tileh = 36;
	const int		gridw = 32;
	const int		gridh = 16;
	void			create();
	extern int		cols;
	directions		direction(int from, int to);
	int				d2o(directions d);
	unsigned short	floor(int index);
	int				gx(int tile);
	int				gy(int tile);
	short unsigned	lookup(int index, map::tokens tile);
	int				move(int index, int orientation);
	int				h2x(int ht);
	int				h2y(int ht);
	int				pos(int tx, int ty);
	int				pos2(int tx, int ty);
	extern int		rows;
	unsigned short	scene(int index);
	void			sfloor(int index, short unsigned fid);
	void			sscene(int i, short unsigned fid);
	int				t2x(int tile);
	int				t2y(int tile);
	int				xy2t(int x, int y);
	int				xy2h(int x, int y);
	unsigned short	wall(int index, int n = 0);
	tokens			group(short unsigned tile);
	namespace generate
	{
		void		highway(int i1, int count, int width);
	}
}
namespace game
{
	int				d100();
}