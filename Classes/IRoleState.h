#pragma once
/**
 * ���ﶯ�������
 * ��· ��ֹ ���� ����
 */
#include <vector>
#include <cocos2d.h>
#include "Astar.h"
#include "Skill.h"
USING_NS_CC;
class IRole;
class Msg{
public:
	enum{
		TOUCH_MSG = 1, // ����¼�
		CARTOON_OVER = 2, // ��������
		BE_ATTACK = 3, // ����
		ATTACK = 4, // ����
		FRAME_UPDATE = 5, // ֡����ʱ��
		GOTO_MSG = 6, // ֱ���ƶ�
	};
	int MsgType;
	Msg()
	{
		MsgType = 0;
	}
	Msg(int msg)
	{
		MsgType = msg;
	}
	virtual Msg *clone(){ return NULL; }
};
/**
* ����¼�
*/
class TouchMsg :public Msg{
public:
	Point point;
	TouchMsg() :Msg(TOUCH_MSG){}
	virtual Msg *clone()
	{ 
		TouchMsg *msg = new TouchMsg();
		msg->point = point;
		return msg;
	}
};
class GoToMsg :public Msg{
public:
	Point point;
	GoToMsg() :Msg(GOTO_MSG){}
	virtual Msg *clone()
	{
		GoToMsg *msg = new GoToMsg();
		msg->point = point;
		return msg;
	}
};
/**
* ���������¼�
*/
class CartoonOverMsg :public Msg{
public:
	CartoonOverMsg() :Msg(CARTOON_OVER){}
	virtual Msg *clone()
	{
		CartoonOverMsg *msg = new CartoonOverMsg();
		return msg;
	}
};
/**
* �����¼�
*/
class BeAttackMsg :public Msg{
public:
	IRole *from; // ������ 
	BeAttackMsg() :Msg(BE_ATTACK){
		from = NULL;
	}
	virtual Msg *clone()
	{
		BeAttackMsg *msg = new BeAttackMsg();
		msg->from = from;
		return msg;
	}
};
/**
* �����¼�
*/
class AttackMsg :public Msg{
public:
	int skillId; // ʹ�õļ��ܱ��
	Point point; // �����ĵ�
	IRole *target; // �����Ķ���
	AttackMsg() :Msg(ATTACK){
		skillId = 0; target = NULL;
	}
	virtual Msg *clone()
	{
		AttackMsg *msg = new AttackMsg();
		msg->point = point;
		msg->target = target;
		msg->skillId = skillId;
		return msg;
	}
};
/**
 * ֡�¼�
 */
class UpdateFrameMsg :public Msg{
public:
	float dt;
	UpdateFrameMsg() :Msg(FRAME_UPDATE)
	{
		dt = 0;
	}
};
class RoleStateMangager;
class IRoleState:public Ref{
public:
	/**
	* �����״̬
	*/
	virtual void Enter() {}
	/**
	* �˳���״̬
	*/
	virtual void Exit() {}

	/**
	* ״̬������
	*/
	virtual void PasreMsg(Msg * msg) = 0;

	/**
	* ״̬������
	*/
	RoleStateMangager *rsm;
	/**
	* ������
	*/
	IRole *role;
	IRoleState(){ rsm = NULL; role = NULL; }

	virtual Sprite * getSprite(){ return NULL;}

	void cartoonOver();
public:
	/**
	 * ��ͬ���͵���Ϣֻ�����µ�
	 */
	void pushMsg(Msg *msg);
	/**
	 * ��Ϣ����
	 */
	std::list<Msg*> msgBuffer;
	typedef std::list<Msg*>::iterator MSGBUFFER_ITER;
	/**
	 * �����ϢBuffer
	 */
	void clearMsgBuffer();
};

class RoleStateMangager{
public:
	/**
	* �л���ǰ״̬
	*/
	void chageState(IRoleState *nowState);
	/**
	* ���浱ǰ״̬
	*/
	void pushState(IRoleState *state);
	/**
	* �ָ���ǰ״̬
	*/
	IRoleState *popState();

	/**
	* ��ȡ��ǰ״̬
	*/
	IRoleState * nowState();

	/**
	* ������Ϣ
	*/
	void PasreMsg(Msg *msg);


private:
	std::vector<IRoleState*> _states;
};
/**
* ����״̬
*/
class IdleRoleState :public IRoleState{
public:
	IdleRoleState(){
		_sprite = NULL;
		_action = NULL;
	}
	/**
	* �����״̬
	* ��ʼ����IDLE����
	*/
	virtual void Enter();
	/**
	* �˳�ʱ
	* �����״̬
	*/
	virtual void Exit();

	/**
	* ״̬������ ����Ӧ����¼� �����¼� ���������¼�
	*/
	virtual void PasreMsg(Msg * msg);
	Sprite * getSprite(){ return _sprite; }
private:
	Action *_action; // ��ǰ����
	Sprite *_sprite; // ��ǰ����
};
/**
* ��·״̬
*/
class WalkRoleState :public IRoleState{
public:
	WalkRoleState()
	{
		_animation = NULL;
		_sprite = NULL;
		index = 0;
		costDt = 0;
		stepCostDT = 0;
	}
	/**
	 * ������·����
	 */
	virtual void Enter();
	/**
	* ��Ӧ�����¼� ���������¼� ��Ӧ����¼�
	*/
	virtual void PasreMsg(Msg * msg);
	/**
	 * �������
	 */
	virtual void Exit();
	/**
	 * �����¼�
	 */
	void update(float dt);

	GridIndex point; // ��ǰҪ�ߵ������

	Sprite * getSprite(){ return _sprite; }
private:
	Animation *_animation; // ��ǰ����
	Sprite *_sprite; // ��ǰ����
	int index;
	float costDt; // ��ǰ֡����ʱ���ۻ�
	float stepCostDT; // �ƶ�ʱ���ۻ�
private:
	Vec2 _positionDelta;
	Vec2 _startPosition;
	Vec2 _previousPosition;
};
/**
* �Զ�Ѱ·״̬
*/
class AutoWalkRoleState :public IRoleState{
public:
	/**
	* �����״̬ʱ �ж��Ƿ񵽴�Ŀ��� �������� �л����ϴ�״̬
	*/
	virtual void Enter();
	/**
	* ��Ӧ����¼�
	*/
	virtual void PasreMsg(Msg * msg);
	/**
	 * Ŀ�ĵ�
	 */
	Point destination;

	Sprite *mousePoint;

	Action *action;
	AutoWalkRoleState()
	{
		mousePoint = NULL;
		action = NULL;
	}
};
/**
* ����״̬
*/
class AttackRoleState :public IRoleState{
public:
	/**
	 * �����״̬
	 */
	virtual void Enter();

	/**
	 * �뿪
	 */
	virtual void Exit();
	/**
	 * ������Ϣ
	 */
	virtual void PasreMsg(Msg * msg);

	/**
	 * ����
	 */
	virtual void update(float dt);
	AttackRoleState()
	{
		currentFrame = 1;
		actionPlayFinish = false;
		_sprite = NULL;
		costDt = 0;
		target = NULL;
	}
	int currentFrame;
	Skill skill;
	float costDt;
	bool actionPlayFinish;

	Sprite * getSprite(){ return _sprite; }

	IRole *target;
	Point targetPosition;
private:
	Sprite *_sprite; // ��ǰ����
};

/**
 * ����״̬
 */
class BeAttackRoleState:public IRoleState{
public:
	virtual void Enter();
	virtual void Exit();
	virtual void PasreMsg(Msg * msg);
	Sprite * getSprite(){ return _sprite; }
	BeAttackRoleState()
	{
		_action = NULL;
		_sprite = NULL;
		retain();
		beAttacker = NULL;
	}
	IRole * beAttacker;
private:
	Action *_action; // ��ǰ����
	Sprite *_sprite; // ��ǰ����
};
/**
* ������ɫ
*/
class IRole :public Node{
public:
	CREATE_FUNC(IRole);
	/**
	* ���Ŷ���
	*/
	void play();
	/**
	* ���������ص�
	*/
	void cartoonOver();

	IRole()
	{
		direction = 1;
		_sprite = 0;
		continueMove = false;
		tempID = 0;
	}
	/**
	* ��ʼ��
	*/
	bool init();
	/**
	* ���
	*/
	void onTouch();
	/**
	 * ������������
	 */
	void changeAnimation(const char *aniName);
	/**
	 * ��������
	 */
	std::string roleName;
	std::string roleIndex;
	/**
	 * ��ȡ��������
	 */
	const char * getAnimationName();

	std::string nowSkill;
	/**
	 * ��ȡ��ǰ����
	 */
	const char * getNowSkill();
	/**
	 * ��ȡ��ɫ����
	 */
	int getDirection();

	RoleStateMangager * getRSM(){ return &rsm; }
	/**
	 * ���õ�ǰ����
	 */
	void setDirection(int direction);

	void update(float dt);

	Sprite * getActionSprite();
	/**
	 * ������Ŀ��Role�ľ���
	 */
	float calcDistance(IRole *role);
	/**
	 * �ƶ��̿��Ƶĳ����ƶ�
	 */
	void doContinueMove();
	/**
	 * ���ٳ����ƶ�
	 */
	void clearContinueMove();

	bool continueMove;

	/**
	 * ��ȡ��������һ����
	 */
	Point getNextPointByDir();

	/**
	 * ����ͬ����Ϣ���ͻ���
	 */
	void doSendMoveMsg(const Point &point);

	int tempID; // ��ʱ���
	/**
	 * ֱ���ƶ���ĳ��
	 */
	void goTo(const Point &point);

	/**
	 * Ѱ·�ƶ���ĳ��
	 */
	void moveTo(const Point &point);
private:
	int direction;

	Sprite *_sprite; // ��ǰ����
public:
	IdleRoleState idleState;	// ����״̬

	WalkRoleState walkState; 	// ��·״̬

	AutoWalkRoleState autoWalkState; // �Զ�Ѱ·״̬

	AttackRoleState attackState; // ����״̬

	BeAttackRoleState beAttackState; // ����״̬
	/**
	* ��ɫ״̬������
	*/
	RoleStateMangager rsm;
	/**
	 * ��ȡ��ǰ״̬��Ӧ�ľ���
	 */
	Sprite * getStateSprite();
};