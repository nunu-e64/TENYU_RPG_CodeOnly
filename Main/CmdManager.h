////多重インクルード防止（インクルードガード）//
#ifndef CMDMANAGER_H						////
#define CMDMANAGER_H						////
////////////////////////////////////////////////

class CWorldManager;
class CField;
class CMap;
class CEveManager;
class CTextBox;

class CBattle;
class CPlayerSpeciesManager;
class CEnemySpeciesManager;
class CTrickManager;


class CCmdManager{
public:
	~CCmdManager(){};
	
protected:

	bool NextCommand(CCmdList* _cmdlist, char* commandline, char* command, char* &argument);
	void ArgCut(const char* _command, char* _argument, char** _arg, int _argnum, bool _warning=true);

	bool MusicCmdSolve (const char* _command, char* _argument, CField* _field, CMap* _map, CTextBox* _textbox, CEveManager* _evemanager);
	bool FieldCmdSolve (const char* _command, char* _argument, CField* _field, CMap* _map, CTextBox* _textbox, CEveManager* _evemanager);
	bool SystemCmdSolve(const char* _command, char* _argument, CField* _field, CMap* _map,                     CEveManager* _evemanager);
	bool WindowCmdSolve(const char* _command, char* _argument, CWorldManager* _worldmanager, CMap* _map, CTextBox* _textbox);
	bool TextCmdSolve  (const char* _command, char* _argument, CWorldManager* _worldmanager, CTextBox* _textbox);

	bool BattleSystemCmdSolve(const char* _command, char* _argument, CPlayerSpeciesManager _playerSpeciesManager, CEnemySpeciesManager _enemySpeciesManager, CTrickManager _trickManager);
	bool BattleCmdSolve(const char* _command, char* _argument, CBattle* _battle);

};

class CFirstSetCmdManager : public CCmdManager{
public:
	void Main(CCmdList* _cmdlist, CField* _field, CMap* _map, CEveManager* _evemanager);
};

class CFieldCmdManager : public CCmdManager{
public:
	void Main(CCmdList* _cmdlist, CField* _field, CMap* _map, CTextBox* _textbox, CEveManager* _evemanager);
};

class CBattleCmdManager : public CCmdManager{
public:
	CBattleCmdManager(){Map_p=NULL;}
	void Main(CCmdList* _cmdlist, CBattle* _battle, CTextBox* _textbox);
	void Init(CMap* _map){Map_p=_map;}

private:
	CMap* Map_p;				//Fieldから受け継ぐ

};

////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////