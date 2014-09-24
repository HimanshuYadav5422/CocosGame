#include "IRoleState.h"
#include "UtilAnimation.h"
#include "Scene.h"
#include "Astar.h"
/**
* �л���ǰ״̬
*/
void RoleStateMangager::chageState(IRoleState *nowState)
{
	if (_states.empty())
	{
		_states.push_back(nowState);
		nowState->Enter();
	}
	else
	{
		IRoleState *state = _states.back();
		_states.pop_back();
		if (state) state->Exit();
		_states.push_back(nowState);
		nowState->Enter();
	}
	
}
/**
* ���浱ǰ״̬
*/
void RoleStateMangager::pushState(IRoleState *nowState)
{
	if (!_states.empty())
	{
		IRoleState *state = _states.back();
		if (state) state->Exit();
		_states.push_back(nowState);
		nowState->Enter();
		
	}
	else
	{
		_states.push_back(nowState);
		nowState->Enter();
	}
	
}
/**
* �ָ���ǰ״̬
*/
IRoleState *RoleStateMangager::popState()
{
	if (!_states.empty())
	{
		IRoleState *state = _states.back();
		if (state) state->Exit();
		_states.pop_back();
		if (_states.empty())
		{
			return NULL;
		}
		state = _states.back();
		if (state) state->Enter();
		return state;
	}
	else
	{
		return NULL;
	}
}

/**
* ��ȡ��ǰ״̬
*/
IRoleState * RoleStateMangager::nowState()
{
	if (!_states.empty())
	{
		IRoleState *state = _states.back();
		return state;
	}
	else
	{
		return NULL;
	}
}

/**
* ������Ϣ
*/
void RoleStateMangager::PasreMsg(Msg *msg)
{
	IRoleState *roleState = nowState();
	if (roleState)
	{
		roleState->PasreMsg(msg);
	}
}
/*****************************************************************************************************************/
// ״̬����
/*****************************************************************************************************************/

void IRoleState::pushMsg(Msg *msg)
{
	Msg *newM = msg->clone();
	if (!newM) return;
	for (MSGBUFFER_ITER iter = msgBuffer.begin(); iter != msgBuffer.end();)
	{
		Msg * old = *iter;
		if (msg->MsgType == old->MsgType)
		{
			iter = msgBuffer.erase(iter);
		}
		else ++iter;
	}
	msgBuffer.push_back(newM);
}
void IRoleState::clearMsgBuffer()
{
	for (MSGBUFFER_ITER iter = msgBuffer.begin(); iter != msgBuffer.end(); ++iter)
	{
		Msg * msg = *iter;
		delete msg;
	}
	msgBuffer.clear();
}

void IRoleState::cartoonOver()
{
	CartoonOverMsg msg;
	PasreMsg(&msg);
}
/*****************************************************************************************************************/
// ��ֹ״̬
/*****************************************************************************************************************/
/**
* �����״̬
* ��ʼ����IDLE����
*/
void IdleRoleState::Enter()
{
	if (_sprite)
	{
		_sprite->stopAction(_action);
		_sprite->setVisible(true);
	}
	else
	{
		_sprite =Sprite::create();
		role->addChild(_sprite);
	}
	if (_sprite)
	{
		int dir = role->getDirection();
		if (dir > 4)
		{
			dir = 8 - dir;
			_sprite->setFlipX(true);
		}
		else
			_sprite->setFlipX(false);
		_action = Animate::create(UtilParser::getInstance()->getAnimation(role->getAnimationName(), "1", dir));
		if (_action)
		{
			Sequence* seq = Sequence::create((Animate*)_action,
				CallFunc::create(this, (SEL_CallFunc)(&IdleRoleState::cartoonOver)),
				NULL);
			_action = seq;
			_sprite->runAction(seq);
		}
		else throw "IdleRoleState::Enter";
	}
}

/**
* �˳�ʱ
* �����״̬
*/
void  IdleRoleState::Exit()
{
	if (_sprite)
	{
		if (_action)
			_sprite->stopAction(_action);
		_sprite->setVisible(false);
		_action = NULL;
	}
}

/**
* ״̬������ ����Ӧ����¼� �����¼� ���������¼�
*/
void IdleRoleState::PasreMsg(Msg * msg)
{
	switch (msg->MsgType)
	{
		case Msg::ATTACK:{
			// ������ 
			AttackMsg *atkMsg = (AttackMsg*)msg;
			role->attackState.target = atkMsg->target;
			role->attackState.targetPosition = atkMsg->point;
			role->getRSM()->pushState(&role->attackState);
		}break;
		case Msg::BE_ATTACK:
		{
			BeAttackMsg * beAttackMsg = (BeAttackMsg*)msg;
			role->beAttackState.beAttacker = beAttackMsg->from;
			role->getRSM()->pushState(&role->beAttackState);
		}break;
		case Msg::TOUCH_MSG:
		{
			TouchMsg *touchMsg = (TouchMsg*)msg;
			// ������
			// �ж��Ƿ���Ҫ�ƶ�
#if (1)
			role->autoWalkState.destination = touchMsg->point;
			role->getRSM()->pushState(&role->autoWalkState); // �л����Զ�Ѱ·����
#else
			role->getRSM()->pushState(&role->attackState);
#endif
		}break; 
		case Msg::CARTOON_OVER:
		{
			// ����������
			if (role->continueMove)
			{
				role->autoWalkState.destination = role->getNextPointByDir();
				role->getRSM()->pushState(&role->autoWalkState); // �л����Զ�Ѱ·����
			}
			else Enter(); // ���¿�ʼ
		}break;
		case Msg::GOTO_MSG:
		{
			GoToMsg * gotoMsg = (GoToMsg*)msg;
			role->walkState.point = MapScene::transPixelPointToGird(gotoMsg->point);
			role->getRSM()->pushState(&role->walkState);
		}break;
		default:
			break;
	}
}

/***********************************************************************************************************/
// �Զ�Ѱ·
/***********************************************************************************************************/
void AutoWalkRoleState::Enter()
{
	if (!mousePoint)
	{
		mousePoint = Sprite::create();
		role->getParent()->addChild(mousePoint,3);

	}
#if (1)
	if (mousePoint)
	{
		mousePoint->stopAllActions();

		action = Repeat::create(Animate::create(UtilParser::getInstance()->getAnimation("misc.anis", "MOUSEPOINT", 0, 1)), -1);
		if (action)
		{
			mousePoint->runAction(action);
		}
	}
#endif
	// �ж��Ƿ񵽵� �����л���WalkState
	if (destination.equals(ccp(0, 0)))
	{
		role->getRSM()->popState();
		mousePoint->setVisible(false);
		mousePoint->stopAllActions();
	}
	else
	{
		mousePoint->setPosition(destination); // ��Ŀ�ĵ�չʾ��
		mousePoint->setVisible(true);
		/**
		 * ��Astar����
		 */
		role->walkState.point = MapScene::transPixelPointToGird(destination);
		
		int dir = GameDirs::getDirection(role->getPositionX(), role->getPositionY(), destination.x, destination.y);
		// ���㷽��
		role->setDirection(dir);
		// ����Ӧ�� 
		role->getRSM()->pushState(&role->walkState);
	}
}
/**
* ��Ӧ����¼�
*/
void AutoWalkRoleState::PasreMsg(Msg * msg)
{
	switch (msg->MsgType)
	{
		case Msg::ATTACK:{
			// ������ 
			role->getRSM()->pushState(&role->attackState);
		}break;
	}
}
/***********************************************************************************************************/
// ��·
/***********************************************************************************************************/
#include "Protos\Command.h"
#include "Network\MyClient.h"
void WalkRoleState::Enter()
{
	// ������·����
	if (!_sprite)
	{
		_sprite = Sprite::create();
		role->addChild(_sprite);
	}
	Point dest = MapScene::transGirdToPixelPoint(point);
	int dir = GameDirs::getDirection(role->getPositionX(), role->getPositionY(), dest.x, dest.y);
	// ���㷽��
	role->setDirection(dir);
	if (_sprite)
	{
		_sprite->setVisible(true);
		int dir = role->getDirection();
		if (dir > 4)
		{
			dir = 8 - dir;
			_sprite->setFlipX(true);
		}
		else
		{
			_sprite->setFlipX(false);
		}
		_animation = UtilParser::getInstance()->getAnimation(role->getAnimationName(), "2", dir);
		if (!_animation)
		{
			throw "IdleRoleState::Enter";
		}
		_animation->retain();
		costDt = 0;
		index = 0;
		stepCostDT = 0;
	}
	// �����ƶ���ص�λ��
	_previousPosition = _startPosition = role->getPosition();
	role->doSendMoveMsg(dest);
	_positionDelta = dest - role->getPosition();
}
void WalkRoleState::Exit()
{
	if (_sprite)
	{
		_sprite->setVisible(false);
		role->autoWalkState.destination = ccp(0, 0);
	}
	if (_animation) _animation->release();
	_animation = NULL;
}

void WalkRoleState::update(float dt)
{
	if (_animation && _sprite)
	{
		costDt += dt; // ֡ʱ�����
		float eachFrameTime = _animation->getDelayPerUnit()/2;
		const Vector<AnimationFrame*>& frames = _animation->getFrames();
		if (costDt >= eachFrameTime )
		{
			index = (index + 1) % frames.size();
			_sprite->setDisplayFrame(frames.at(index)->getSpriteFrame());
			costDt = 0;
		}
		// ����λ��
		float allNeedTime = eachFrameTime * frames.size();
		stepCostDT += dt; // �ƶ�ʱ���ۻ�
		if (allNeedTime)
		{
			Vec2 currentPos = role->getPosition();
			Vec2 diff = currentPos - _previousPosition;
			_startPosition = _startPosition + diff;
			Vec2 newPos = _startPosition + (_positionDelta * stepCostDT);
			role->setPosition(newPos);
			_previousPosition = newPos;
		}
		if (index >= frames.size() - 1 && stepCostDT >= allNeedTime)
		{
			//role->getRSM()->popState();
			CartoonOverMsg msg;
			role->getRSM()->PasreMsg(&msg);
			return;
		}
	}
}
void WalkRoleState::PasreMsg(Msg *msg)
{
	switch (msg->MsgType)
	{
	case Msg::ATTACK:{
		// ������ 
	//	pushMsg(msg);
		AttackMsg *atkMsg = (AttackMsg*)msg;
		role->attackState.target = atkMsg->target;
		role->attackState.targetPosition = atkMsg->point;
		role->getRSM()->pushState(&role->attackState);
	}break;
	case Msg::TOUCH_MSG:
	{
		// ����
		//role->getRSM()->popState();
	}break;
	case Msg::FRAME_UPDATE:
	{
		UpdateFrameMsg *fMsg = (UpdateFrameMsg*)msg;
		update(fMsg->dt);
	}break;
	case Msg::BE_ATTACK:{
		// ���� ��Ҫ������ִ��
		BeAttackMsg *beAtkMsg = (BeAttackMsg*)msg;
		role->beAttackState.beAttacker = beAtkMsg->from;
		role->getRSM()->pushState(&role->beAttackState);
	}break;
	case Msg::GOTO_MSG:{
		GoToMsg *gotoMsg = (GoToMsg*)msg;
		if (role->continueMove)
		{
			role->walkState.point = MapScene::transPixelPointToGird(gotoMsg->point);
			role->autoWalkState.mousePoint->setPosition(role->autoWalkState.destination); // ��Ŀ�ĵ�չʾ��
			Enter();
		}
	}break;
	case Msg::CARTOON_OVER:
	{
		// ���ص���һ��״̬
		if (!role->continueMove)
			role->getRSM()->popState();

		// ������Ϣ����
		for (MSGBUFFER_ITER iter = msgBuffer.begin(); iter != msgBuffer.end(); ++iter)
		{
			Msg *msg = *iter;
			switch (msg->MsgType)
			{
				case Msg::ATTACK:
				{
					role->getRSM()->pushState(&role->attackState);
				}break;
			}
		}
		if (role->continueMove)
		{
			role->autoWalkState.destination = role->getNextPointByDir();
			role->walkState.point = MapScene::transPixelPointToGird(role->autoWalkState.destination);
			role->autoWalkState.mousePoint->setPosition(role->autoWalkState.destination); // ��Ŀ�ĵ�չʾ��
			Enter();
		}
		this->clearMsgBuffer();
	}break;
	default:
		break;
	}
}
/***********************************************************************************************************/
// ����
/***********************************************************************************************************/
void AttackRoleState::Enter()
{
	// ���ܳ�ʼ��
	skill.config = SkillConfigs::getInstance()->getSkillConfig(role->roleIndex.c_str(), role->getNowSkill()); // ���ü��������ļ�
	skill.roleName = role->getAnimationName();
	skill.parent = role;
	skill.me = role;
	skill.target = target;
	skill.targetPosition = targetPosition;
	int dir = GameDirs::getDirection(role->getPositionX(), role->getPositionY(), targetPosition.x, targetPosition.y);
	role->setDirection(dir);
	skill.init();
	skill.setCurrectDir(role->getDirection());
	actionPlayFinish = false;
	if (!_sprite)
	{
		_sprite = Sprite::create();
		role->addChild(_sprite);
	}
	costDt = 0;
}
void AttackRoleState::Exit()
{
	if (_sprite)
	{
		role->removeChild(_sprite);
		_sprite = NULL;
	}
	skill.end();
}
void AttackRoleState::PasreMsg(Msg * msg)
{
	switch (msg->MsgType)
	{
		case Msg::FRAME_UPDATE:
		{
			UpdateFrameMsg *fMsg = (UpdateFrameMsg*)msg;
			update(fMsg->dt);
		}break;
		default:
			break;
	}
}
void AttackRoleState::update(float dt)
{
	// ����ˢ�� 
	if (costDt > 0.05)
	{
		skill.update();

		// ���������� ���л�״̬
		SpriteFrame*frame = skill.getActionFrame();
		if (frame)
		{
			_sprite->setDisplayFrame(frame);
		}
		if (role->getDirection() > 4)
		{
			_sprite->setFlipX(true);
		}	
		costDt = 0;
	}
	if (skill.isOver())
	{
		role->getRSM()->popState();
	}
	costDt += dt; // ֡ʱ�����
}
/***********************************************************************************************************/
// ����
/***********************************************************************************************************/
void BeAttackRoleState::Enter()
{
	// �����ܻ����� ��������
	if (_sprite)
	{
		_sprite->stopAction(_action);
	}
	else
	{
		_sprite = Sprite::create();
		role->addChild(_sprite);
	}
	if (_sprite && beAttacker)
	{
		// ���ݹ����ߵ����ܻ�����
		int beDir = GameDirs::getDirection(role->getPositionX(), role->getPositionY(),beAttacker->getPositionX(), beAttacker->getPositionY());
		role->setDirection(beDir);
		int dir = role->getDirection();
		if (dir > 4)
		{
			dir = 8 - dir;
			_sprite->setFlipX(true);
		}
		Animation *ani = UtilParser::getInstance()->getAnimation(role->getAnimationName(), "3", dir);
		if (ani)
		{
			_action = Animate::create(ani);
			if (_action)
			{
				Sequence* seq = Sequence::create((Animate*)_action,
					CallFunc::create(this, (SEL_CallFunc)(&BeAttackRoleState::cartoonOver)),
					NULL);
				_sprite->runAction(seq);
			}
			else throw "BeAttackRoleState::Enter";
		}
	}
}
/**
 * �����ⲿ�¼�
 */
void BeAttackRoleState::PasreMsg(Msg *msg)
{
	// ������ �и��ʻָ�״̬
	switch (msg->MsgType)
	{
	case Msg::ATTACK:{
		// ������ 
		pushMsg(msg);
	}break;
	case Msg::TOUCH_MSG:
	{
		// ����
	}break;
	case Msg::BE_ATTACK:{
		
	}break;
	case Msg::CARTOON_OVER:
	{
		// ���ص���һ��״̬
		role->getRSM()->popState();

		// ������Ϣ����
	}break;
	default:
		break;
	}
}

void BeAttackRoleState::Exit()
{
	// ���������

	if (_sprite)
	{
		if (_action)
			_sprite->stopAction(_action);
		role->removeChild(_sprite);
		_sprite = NULL;
		_action = NULL;
	}
}


/***********************************************************************************************************/
// ��ɫ
/***********************************************************************************************************/
bool IRole::init()
{
	idleState.role = this;
	autoWalkState.role = this;
	walkState.role = this;
	attackState.role = this;
	beAttackState.role = this;
	this->scheduleUpdate();
#if (0)
	Sprite * rolePoint = Sprite::create();
	Action *action = Repeat::create(Animate::create(UtilParser::getInstance()->getAnimation("misc.anis", "ROLEPOINT", 0,1)), -1);
	if (action)
	{
		rolePoint->runAction(action);
	}
	this->addChild(rolePoint); // �ŵ׹⻷
#endif
	
	return true;
}

void IRole::play()
{
	getRSM()->chageState(&idleState); // ��ʼʱ�л�����ֹ״̬
}
/**
* ���������ص�
*/
void IRole::cartoonOver()
{
	CartoonOverMsg overMsg;
	getRSM()->PasreMsg(&overMsg);
}

int IRole::getDirection()
{
	return this->direction;
}

void IRole::setDirection(int direction)
{
	this->direction = direction;
}

void IRole::update(float dt)
{
	Node::update(dt);

	UpdateFrameMsg uMsg;
	uMsg.dt = dt;
	getRSM()->PasreMsg(&uMsg);
}

Sprite * IRole::getActionSprite()
{
	if (!_sprite) {
		_sprite = Sprite::create();
		this->addChild(_sprite);
	}
	return _sprite;
}


/**
* ��ȡ��������
*/
const char * IRole::getAnimationName()
{
	return roleName.c_str();
}

void IRole::changeAnimation(const char *name)
{
	String info;
	roleIndex = name;
	info.initWithFormat("%s.%s", name, "anis");
	roleName = info.getCString();
}

float IRole::calcDistance(IRole *role)
{
	if (!role) return 0;
	float dist = ccpDistance(getPosition(), role->getPosition());
	return dist;
}

/**
* ��ȡ��ǰ����
*/
const char * IRole::getNowSkill()
{
	return nowSkill.c_str();
}

Sprite * IRole::getStateSprite()
{
	return getRSM()->nowState()->getSprite();
}

void IRole::doContinueMove()
{
	continueMove = true;
}
void IRole::clearContinueMove()
{
	continueMove = false;
}

Point IRole::getNextPointByDir()
{
	Point dest;
	int dirValue = 212;
	int dValue = 176;
	switch (direction)
	{
		case GameDirs::BACK_DIR:
		{
			dest = ccp(0, dirValue);
		}break;
		case GameDirs::LEFT_DIR:
		{
			dest = ccp(-dirValue, 0);
		}break;
		case GameDirs::RIGHT_DIR:
		{
			dest = ccp(dirValue, 0);
		}break;
		case GameDirs::FRONT_DIR:
		{
			dest = ccp(0, -dirValue);
		}break;
		case GameDirs::BACK_LEFT_DIR:
		{
			dest = ccp(-dValue, dValue);
		}break;
		case GameDirs::FRONT_RIGHT_DIR:
		{
			dest = ccp(dValue, -dValue);
		}break;
		case GameDirs::FRONT_LEFT_DIR:
		{
			dest = ccp(-dValue, -dValue);
		}break;
		case GameDirs::BACK_RIGHT_DIR:
		{
			dest = ccp(dValue, dValue);
		}break;
	}
	return ccpAdd(dest,this->getPosition()); 
}

void IRole::doSendMoveMsg(const Point &point)
{
	Terrain * map = SceneManager::getInstance()->nowTerrain;
	if (!map) return;
	Cmd::stUserMoveMoveUserCmd send;
	send.x = point.x / 64;
	send.y = map->mapGridCountSize.height - point.y / 32;
	send.byDirect = getDirection();
	send.bySpeed = 100;
	if (tempID)
	{
		send.dwUserTempID = tempID;
		theClient.sendMsg(&send, sizeof(send));
	}
}

/**
* ֱ���ƶ���ĳ��
*/
void IRole::goTo(const Point &point)
{
	GoToMsg msg;
	msg.point = point;
	getRSM()->PasreMsg(&msg);
}

/**
* Ѱ·�ƶ���ĳ��
*/
void IRole::moveTo(const Point &point)
{

}