/**
 * ���������
 */
#pragma once
#include "cocos2d.h"

USING_NS_CC;
class IRole;
/**
 * ���ܿ����
 */
class CDShowItem:public Node{
public:
	CREATE_FUNC(CDShowItem);
	bool init();
	/**
	 * չʾ
	 */
	void show();
	/**
	 * ��������
	 */
	void setTexture(Texture2D *texture);

	CDShowItem()
	{
		_valid = true;
		_sprite = NULL;
		_canMove = false;
		info = NULL;
	}
	std::string skillName;

	virtual void doClickMe() {}
private:
	bool _canMove;
	bool _valid;
	void cartoonOver();
	Sprite *_sprite; // չʾ����
	ProgressTimer *cdShow;
	LabelTTF *info;
};
/**
 * ������
 */
class SkillItem :public CDShowItem{
public:
	CREATE_FUNC(SkillItem);
	void initWithInfo(const char *pngName,int skillId);
	int skillId; // ����
	void doClickMe();
};
/**
 * �л�Ӣ����
 */
class ChangeHeroItem :public CDShowItem{
public:
	CREATE_FUNC(ChangeHeroItem);
	void initWithInfo(const char*pngName, int heroId);
	int heroId;
	void doClickMe();
};
/**
 * ������
 */
class SkillShowPanel :public Node{
public:
	CREATE_FUNC(SkillShowPanel);
	void push(CDShowItem *item);
	std::vector<CDShowItem*> items;
	void show();
};
class TaskTalkUI :public Node{
public:
	CREATE_FUNC(TaskTalkUI);
	bool init(){ return true; }
	/**
	 * ͨ������ �� ��Ϣ��ʼ�������
	 */
	void initWithInfo(const char *name, const char *info);
	/**
	 * ˵��
	 */
	void talk();
};

class ChoiceHeroUI :public Node{
public:
	CREATE_FUNC(ChoiceHeroUI);
	bool init();
};

class HeroHeadUI :public Node{
public:
	CREATE_FUNC(HeroHeadUI);
	bool init();
};

class ChoiceBossUI :public Node{
public:
	CREATE_FUNC(ChoiceBossUI);
	bool init();
};

class QuickUI :public Node{
public:
	CREATE_FUNC(QuickUI);
	bool init();
};
enum{
	MY_TOUCH_DOWN,
	MY_TOUCH_MOVE,
	MY_TOUCH_END,
};
/**
 * �ƶ�������
 */
class MoveOPUI :public Node{
public:
	CCPoint opMoveCenter;
	// ���Բ�����ɫ�ƶ� 
	static MoveOPUI* create(IRole *player);
	bool init(IRole *player);
	IRole *player;
	Touch *nowTouch;
	/**
	* ������
	* \parma touchType �������
	* \param touchPoint �����
	*/
	bool doTouch(int touchType, Touch* touch);
	MoveOPUI()
	{
		nowTouch = NULL;
		player = NULL;
		nowTouchIn = false;
		tagSprite = NULL;
	}
	bool nowTouchIn; // ��ǰ�Ƿ������ϲ���

	void showTouch(int dirType);
	Sprite * tagSprite;
}; // �ƶ��� ����һֱ��Game��

class MainUI:public Node{
public:
	CREATE_FUNC(MainUI);
	bool init();
};