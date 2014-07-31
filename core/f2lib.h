//   Copyright 2013 by Pavel Chistyakov
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

enum tags {
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
	WeaponTypeDagger, WeaponTypeBatton, WeaponTypeHammer, WeaponTypeSpear,
	WeaponTypePistol, WeaponTypeAutomatic, WeaponTypeRifle,
	WeaponTypeBigGun, WeaponTypeMachineGun, WeaponTypeRocket,
	FirstWeaponType = WeaponTypeDagger, LastWeaponType = WeaponTypeRocket,
	// Materials
	Glass, Metal, Plastic, Wood, Dirt, Stone, Cement, Leather,
	// Animation
	Frame, Action, AnimationCicle, AnimationFrame, Direction, AnimationStop,
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
enum events {
	// события от нажатия клавиш
	KeyLeft = 0xE000, KeyRight, KeyUp, KeyDown, KeyPageUp, KeyPageDown, KeyHome, KeyEnd,
	KeyBackspace, KeyEnter, KeyDelete, KeyEscape, KeySpace, KeyTab,
	F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
	// события от мышки
	MouseLeft, MouseLeftDBL, MouseRight,
	MouseMove, // при движении мышки
	MouseWheelUp, MouseWheelDown,
	// другие события ввода
	InputSymbol, // при нажатию на клавишу. Заполняет hot::symbol символом с клавиатуры
	InputTimer, // при срабатывании таймера
	// диапазон клавиш алфавита
	Alpha,
	// служебные константы
	CommandMask = 0x0000FFFF,
	// различные флаги. которые могут добавляться к предыдущим событиям
	Ctrl = 0x00010000, // нажата клавиша ctrl
	Alt = 0x00020000, // нажата клавиша alt
	Shift = 0x00040000, // нажата клавиша shift
	Checked = 0x40000000, // элемент управления помечен (никогда не возвращается input(), но используется в процедурах отрисовки элементов управления)
	Disabled = 0x80000000, // элемент неактивен (никогда не возвращается input(), но используется в процедурах отрисовки элементов управления)
};
enum actions {
	ActionStand, ActionWalk, ActionClimb, ActionPickup, ActionUse, ActionDodge,
	ActionDamaged, ActionDamagedRear,
	ActionUnarmed1, ActionUnarmed2, ActionThrown, ActionRun, ActionHandsUp, ActionCloackOff,
	ActionKnockOutBack, ActionKnockOutForward,
};
enum iflags
{
	FIUseColors = 0x1,
	FINoOffset = 0x2,
	FINoCenter = 0x4,
	FIScaleToClip = 0x8,
	FIMirrorV = 0x10,
	FIReal = 0x20,
};
enum colors { ColorDisable, ColorText, ColorCheck, ColorInfo, ColorButton, ColorState };
enum areas { AreaNormal, AreaHilite, AreaPressed };
enum formats { AUTO, RAW, RLE };
// обобщенная коллекция фиксированного размера ("sdat" абривиатура от слов Static Data)
template<typename T, int maxcount = 128>
struct sdat
{
	T						data[maxcount];
	int						count;
	T*						add() { return &data[count++]; }
	void					add(T e) { *(add()) = e; }
	T*						addv() { for(int i = 0; i<count; i++) if(!data[i].isvalid()) return &data[i]; return add(); }
	void					clear() { count = 0; }
	int						find(const T e) { int i = 0; for(; i<count; i++) if(data[i]==e) return i; return -1; }
};
// Объект рамки, включающий различные операций над ней
struct rect
{
	int						x1, y1;
	int						x2, y2;
	//
	inline bool				operator!=(const rect& rc) const { return rc.x1 != x1 || rc.x2 != x2 || rc.y1 != y1 || rc.y2 != y2; }
	inline bool				operator==(const rect& rc) const { return rc.x1 == x1 && rc.x2 == x2 && rc.y1 == y1 && rc.y2 == y2; }
	void					clear() { x1 = x2 = y1 = y2 = 0; }
	inline int				height() const { return y2 - y1; }
	bool					intersect(const rect& rc) const;
	bool					isvalid() { return x1 != 0 || x2 != 0 || y1 != 0 || y2 != 0; }
	void					offset(int x, int y) { x1 += x; x2 -= x; y1 += y; y2 -= y; }
	void					set(int px1, int py1, int px2, int py2) { x1 = px1; x2 = px2; y1 = py1; y2 = py2; }
	inline int				width() const { return x2 - x1; }
};
// Точка в двух измерениях
struct point
{
	short int				x;
	short int				y;
	inline bool				operator!=(const point pt) const { return pt.x != x || pt.y != y; }
	inline bool				operator==(const point pt) const { return pt.x == x && pt.y == y; }
	bool					in(const rect& rc) const { return x >= rc.x1 && x <= rc.x2 && y >= rc.y1 && y <= rc.y2; }
	bool					in(const point p1, const point p2, const point p3) const;
};
// Цвет
union color
{
	struct rgbs
	{
		unsigned char		r;
		unsigned char		g;
		unsigned char		b;
		unsigned char		a;
	} rgb;
	unsigned				l;
	//
	operator const char*() const;
	//
	color					bright(int step) const;
	color					darken() const;
	static color			get(unsigned char index);
	color					gray() const;
	static void				init();
	unsigned char			index() const;
	color					lighten() const;
	color					mix(const color c1, unsigned char s = 128) const;
	color					negative() const;
};
namespace res
{
	struct header // заголовок каждого файла ресурсов
	{
		char				type[4]; // тип ресурса
		unsigned			size; // размер ресурса без учета этого заголовка
		void				set(const char* text);
	};
	enum token
	{
		BACKGRND, SKILLDEX, INVEN, ITEMS, MISC, SCENERY, WALLS, TILES, INTRFACE,
		FONT1, FONT2, FONT3, FONT4,
		// спрайты людей
		HANPWR, HAPOWR, HAROBE,
		HFCMBT, HFJMPS, HFLTHR, HFMAXX, HFMETL, HFPRIM,
		HMBJMP, HMBMET,
		HMCMBT, HMJMPS, HMLTHR, HMMAXX, HMMETL, HMWARR,
		MABOS2, MABOSS, MAMTNT, MAMTN2,
		MAROBE, NABLUE, NABRWN,
		NACHLD,
		NAGHUL, NAGLOW, NAPOWR, NAROBE, NAVGUL, NAWHIT,
		NFASIA, 
		NFBRLP, NFLYNN,
		NFMAXX, NFMETL,
		NFNICE, NFPEAS, NFPRIM, NFTRMP, NFVALT, NFVRED,
		NMASIA,
		NMBONC, NMBOXX, NMBPEA, NMBRLP, NMBRSR, NMBSNP,
		NMCOPP, NMDOCC, NMFATT, NMGANG, NMGRCH, NMLABB,
		NMLOSR, NMLTHR, NMMAXX, NMMEXI, NMMYRN, NMNICE, NMOLDD,
		NMPEAS, NMRGNG, NMVALT, NMWARR,
		// спрайты животных и роботов
		MAANTT, MABRAN, MABROM, MACLAW, MACLW2, MADDOG, MADEGG,
		MADETH, MAFIRE, MAGCKO, MAGKO2, MALIEN, MAMANT, MAMRAT,
		MAMURT, MAPLNT, MAQUEN, MASCP2, MASCRP, MASPHN, MASRAT,
		MATHNG, MACYBR, MAFEYE, MAGUN2, MAGUNN, MAHAND, MAROBO,
		MAROBT,
		// служебные элементы
		RESERV,
		COLOR,
		FirstResource = BACKGRND, LastResource = COLOR,
		FirstCreature = HANPWR, LastCreature = MAROBT,
	};
	formats					codetype(res::token icn, int animation, int frame = 0);
	void*					get(token icn);
	int						gaction(res::token icn, int n);
	int						gframes(token icn);
	int						gframes(token icn, int cicle);
	const char*				gname(token icn);
	int						gsx(res::token icn, int animation, int frame = 0);
	int						gsy(res::token icn, int animation, int frame = 0);
	int						gvar(res::token icn, int n);
}
namespace hot
{
	extern int				key; // Содержит последнее значение, которое вернула процедура input().
	extern point			mouse; // Текущие координаты мышки.
	extern bool				pressed; // Содержит 'true' если нажата любая клавиша мышки.
	extern char				symbol; // Символ в кодировке системы, введенный с клавиатуры
	extern int				command; // Если при входе в input() не равна 0 input() тут же вернет значение этой переменной и присвоит ей 0.
	extern unsigned			frame; // Постоянно увеличивающийся при каждом срабатывании таймера счетчик анимации.
};
namespace cursors
{
	void					arrow(int id = -1); // Отрисовка курсора в виде стрелки.
	void					cross();
	void					background(); // Отрисовка фона, который был использован перед выводом курсора.
	void					finger(); // Отрисовка курсора в виде пальца.
	void					normal(bool save = true); // Отрисовка стандартного курсора.
	void					saveback(); // Сохранить текущее изображение как фон для курсора.
	void					wait();
}
namespace io
{
	struct stream // Абстрактный класс потока. Из потока можно читать и писать информацию.
	{
		enum relocations { Begin, End, Current };
		stream&				operator<<(const char* t);
		stream&				operator<<(const int n);
		unsigned char		get(); // Получить один байт из потока.
		unsigned short		getLE16();
		unsigned			getLE32();
		unsigned			gsize();
		virtual int			read(void* result, int count) = 0; // Функцию должен перегрузить наследуемый класс.
		virtual int			seek(int count, relocations rel) = 0;
		void				skip(int count) { seek(count, Current); }
		virtual int			write(const void* result, int count) = 0;
		int					write(int value) { return write(&value, sizeof(value)); }
		int					writes(short int value) { return write(&value, sizeof(value)); }
	};
	struct file : public stream // Класс по работе с файлами операционной системы.
	{
		enum access { Read, Write, ReadWrite };
		file();
		file(const char* url, access right = Read);
		~file();
		void				close();
		bool				create(const char* url, access right);
		static void			dir(char* temp);
		static bool			exist(const char* url);
		bool				isvalid() const;
		static const char*	find(const char* mask);
		static void*		load(const char* url, int* size = 0, int additional = 0); // can release by delete operator
		virtual int			read(void* result, int count) override;
		static void			remove(const char* url);
		int					seek(int count, relocations rel) override;
		int					write(const void* result, int count) override;
	private:
		void*				handle;
	};
	stream&					debug();
}
namespace ui
{
	struct zsprite // Спайты анимации подготавлемые для вывода в определенном порядке
	{
		point				position;
		res::token			rid;
		int					fid;
		int					an;
		unsigned			flags;
		static int			compare(const void* p1, const void* p2, void* param);
	};
	struct colori
	{
		unsigned char		color[3];
		unsigned char		palet[6][256];
	};
	struct states
	{
		states();
		~states();
	private:
		rect				clipping;
		colori*				fore;
		res::token			font;
	};
	struct cicle
	{
		unsigned short		active; // Активный кадр анимации
		unsigned short		count; // Количесто кадров анимации в 6 положениях.
		unsigned			offset; // Смещение к данным картинки
		struct frame*		ptr(struct sprite* p) const;
	};
	struct frame
	{
		formats				encode;
		short unsigned		sx;
		short unsigned		sy;
		short				cx;
		short				cy;
		short				ox;
		short				oy;
		short				mx;
		short				my;
		unsigned			offset;
		unsigned char*		ptr(struct sprite* p) const;
	};
	struct sprite : public res::header
	{
		int					count;
		unsigned			flags;
		int					vars[6];
		cicle				cicles[1];
		//
		unsigned			find(unsigned char* p, int count);
		unsigned char*		ptr();
		void				setup(int count);
		void				write(const char* url);
	};
	namespace dialog
	{
		void				combat();
		int					charedit();
		int					game();
		bool				gender(int x, int y, int& val);
		void				inventory(int mid);
		int					newgame();
		bool				numage(int x, int y, int& val, int minimum=1, int maximum=99);
		void				message(const char* text);
		bool				string(int x, int y, char* text, int maxchars = 13);
		void				worldmap();
	}
	namespace show
	{
		void				begin();
		void				begin_combat();
		void				item(int x, int y, int sx, int sy, unsigned it, bool resize = true);
		void				hexagon(int ox, int oy, int t);
		void				map(int ox, int oy, int mvc);
		void				mapview(int index = -1);
		void				grid(int ox, int oy, int mvc);
		void				inventory(int x, int y, int& first, int count, int mid);
		void				end();
		void				end_combat();
		void				tiles(int ox, int oy, int mvc);
	}
	const int				width = 640;
	const int				height = 480;
	//
	areas					area(int id, const rect& rc);
	void					button(int x, int y, int width, int id, const char* string = 0, int key = 0, int state = 0);
	bool					buttonf(int x, int y, int id, int normal, int pressed, bool action, unsigned state, areas* ra = 0, rect* rrc = 0);
	void					buttonp(int x, int y, int id, int normal, int pressed, const char* string, int ty = 6, unsigned state = 0);
	void					caption(const char* text);
	extern rect				clipping;
	bool					create(const char* title);
	void					debuging(int ox, int oy);
	void					execute(int id);
	extern res::token		font;
	frame*					gframe(res::token id, int animation, int frame = 0);
	unsigned char			gcol(int n = 0);
	void					highlight();
	bool					hittest(int x, int y, res::token icn, int frame, unsigned flags = 0, int animation = 0);
	void					image(int x, int y, res::token icn, int frame, unsigned flags = 0, int animation = 0);
	int						input();
	void					inputex();
	void					line(int x1, int y1, int x2, int y2, unsigned char color);
	int						main(const char* cmdline);
	void					number(int x, int y, int width, int id, int value);
	extern unsigned char	pallette[256*3];
	void					pixel(int x, int y, unsigned char c);
	unsigned char*			ptr(int x, int y);
	void					radio(int x, int y, int id, int frame, int key, unsigned state = 0);
	void					rectb(int x1, int y1, int x2, int y2, unsigned char color);
	void					rectf(int x1, int y1, int x2, int y2, unsigned char color);
	extern int				scanline;
	void					scol(colors index);
	void					shortcut(int key, int cmd);
	void					store(sprite* image, int id, unsigned char* src, int w, int h, int ox, int oy, formats ft, int animation_count, int animation, int action, int psx, int psy, int msx, int msy);
	void					timer(int milliseconds);
	int						text(int x, int y, const char* string, int count = -1);
	int						text(int x, int y, int width, const char* string);
	int						textbc(const char* string, int count, int width);
	int						texth();
	int						texth(const char* string, int width);
	int						textm(int x, int y, int width, const char* string);
	int						textw(const char* string, int count = -1);
	void					tumbler(int x, int y, int n);
	void					usepal(unsigned char* p);
	extern sdat<zsprite, 1024> zsprites;
}
namespace arrays
{
	int						count(int* a);
	int						random(int* a);
}
struct damagei
{
	tags					type;
	short unsigned			min;
	short unsigned			max;
};
struct gears
{
	unsigned				body;
	unsigned				hands[2];
};
namespace items
{
	extern damagei			damage;
	void					add(unsigned& i1, unsigned& i2);
	unsigned				create(tags type, int count = 1, int param = 0);
	int						get(unsigned mid, int id);
	const char*				gets(unsigned mid, int id);
	unsigned				set(unsigned mid, int id, int value);
	unsigned				pick(unsigned& i1);
	inline tags				type(unsigned mid) { return tags(mid&0xFFFF); }
}
namespace perks
{
	int						get(int mid, int id);
	const char*				gets(int mid, int id);
}
namespace premade
{
	int						get(int mid, int id);
	const char*				gets(int mid, int id);
	tags					tag(int mid, int pos);
	tags					traits(int mid, int pos);
}
namespace creatures
{
	void					animate();
	bool					add(int mid, unsigned item, int id = -1, int pos = -1, bool run = true);
	void					animate();
	void					clear();
	void					check(int skill);
	void					create(int mid, int pid);
	void					damage(int mid, int weapon = 0);
	bool					isallow(int mid, int id);
	bool					istagged(int mid, int id);
	int						invertory(unsigned** reuslt, int count, int mid);
	unsigned*				gears(int mid, int id, int pos = 0);
	int						get(int id);
	int						get(int mid, int id);
	int						getr(int mid, int id);
	const char*				gets(int mid, int id);
	bool					set(int mid, int id, int value);
	void					setr(int mid, int id, int value);
	bool					set(int mid, int id, const char* value);
	void					settag(int mid, int id, int value);
}
namespace units
{
	int						add();
	int						find(int index);
	int						get(int mid, int id);
	const char*				gets(int mid, int id);
}
namespace world
{
	void					clear();
	int						day(unsigned round);
	int						year(unsigned round);
	int						month(unsigned round);
	int						hour(unsigned round);
	int						minute(unsigned round);
	void					msg(const char* text);
	int						get(int id);
	unsigned				round(int year, int month, int day);
	bool					set(int id, int value);
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
	const int				tilew = 80;
	const int				tileh = 36;
	const int				gridw = 32;
	const int				gridh = 16;
	extern point			camera;
	void					create();
	extern int				cols;
	directions				direction(int from, int to);
	int						d2o(directions d);
	bool					execute(int id);
	unsigned short			floor(int index);
	int						gx(int tile);
	int						gy(int tile);
	short unsigned			lookup(int index, map::tokens tile);
	int						move(int index, int orientation);
	int						h2x(int ht);
	int						h2y(int ht);
	int						pos(int tx, int ty);
	int						pos2(int tx, int ty);
	extern int				rows;
	unsigned short			scene(int index);
	void					sfloor(int index, short unsigned fid);
	void					sscene(int i, short unsigned fid);
	int						t2x(int tile);
	int						t2y(int tile);
	int						xy2t(int x, int y);
	int						xy2h(int x, int y);
	unsigned short			wall(int index, int n = 0);
	tokens					group(short unsigned tile);
	namespace generate
	{
		void				highway(int i1, int count, int width);
	}
}
namespace game
{
	int						d100();
}
extern char					chupper(char c); // to upper reg
extern char					chlower(char c); // to lower reg
const char*					gstr(int id); // get name of string token (export)
extern bool					ischa(int c); // is alphabetical character?
extern int					locale; // current locale (defined external)
void						rmcpy(void* p1, const void* p2, int count);
void						rmmov(void* p1, const void* p2, int count);
void						rmset(void* p1, int value, int count);
int							rmcmp(const void* p1, const void* p2, int count);
void*						rmresize(void* ptr, unsigned size);
unsigned					rmsize(void* ptr);
const char*					szchr(const char* p1, char ch);
void						szcpy(char* p1, const char* p2);
void						szcpy(char* p1, const char* p2, int max_count);
const char*					szcmd();
int							szcmp(const char* p1, const char* p2);
int							szcmp(const char* p1, const char* p2, int count);
int							szcmpi(const char* p1, const char* p2);
int							szcmpi(const char* p1, const char* p2, int count);
const char*					szdup(const char *text, int size);
char*						szend(char* text);
inline const char*			szend(const char* text) { while(*text) text++; return text; }
extern int					szerror(char* result, int count, int id);
const char*					szext(const char* path);
const char*					szline(char* result, const char* p);
const char*					sznameof(const char* text);
void*						xfind(const void *key, const void *base, int count, int size, int(*compar)(const void* p1, const void* p2, void* param), void* param = 0);
const char*					szfname(const char* text); // get file name from string (no fail)
const char*					szfpath(char* path, const char* fullpath);
const char*					szmkpath(char* result, const char* fullpath, const char* filename, const char* ext = 0);
char*						sznum(char* p1, int num, int precision = 0, const char* empthy = 0, int radix = 10);
const char*					sznumber(int value, int percision = 0);
const char*					szpercent(int percent);
char*						szprint(char* dst, const char* src, ...);
inline const char*			szskipsp(const char* p) { while(*p==' ' || *p=='\t') p++; return p; }
const char*					szskipcr(const char* p);
const char*					szt(const char* en, const char* ru = 0);
int							sz2num(const char* p1, const char** pp1 = 0);
void						swap(unsigned short& u);
void						swap(unsigned& u);
extern int					timeticks(); // get random seed
void						xsrand();
void						xsrand(unsigned seed);
int							xrand();
bool						xrun(const char* url, char* command_line, const char* directory, bool handle_output = false, bool hide_window = false);
void						xsort(void* base, int count, int size, int(*compare)(const void* p1, const void* p2, void* param), void* param = 0);
const char*					upper(const char* string);
inline void					szcat(char* p1, const char* p2) { szcpy((char*)szend(p1), p2); }
inline char*				szchr(char* text, char ch) { return (char*)szchr((const char*)text, ch); }
inline int					szlen(const char* text) { return szend((char*)text) - text; }
template<class T> inline T	imax(T a, T b) { return a > b ? a : b; }
template<class T> inline T	imin(T a, T b) { return a < b ? a : b; }
template<class T> inline T	iabs(T a) { return a > 0 ? a : -a; }
template<class T> inline void iswap(T& a, T& b) { T i = a; a = b; b = i; }