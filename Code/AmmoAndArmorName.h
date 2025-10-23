#include "definitions.h"
#include "Weapon.h"

#define ActualArmorTypes (NumberOfArmorTypes - 1)

static const char * AmmoName[NumberOfBulletTypes]={
	"9mm","5.56mm","7.62mm","12.7mm","40mm Shells",
	"40mm HE Shells","AP Missile", "AT Missile",
	"Heavy AT Shot","Shells","Batteries","Charges",
	"Mines","Tank Mines", "Grenades", "40mm Shells", "AP Rockets", "Missile"};

static const char * ArmorName[ActualArmorTypes][5]={
	{"Bulletproof", "West","Bullet Protection","bullets","a"},
	{"Anti Shrapnel", "Helmet","Explosion Protection", "most explosions", "a"},
	{"Armored", "Pants","Overall Protection","all type of attacks", "a pair of"},
	{"Anti Mine", "Boots","Mine Protection","mines", "a pair of"},
	{"Defensive", "Aura","Energy Protection","laserbeams and tazers", "an"}
};

static const char * WeaponTypeName[wNum]={"Pistol","Sub Machine Gun","Regular Rifle","Sniper Rifle","Assault Rifle","Machine Gun","Regular Shotgun","Assault Shotgun","Mortar","Rocket Launcher","Tazer"};
