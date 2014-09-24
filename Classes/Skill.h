#pragma once
/**
 * ���ܵĲ���
 */
#include "XmlSkills.h"
#include "UtilAnimation.h"
/**
 * ���ܲ�����ȡ�����Ϣ�ӿ�
 */
class ISkill{
public:
	virtual Point GetAttackObjectPosition() = 0;// ��ȡ������λ��
	virtual Point GetGameObjectPosition() =0;//��ü���ӵ���ߵ�λ��
	virtual Node * getEffectLayer(int ID) { return NULL; }// ��ȡ��Ч���ڵĲ�
	virtual std::string getRoleName(){ return ""; } // ��ȡ��ɫ������[���ǽ�ɫ���ļ���]
	virtual bool checkHurts(std::vector<IRole*>& out, const Point &point, Sprite *src, const std::string & effectID){ return false; }
};
/**
* ������Ч
*/
class SkillSoundEffect{
public:
	void stopPlay();
	SoundConfig *config;
	void playSound();
	int soundId;
	SkillSoundEffect()
	{
		soundId = 0;
	}
};
class Skill;
/**
 * ������Ч��
 */
class SkillSubEffect:public Node{
public:
	ISkill *skill;
	CREATE_FUNC(SkillSubEffect);
	bool init(){ return true; }
	SubEffectConfig *config;
	SkillSoundEffect sound;
	Sprite *sprite; // ��Ч��Ӧ�ľ���
	void startPlay();
	SkillSubEffect()
	{
		config = NULL;
		skill = NULL;
		sprite = NULL;
		picDir = 0;
		costDt = 0;
		reachX = reachY = false;
	}
	bool init(Skill *skill);
	void update(float dt);
	void updatePosition(float dt);
	int currFrame; // ��ǰ֡
	float nowSpeed; // ��ǰ�ٶ�
	float moveAngle; // �ƶ��Ƕ�
	float currSpeedX; // ��ǰ������
	float currSpeedY; // ��ǰ������
	float accelerate; // ���ٶ�
	float rotateAngle; // ��ת�ű�
	float rotateSpeed; // ��ת�ٶ�
	int interval;
	bool startPlayTag; // �Ƿ�ʼ��ʾ
	bool playFinish; // �Ƿ񲥷Ž���
	float connectDistance; // ����������ľ���
	float getPointAngle(float x, float y);

	void show(bool); // չʾ �� ����
	bool checkFinish(); // ����Ƿ����
	void updateRotate(float dt); // ���½Ƕ�
	void updateSound(float dt); // ��������
	void changeFrame(); // �л�֡
	//�����һ֡�Ŀ�����Ϣ
	//�ӳٵļ�¼�ӳٵ�֡��
	//�����ĵ�ǰ֡��һ
	void checkFrameCtrl(int frame);
	int frameIndex; // ��ǰ֡
	int currDelayCount; // ��ǰ��֡
	CacheAnimation cacheAnimation; // ֡����
	int totalFrame; // ����֡������
	void calcTotalFrame(); // ������֡��
	bool checkOutOfRange(); // �жϲ�����Χ
	bool haveReach(); // �Ƿ񵽴�Ŀ��
	bool reachX; // �Ƿ�X�ᵽ��
	bool reachY; // �Ƿ�Y�ᵽ��
	int maxAtkDisttance; // ����ܹ�������
	int attackDistance; // չʾ����
	Point startPos; // ��ʼλ��
	void calcStartPosition(); // ���㿪ʼλ��
	int currDir; // ��ǰ����
	void changeDir(int dir); // ��ǰ����
	int picDir; // ͼƬ�ķ���
	float deg2Rad(float val){ return 3.1415926 /180 * val; }
	float getDirDegree(int val);
	float costDt; //  ÿ֡���
	bool checkHurt(); // ����˺�
};
/**
 * ����Ч��
 */
class SkillEffect{
public:
	void init(Skill *skill);
	void startPlay();
	void changeDir(int dir); // ���ķ���
	std::vector<SkillSubEffect*> subEffects;
	typedef std::vector<SkillSubEffect*>::iterator SUBEFFECTS_ITER;
	EffectConfig *config;
	SkillEffect()
	{
		config = NULL;
	}
};
class Skill;
/**
 * ���ܶ���
 */
class SkillAction{
public:
	/**
	 * ��ȡ����֡
	 */
	SpriteFrame * getActionFrame();
	/**
	 * ����״̬ ��ȡ�µĶ�������
	 */
	void changeAction();

	/**
	 * �л�����
	 */
	void changeDir(int dir);
	//�����һ֡�Ŀ�����Ϣ
	//�ӳٵļ�¼�ӳٵ�֡��
	//�����ĵ�ǰ֡��һ
	bool checkFrameCtrl(int frame);

	/**
	 * ��ʼ��
	 */
	void init(Skill *skill);
	CacheAnimation cacheAnimation;

	ActionConfig *config; // ���õľ�̬��Ϣ

	
	int currFrame; // ��ǰ֡
	int currFrameInEle; //��ǰ֡����
	int currDelayCount; // ��ǰ��ʱ����
	int dir; // ����
	ISkill *skill; // ��ǰ����
	SkillAction()
	{
		config = NULL;
		currFrame = 0;
		currFrameInEle = 0;
		currDelayCount = 0;
		dir = 0;
		skill = NULL;
	}
	/**
	 * ����
	 */
	void end();
	void startPlay();
};

class SkillFrame{
public:
	std::vector<SkillEffect> effects;
	typedef std::vector<SkillEffect>::iterator EFFECTS_ITER;
	FrameConfig* config;
	void init(Skill *skill);
	void startPlay();
	SkillFrame()
	{
		config = NULL;
	}
	SkillAction action; // ��ǰ����
	SkillSoundEffect sound; // ��Ч
	void changeDir(int dir); // ���ķ���
};
/**
 * 1. ����ʱչʾ��ض���
 */
class Skill:public ISkill{
public:
	Skill()
	{
		currFrame = 0;
		dir = 0;
		config = NULL;
		parent = NULL;
		currDelayCount = 0;
		currFrameInEle = 0;
		nowAction = NULL;
		me = NULL;
		target = NULL;
	}
	IRole * me;
	IRole *target;
	Point targetPosition;
	int currFrame; //��ǰ֡,�ö����ĵ�ǰ֡
	int currFrameInEle; //��ǰԪ����֡������
	int currDelayCount; //��ǰ�ӳٵ�֡��
	int dir;//����
	void setCurrectDir(int dir); // ���÷���
	SkillConfig *config; // ������ؾ�̬��Ϣ
	void init(); // ��ʼ��
	void update(); // ����
	void checkUpdate(int frame); // ��鵱ǰ֡����
	void end(); // ����
	bool actionPlayFinish;
	bool isOver(){ return actionPlayFinish;} // �Ƿ����
	SpriteFrame * getActionFrame(); // ��ȡ��ǰ����֡
	virtual Point GetAttackObjectPosition();// ��ȡ������λ��
	virtual Point GetGameObjectPosition();//��ü���ӵ���ߵ�λ��
	SkillAction *nowAction;
	std::vector<SkillFrame> frames; // ÿ֡��Ϣ
	typedef std::vector<SkillFrame>::iterator FRAMES_ITER;
	void startPlay();
	Node *parent;
	Node * getEffectLayer(int ID);
	std::string roleName;
	std::string getRoleName(){ return roleName; }
	/*
	* ������ײ����
	**/
	CCRenderTexture *rtForC;

	bool checkHurts(std::vector<IRole*>& out, const Point &point, Sprite *src, const std::string & effectID);
	bool checkCollide(Sprite *src, Sprite *dest);
};