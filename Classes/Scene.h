#pragma once
#include "cocos2d.h"
#include "IRoleState.h"
#include "Astar.h"
#include "Map.h"
USING_NS_CC; 

/**
 * ����
 */
class MapScene{
public:
	enum{
		_BLOCK = 1 << 0,
		_CAN_WALK = 1 << 1,
	};
	/**
	 * ��ȡ�赲����Ϣ
	 */
	int getBlock(const Point &point);
	/**
	 * ���ļ��м����赲����Ϣ
	 */
	void readBlockInfoFromFile(const char *fileName);

	/**
	 * �����ص�ת��Ϊ����
	 */
	static GridIndex transPixelPointToGird(const Point &point);
	/**
	 * ������ת��Ϊ���ص�
	 */
	static Point transGirdToPixelPoint(const GridIndex &point);
private:
	std::vector<int> _blockInfos;
};

/**
 * ����������
 */
class SceneManager{
public:
	static SceneManager *me;
	static SceneManager * getInstance();
	SceneManager()
	{
		mainRole = NULL;
		uiLayer = NULL;
		nowTerrain = NULL;
		rtForC = NULL;
	}
	// ����
	IRole * mainRole;
	enum{
		UI_LAYER = 4,
		MAP_LAYER = 0,
		ROLE_LAYER = 1,
		EFFECT_LAYER = 2,
		POINTER_LAYER = 3,
	};
	void addTo(int layerID, Node *node);
	void remove(Node *node);
	// ��ͼ
	Terrain * nowTerrain;
	// ���ӽ�ɫ
	void addOtherRole(IRole *role);

	Node *uiLayer;// UI ��

	void doTick(float dt); // ��ʱ��

	bool checkHurts(std::vector<IRole*>& out, Sprite *src);

	CCRenderTexture * rtForC;
	/**
	 * ��ȡ��������
	 */
	Point getAttackObjectPosition(Node *role);

	void addNpcByTempId(int tempid, IRole * role);
	IRole* getNpcByTempId(int tempid);

	void addUserByTempId(int tempid, IRole * role);
	IRole* getUserByTempId(int tempid);
private:
	std::vector<IRole* > _roles; // ��ɫ����
	typedef std::vector<IRole*>::iterator ROLES_ITER;

	std::map<int, IRole *> _npcs; // npc����
	typedef std::map<int, IRole*>::iterator NPCS_ITER;

	std::map<int, IRole *> _users; // user����
	typedef std::map<int, IRole*>::iterator USERS_ITER;
};