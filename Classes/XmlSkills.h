/**
* �������ļ��м��ؼ�����Ϣ
* xxx.skills ��Ӧ���ܵĴ���ļ� ���н�����������Ϣ
*/
#pragma once
#include "cocos2d.h"
#include <map>
#include <vector>
#include "tinyxml2\tinyxml2.h"
USING_NS_CC;

/**
* ֡������Ϣ
*/
class FrameCtrlInfo{
public:
	int frameID; // ֡���
	int ctrlType; // ��������
	int delayCount; // �ӳ�֡��
	enum{
		JUMP = 0, // ��Ծ
		DELAY = 1,  // ��ʱ
	};
	void parseXml(tinyxml2::XMLElement *element)
	{
		frameID = element->IntAttribute("Frame");
		ctrlType = element->IntAttribute("CtrlType");
		delayCount = element->IntAttribute("DelayCount");
	}
};
/**
 * ��Ч����
 */
class SoundConfig{
public:
	//����ID
	std::string strID;

	//
	bool withSound;

	//
	int playFrame;

	//
	int fadeIn;

	//
	int fadeOut ;

	//����
	int volume;

	//�ظ�
	bool repeat;

	SoundConfig()
	{
		withSound = false;
		playFrame = -1;
		fadeIn = 0;
		fadeOut = 0;
		volume = 0;
		repeat = false;
	}
	void parseXml(tinyxml2::XMLElement *element)
	{
		const char * ID = element->Attribute("SoundID"); 
		if (!ID)
		{
			ID = element->Attribute("ID");
		}
		if (ID) strID = ID;
		withSound = element->BoolAttribute("WithSound");
		const char *PlayFrame = element->Attribute("PlayFrame");
		if (!PlayFrame)  playFrame = -1;
		else playFrame = atoi(PlayFrame);
		fadeIn = element->IntAttribute("FadeIn");
		fadeOut = element->IntAttribute("FadeOut");
		volume = element->IntAttribute("Volume");
		repeat = element->BoolAttribute("Repeat");
	}
};
/**
 * Ч������
 */
class SubEffectConfig{
public:
	/**
	* ��GameObject�����һ����Ϊ���������Ҫ
	* ˳��������ʾ��Դid
	*/
	int useForSort;
	struct OffsetConfig{
		/**
		* λ�û�׼,true:����;false:Ŀ��
		*/
		bool baseOnSelf;

		/**
		* �߶�ƫ��
		*/
		int heightOffset;

		/**
		* λ��ƫ������,false:�ͱ��˷����޹�;true:�ͱ��˷����й�
		*/
		bool relatedWithSelf;

		/**
		* �ͱ��˷����޹ص�ƫ��ֵ
		*/
		struct OffsetNoDirection{
			int offsetX;
			int offsetY;
			Point offset;
			void parseXml(tinyxml2::XMLElement *element)
			{
				offsetX = element->IntAttribute("OffsetX");
				offsetY = element->IntAttribute("OffsetY");
				offset.x = offsetX;
				offset.y = offsetY;
			}
		}offsetNoDir;

		/**
		* �ͱ��˷����йص�ƫ��ֵ
		*/
		struct OffsetInDirection{
			Point currVal;
			std::map<int, Point> offsets; // ��������ϵ�ƫ��
			void parseXml(tinyxml2::XMLElement *element)
			{
				tinyxml2::XMLElement * offsetElement = element->FirstChildElement("Offset");
				while (offsetElement)
				{
					int dir = offsetElement->IntAttribute("Direction");
					Point point;
					point.x = offsetElement->IntAttribute("OffsetX");
					point.y = offsetElement->IntAttribute("OffsetY");
					offsets[dir] = point;
					offsetElement = offsetElement->NextSiblingElement("Offset");
				}
			}
			Point getOffsetWithDir(int dir)
			{
				Point obj;
				if (4 >= dir)
				{
					obj = offsets[dir];
				}
				else
				{
					obj = offsets[8 - dir];
				}
				return obj;
			}
		}offsetInDir;
		void parseXml(tinyxml2::XMLElement *element)
		{
			baseOnSelf = element->BoolAttribute("BaseOnSelf");
			heightOffset = element->IntAttribute("HeightOffset");
			relatedWithSelf = element->BoolAttribute("PosOffsetWithDirection");
			tinyxml2::XMLElement * offsetNoDirection = element->FirstChildElement("OffsetNoDirection");
			if (offsetNoDirection)
			{
				offsetNoDir.parseXml(offsetNoDirection);
			}
			tinyxml2::XMLElement * OffsetInDirection = element->FirstChildElement("OffsetInDirection");
			if (OffsetInDirection)
			{
				offsetInDir.parseXml(OffsetInDirection);
			}
		}
		Point getOffsetWithDir(int dir)
		{
			if (relatedWithSelf)
			{
				return offsetInDir.getOffsetWithDir(dir);
			}
			return offsetNoDir.offset;
		}
	}offsetConfig;
	struct MoveStyleConifg{
		enum{
			MOVE_TYPE_STAY = 0, // �̶�����
			MOVE_TYPE_TOTARGET = 1, // ��Ŀ���ƶ�
			MOVE_TYPE_TODIRECTION = 2, // ��̶������ƶ�
			MOVE_TYPE_CONNECT = 3, // ����Ŀ��������
		};
		int moveType; // �ƶ�����
		//�Թ�������Ϊ�ǶȻ�׼������������Ϊ��׼
		bool faceToDirection;
		//�н�
		float angle;
		//�˶��ٶ�
		float moveSpeed;

		//�˶����ٶ�
		float moveAcceleration;

		//�����˶�������ת
		bool rotateWithMove;

		/**
		* ��xml �н�������
		*/
		void parseXml(tinyxml2::XMLElement *element)
		{
			moveType = element->IntAttribute("MoveType"); 
			faceToDirection = element->BoolAttribute("FaceToDirection");
			angle = element->FloatAttribute("Angle");
			moveSpeed = element->FloatAttribute("MoveSpeed");
			moveAcceleration = element->FloatAttribute("MoveAcceleration");

			moveSpeed = moveSpeed * .001;
			moveAcceleration = moveAcceleration * .000001;

			rotateWithMove = element->BoolAttribute("RotateWithMove");
		}
	}moveStyleConfig;
	struct PlayOrderConfig{
		bool playIndependent;
		void parseXml(tinyxml2::XMLElement *element)
		{
			playIndependent = element->BoolAttribute("PlayIndependent");
		}
	}playOrderConfig; // 
	struct LayerConfig{
		enum{
			LAYER_FOOT = 0, // ��Ч�㼶
			LAYER_CHARA = 1,
		};
		int layer; // ��Ч���ڲ㼶
		bool notTrrigerOnVoid; // �շ�ʱ��������Ч
		void parseXml(tinyxml2::XMLElement *element)
		{
			layer = element->IntAttribute("Layer");
			notTrrigerOnVoid = element->BoolAttribute("NotTrrigerOnVoid");
		}
	}layerConfig; // ������
	struct DeadTypeConfig{
		enum{
			DEAD_TYPE_AFTERDONE = 0, // ������Ϻ�����
			DEAD_TYPE_ATTACK = 1, // ��Ŀ�������
			DEAD_TYPE_FRAMES = 2, // �̶�֡������
			DEAD_TYPE_ATTACKNUM = 3, // ����̶�����������
		};
		int deadType;  //������ʽ
		int deadFrames;   //�̶�֡��
		int attackNum; //�̶��������
		void parseXml(tinyxml2::XMLElement *element)
		{
			deadType = element->IntAttribute("DeadType");
			deadFrames = element->IntAttribute("DeadFrames");
			attackNum = element->IntAttribute("AttackNum");
		}
	}deadTypeConfig; // ��������

	struct HurtTypeConfig{
		enum{
			GENERATE_WHEN_ATTACK = 0, //��ײʱ����
			GENERATE_WHEN_FRAMES = 1,  //�ڹ̶�֡����
		};
		bool genHurt;//�Ƿ�����˺�
		std::string hurtEffectID;//�˺���ЧID
		int generateType; // ������ʽ
		int hurtFrame; //�����˺���֡
		bool shakeScreen;//�˺�ʱ����
		void parseXml(tinyxml2::XMLElement *element)
		{
			genHurt = element->BoolAttribute("GenerateHurt");
			hurtEffectID = element->Attribute("HurtEffectID") ? element->Attribute("HurtEffectID") : "";
			generateType = element->IntAttribute("GenerateType");
			hurtFrame = element->IntAttribute("HurtFrame");
			shakeScreen = element->BoolAttribute("ShakeScreen");
		}
	}hurtTypeConfig; // �˺���ʽ
	struct RotateConfig{
		//����Ч������ת
		bool isEffectCenter ;

		//��ת����ˮƽƫ��
		int rotateOffsetX ;

		//��ת���Ĵ�ֱƫ��
		int rotateOffsetY;

		//��ת���ٶ�
		float rotateSpeed ;

		//��ת���ٶ�
		float rotateAcceleration;

		void parseXml(tinyxml2::XMLElement *element)
		{
			isEffectCenter = element->BoolAttribute("IsEffectCenter");
			rotateOffsetX = element->IntAttribute("RotateOffsetX");
			rotateOffsetY = element->BoolAttribute("RotateOffsetY");
			rotateSpeed = element->FloatAttribute("RotateSpeed");
			rotateAcceleration = element->FloatAttribute("RotateAcceleration");
		}
	}rotateConfig; // ��ת����
	struct GradualChangeConfig{
		struct ChangeValue{
			int x;
			int y;
			int alpha;
		} start,mid,end;
		void parseXml(tinyxml2::XMLElement *element)
		{
			tinyxml2::XMLElement *changeConfig = element->FirstChildElement("Start");
			parseChange(start,changeConfig);
			changeConfig = element->FirstChildElement("Mid");
			parseChange(mid, changeConfig);
			changeConfig = element->FirstChildElement("End");
			parseChange(end, changeConfig);
		}
		void parseChange(ChangeValue &value, tinyxml2::XMLElement *changeConfig)
		{
			value.x = changeConfig->IntAttribute("SizeX");
			value.y = changeConfig->IntAttribute("SizeY");
			value.alpha = changeConfig->IntAttribute("Alpha");
		}
	}gradualChangeConfig; // ��������
	struct FrameControl{
		int preDirFrames; // ÿ�������ϵ�֡��
		std::map<int, FrameCtrlInfo> ctrlInfos;
		void parseXml(tinyxml2::XMLElement *element)
		{
			preDirFrames = element->IntAttribute("PerDirFrames");
			tinyxml2::XMLElement * frameCtrlElement = element->FirstChildElement("FrameControlInfo");
			while (frameCtrlElement)
			{
				FrameCtrlInfo info;
				info.parseXml(frameCtrlElement);
				ctrlInfos[info.frameID] = info;
				frameCtrlElement = frameCtrlElement->NextSiblingElement("FrameControlInfo");
			}
		}
		FrameCtrlInfo * getCtrlInfo(int frame)
		{
			std::map<int, FrameCtrlInfo>::iterator it = ctrlInfos.find(frame);
			if (it != ctrlInfos.end()) return &it->second;
			return NULL;
		}
	}frameControl; // ֡������Ϣ
	SoundConfig soundConfig; // ��Ч����

	void parseXml(tinyxml2::XMLElement *element)
	{
		useForSort = element->IntAttribute("ID");
		tinyxml2::XMLElement* offsetConfigElement = element->FirstChildElement("OffsetConfig");
		if (offsetConfigElement)
		{
			offsetConfig.parseXml(offsetConfigElement);
		}
		tinyxml2::XMLElement* MoveStyleElement = element->FirstChildElement("MoveStyleConfig");
		if (MoveStyleElement)
		{
			moveStyleConfig.parseXml(MoveStyleElement);
		}
		tinyxml2::XMLElement* PlayOrderElement = element->FirstChildElement("PlayOrderConfig");
		if (PlayOrderElement)
		{
			playOrderConfig.parseXml(PlayOrderElement);
		}
		tinyxml2::XMLElement* LayerConfigElement = element->FirstChildElement("LayerConfig");
		if (LayerConfigElement)
		{
			layerConfig.parseXml(LayerConfigElement);
		}
		tinyxml2::XMLElement* DeadTypeElement = element->FirstChildElement("DeadTypeConfig");
		if (DeadTypeElement)
		{
			deadTypeConfig.parseXml(DeadTypeElement);
		}
		tinyxml2::XMLElement* HurtTypeElement = element->FirstChildElement("HurtTypeConfig");
		if (HurtTypeElement)
		{
			hurtTypeConfig.parseXml(HurtTypeElement);
		}
		tinyxml2::XMLElement* RotateConfigElement = element->FirstChildElement("RotateConfig");
		if (RotateConfigElement)
		{
			rotateConfig.parseXml(RotateConfigElement);
		}
		tinyxml2::XMLElement* GradualChangeElement = element->FirstChildElement("GradualChangeConfig");
		if (GradualChangeElement)
		{
			gradualChangeConfig.parseXml(GradualChangeElement);
		}
		tinyxml2::XMLElement* FrameControlElement = element->FirstChildElement("FrameControl");
		if (FrameControlElement)
		{
			frameControl.parseXml(FrameControlElement);
		}
		tinyxml2::XMLElement* SoundConfigElement = element->FirstChildElement("SoundConfig");
		if (SoundConfigElement)
		{
			soundConfig.parseXml(SoundConfigElement);
		}
	}
};
/**
* ��Ч����
*/
class EffectConfig{
public:
	std::vector<SubEffectConfig> subEffects; // ����Ч����
	void parseXml(tinyxml2::XMLElement *element)
	{
		tinyxml2::XMLElement * subEffectElement = element->FirstChildElement("SubEffectConfig");
		while (subEffectElement)
		{
			SubEffectConfig effect;
			effect.parseXml(subEffectElement);
			subEffects.push_back(effect);
			subEffectElement = subEffectElement->NextSiblingElement("SubEffectConfig");
		}
	}
};
/**
 * ��������
 */
struct ActionConfig{
public:
	std::string actionID; // �������
	bool keepImage;
	int totalFrames;
	int perDirFrames;
	std::map<int, FrameCtrlInfo> ctrlInfos;
	void parseXml(tinyxml2::XMLElement *element)
	{
		actionID = element->Attribute("ID") ? element->Attribute("ID"):"";
		keepImage = true;
		const char * keepImageStr = element->Attribute("KeepImage");
		if (keepImageStr && !strcmp(keepImageStr, "false"))
		{
			keepImage = false;
		}
		totalFrames = element->IntAttribute("TotalFrames");
		perDirFrames = element->IntAttribute("PerDirFrames");
		tinyxml2::XMLElement * frameCtrlElement = element->FirstChildElement("FrameControlInfo");
		while (frameCtrlElement)
		{
			FrameCtrlInfo info;
			info.parseXml(frameCtrlElement);
			ctrlInfos[info.frameID] = info;
			frameCtrlElement = frameCtrlElement->NextSiblingElement("FrameControlInfo");
		}
	}
	FrameCtrlInfo * getCtrlInfo(int frame)
	{
		std::map<int, FrameCtrlInfo>::iterator it = ctrlInfos.find(frame);
		if (it != ctrlInfos.end()) return &it->second;
		return NULL;
	}
};
/**
 * ÿ֡������Ϣ
 */
class FrameConfig{
public:
	int frameID; // ֡���
	ActionConfig action; // ����
	SoundConfig soundConfig; // ��Ч����
	std::vector<EffectConfig> effects; // ��Ч����
	void parseXml(tinyxml2::XMLElement *element)
	{
		frameID = element->IntAttribute("Frame");
		// ���ض�������
		tinyxml2::XMLElement*actionElement = element->FirstChildElement("ActConfig");
		if (actionElement)
		{
			action.parseXml(actionElement);
		}
		// ��Ч����
		tinyxml2::XMLElement*soundElement = element->FirstChildElement("SoundConfig");
		if (soundElement)
		{
			soundConfig.parseXml(soundElement);
		}
		
		// ��Ч����
		tinyxml2::XMLElement * effectElement = element->FirstChildElement("EffectConfig");
		while (effectElement)
		{
			EffectConfig effect;
			effect.parseXml(effectElement);
			effects.push_back(effect);
			effectElement = effectElement->NextSiblingElement("EffectConfig");
		}
	}
};
/**
* ����������Ϣ
*/
class SkillConfig{
public:
	int charactorID; // ��ɫ���
	int configID; // ���ܱ��
	struct Attr{
		//˲����ʼ֡
		int teleportFrame;

		//˲����֡��
		int teleportTotalFrame;

		//˲�Ƽ��ٶ�
		int teleportAccelerate;

		//��ǰ˲�Ƶ�֡��
		int currTeleportFrame;

		//��ǰ˲�Ƶ�X������ٶ�
		float teleportSpeedX;

		//��ǰ˲�Ƶ�Y������ٶ�
		float teleportSpeedY;

		//��Ծ��ʼ֡
		int jumpFrame;

		//��Ծ��֡��
		int jumpTotalFrame;

		//��Ծ�߶�
		int jumpHeight;

		//��Ծ���ٶ�
		int jumpAccelerate;

		//��ǰ��Ծ��֡��
		int currJumpFrame;

		//��ǰ��Ծ�ļ��ٶ�
		int currJumpAcce;

		//��ǰ��Ծ�ĳ��ٶ�
		int jumpStartSpeed;

		//��֡��
		int skillTotalFrames;
		
		void parseXml(tinyxml2::XMLElement *element)
		{
			teleportFrame = element->IntAttribute("TeleportFrame");

			teleportTotalFrame = element->IntAttribute("TeleportTotalFrame");

			teleportAccelerate = element->IntAttribute("TeleportAccelerate");

			jumpFrame = element->IntAttribute("JumpFrame");

			jumpTotalFrame = element->IntAttribute("JumpTotalFrame");

			jumpHeight = element->IntAttribute("JumpHeight");

			jumpAccelerate = element->IntAttribute("JumpAccelerate");

			skillTotalFrames = element->IntAttribute("TotalFrames");
		}
	}attrs;
	std::vector<FrameConfig> configLists;
	void parseXml(tinyxml2::XMLElement *element)
	{
		configID = element->IntAttribute("ID");
			
		attrs.parseXml(element);

		tinyxml2::XMLElement * configElement = element->FirstChildElement("ConfigList");
		while (configElement)
		{
			FrameConfig config;
			config.parseXml(configElement);
			configLists.push_back(config);
			configElement = configElement->NextSiblingElement("ConfigList");
		}
	}
};

/**
 * ������Ϣ������
 */
class IPackGetBack;
class SkillConfigs{
public:
	std::map<std::string, SkillConfig*> skills;
	typedef std::map<std::string, SkillConfig*>::iterator SKILLS_ITER;
	/**
	 * ���ַ����н��������ļ�
	 */
	void parseFromString(const char *content){
		tinyxml2::XMLDocument doc;
		doc.Parse(content, strlen(content));
		tinyxml2::XMLElement * charactorElement = doc.FirstChildElement("Charactor");
		const char *ID = charactorElement->Attribute("ID");
		if (!ID) return;
		tinyxml2::XMLElement *skillConfigElemet = charactorElement->FirstChildElement("SkillConfig");
		if (skillConfigElemet)
		{
			SkillConfig * skillConfig = new SkillConfig();
			skillConfig->parseXml(skillConfigElemet);
			CCString uid;
			uid.initWithFormat("%s%d", ID, skillConfig->configID);
			skills[uid.getCString()] = skillConfig;
		}
	}
	SkillConfig * getSkillConfig(const std::string& cid, const std::string& sid);
	/**
	 * �Ӵ���ļ��н�������
	 * ÿ��Pack��Ӧ������
	 */
	void parseFromPack(const char *fileName, IPackGetBack *iBack = NULL);

	static SkillConfigs* getInstance(); // ��ȡ���ܹ������ĵ���
	static SkillConfigs *me; // ���ܹ������ĵ���
};