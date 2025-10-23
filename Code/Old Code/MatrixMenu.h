#ifndef __MatrixMenu__ 
#define __MatrixMenu__

class MatrixMenu
{
private:

	int SubScripts,Rows;
	char
public:

	MatrixMenu();
	~MatrixMenu();

	void AddSubScript();
};
#endif

	EnemySetupMenu->AddSubscript("Type");
	EnemySetupMenu->AddSubscriptStaticItem("Droids");
	EnemySetupMenu->AddSubscriptStaticItem("Tanks");
	EnemySetupMenu->AddSubscriptStaticItem("Cars");
	EnemySetupMenu->AddSubscriptStaticItem("Machines");
	EnemySetupMenu->AddSubscript("BaseCount");
	EnemySetupMenu->AddSubscriptStep(1,100,1);
	EnemySetupMenu->AddSubscript("Frequency");
	EnemySetupMenu->AddSubscriptItem("Very Low");
	EnemySetupMenu->AddSubscriptItem("Low");
	EnemySetupMenu->AddSubscriptItem("Medium");
	EnemySetupMenu->AddSubscriptItem("High");
	EnemySetupMenu->AddSubscriptItem("Very High");
	EnemySetupMenu->AddSubscript("Upgrade Likeliness");
	EnemySetupMenu->AddSubscriptItem("Very Low");
	EnemySetupMenu->AddSubscriptItem("Low");
	EnemySetupMenu->AddSubscriptItem("Medium");
	EnemySetupMenu->AddSubscriptItem("High");
	EnemySetupMenu->AddSubscriptItem("Very High");