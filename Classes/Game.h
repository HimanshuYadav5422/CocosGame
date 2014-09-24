/**
 * ����Game
 */
#pragma once
#include "MainUI.h"
#include "Scene.h"

#include "cocos2d.h"
class IRole;
#include "Map.h"
class Game : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(Game);
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event  *event);
	//ͨ������������� ������ lable��λ��
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event  *event);
	/**
	 *  �����Լ�
	 **/
	void update(float dt);

	/**
	 * ���ػ�ȡ������
	 */
	void doGetGate(void*cmd, int len);

	/**
	 * �����޵��������
	 */
	void doLoginError(void *cmd, int len);

	/**
	 * �����¼�ɹ�
	 */
	void doLoginOk(void *cmd, int len);

	/*
	 * �����ͼ��Ϣ 
	 **/
	void doGetMap(void *cmd, int len);

	/**
	 * ��ȡ�����˵���Ϣ
	 */
	void doGetOthers(void *cmd, int len);
	/**
	 * �ƶ�Npc
	 */
	void doMoveNpc(void *cmd, int len);
	/**
	 * �ƶ�User
	 */
	void doMoveUser(void *cmd, int len);
	/**
	 * ��ȡ����������
	 */
	void doGetMainRoleData(void *cmd, int len);
	/**
	 * �����������
	 */
	void doAddOther(void *cmd, int len);
	/**
	 * ����NPC
	 */
	void doAddNpc(void *cmd, int len);
	/**
	 * ��������
	 */
	void doAttackRole(void *cmd, int len);
	MoveOPUI *moveOPUI;
};