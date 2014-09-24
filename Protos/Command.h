#pragma once
/**
 * �����������
 */
typedef unsigned char uint8;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned long long uint64;


/**
 * ��غ궨��
 */
#ifndef MAX_PASSWORD
#define MAX_PASSWORD	16
#endif

#ifndef MAX_NAMESIZE
#define MAX_NAMESIZE	32
#endif

#ifndef MAX_IP_LENGTH
#define MAX_IP_LENGTH	16
#endif
#pragma pack(1)
/**
 * ��¼ָ��
 **/
namespace Cmd{
	//////////////////////////////////////////////////////////////
	// ��ָ��忪ʼ
	//////////////////////////////////////////////////////////////
	const uint8 NULL_USERCMD_PARA = 0;
	struct stNullUserCmd{
		stNullUserCmd()
		{
			dwTimestamp = 0;
		}
		union{
			struct {
				uint8	byCmd;
				uint8	byParam;
			};
			struct {
				uint8	byCmdType;
				uint8	byParameterType;
			};
		};
		uint32	dwTimestamp;
	};
	////////////////////////////////////////////////////////
	// ��½ָ��忪ʼ
	////////////////////////////////////////////////////////
	const uint8 LOGON_USERCMD = 1;
	struct stLogonUserCmd : public stNullUserCmd
	{
		stLogonUserCmd()
		{
			byCmd = LOGON_USERCMD;
		}
	};
	const uint8 SESSION_KEY_LOGIN = 13;
	struct stSessionKeyLoginCmd : public stLogonUserCmd
	{
		stSessionKeyLoginCmd()
		{
			byParam = SESSION_KEY_LOGIN;
			sessionType = zone = wangtong = 0;
		}
		char pstrName[MAX_NAMESIZE];        /**< �ʺ� */
		uint16 sessionType;               //ƽ̨���� 0�ڲ�ƽ̨
		uint16 zone;                          /**< ��Ϸ����� */
		uint8 wangtong;                   //0���� 1��ͨ
		uint8 isChild;                    //�Ƿ�δ����
	};
	/// ��½ʧ�ܺ󷵻ص���Ϣ
	const uint8 SERVER_RETURN_LOGIN_FAILED = 3;
	struct stServerReturnLoginFailedCmd : public stLogonUserCmd
	{
		stServerReturnLoginFailedCmd()
		{
			byParam = SERVER_RETURN_LOGIN_FAILED;
		}
		uint8 byReturnCode;           /**< ���ص��Ӳ��� */
	};
	/// ��½�ɹ����������ط�������ַ�˿��Լ���Կ����Ϣ
	const uint8 SERVER_RETURN_LOGIN_OK = 4;
	struct stServerReturnLoginSuccessCmd : public stLogonUserCmd
	{
		stServerReturnLoginSuccessCmd()
		{
			byParam = SERVER_RETURN_LOGIN_OK;
		}
		uint32 dwUserID;
		uint32 loginTempID;
		char pstrIP[64];
		uint16 wdPort;
		char key[8];
	};

	/// �ͻ���½���ط����������˺ź�����
	const uint8_t PASSWD_LOGON_USERCMD_PARA = 5;
	struct stPasswdLogonUserCmd : public stLogonUserCmd
	{
		stPasswdLogonUserCmd()
		{
			byParam = PASSWD_LOGON_USERCMD_PARA;
		}

		uint32 loginTempID;
		uint32 dwUserID;
		char pstrName[MAX_NAMESIZE];        /**< �ʺ� */
		uint16 zone;
	};
	//////////////////////////////////////////////////////////////
	// ѡ��ָ��忪ʼ
	//////////////////////////////////////////////////////////////
	const uint8 SELECT_USERCMD = 24;
	struct stSelectUserCmd : public stNullUserCmd
	{
		stSelectUserCmd()
		{
			byCmd = SELECT_USERCMD;
		}
	};
	/// ����ɫ��Ϣ����
	const uint16 MAX_CHARINFO = 1;
	/// ��ɫ��Ϣ
	struct SelectUserInfo
	{
		uint32 id;                        /// ��ɫ���
		char  name[MAX_NAMESIZE + 1];     /// ��ɫ����
		uint8 profession;                 //  ְҵ
		uint8 type;                       /// ��ɫ����
		uint8 sex;                        /// �Ա�
		uint16 level;                     /// ��ɫ�ȼ�
		uint32 mapid;                 /// ��ɫ���ڵ�ͼ���
		char  mapName[MAX_NAMESIZE + 1];  /// ��ɫ���ڵ�ͼ����
		uint16 country;                   /// ����ID
		uint32 bitmask;                   /// ��ɫ����
		char countryName[MAX_NAMESIZE + 1];  /// ��������
	};
	const uint8 USERINFO_SELECT_USERCMD_PARA = 1;
	struct stUserInfoUserCmd : public stSelectUserCmd
	{
		stUserInfoUserCmd()
		{
			byParam = USERINFO_SELECT_USERCMD_PARA;
			memset(charInfo,0,sizeof(charInfo));
			size = 0;
		}
		SelectUserInfo charInfo[MAX_CHARINFO];
		uint16 size;
		uint8 data[0];
		uint16 getLen() { return sizeof(*this) + size*sizeof(data[0]); }
	};
	/// ���󴴽��û�����
	const uint8 CREATE_SELECT_USERCMD_PARA = 2;
	struct stCreateSelectUserCmd : public stSelectUserCmd
	{
		stCreateSelectUserCmd()
		{
			byParam = CREATE_SELECT_USERCMD_PARA;
		}

		char strUserName[MAX_NAMESIZE]; /**< �û�����  */
		uint8 profession;             //ְҵ
		uint8 charType;               //avater����
		uint16 country;               /**< ����ID */
		uint8 sex;                    //�Ա�
	};
	/// �����½
	const uint8 LOGIN_SELECT_USERCMD_PARA = 3;
	struct stLoginSelectUserCmd : public stSelectUserCmd
	{
		stLoginSelectUserCmd()
		{
			byParam = LOGIN_SELECT_USERCMD_PARA;
			asOrClient = 0;
			platformLevel = 0;
		}
		uint8 asOrClient;// as 0 , Client 1
		uint32 charNo;
		uint32 platformLevel;
		char jpegPassport[7];               /**< ͼ����֤�� */
		uint8 browserType;                   //���������
		char flashVersion[32];                  //flash�汾
	};
	/// ����ָ��
	const uint8 DATA_USERCMD = 3;
	struct stDataUserCmd : public stNullUserCmd
	{
		stDataUserCmd()
		{
			byCmd = DATA_USERCMD;
		}
	};
	//�ͻ��˼��ص�ͼ���
	const uint8 LOADMAPOK_DATA_USERCMD_PARA = 29;
	struct stLoadMapOKDataUserCmd : public stDataUserCmd{
		stLoadMapOKDataUserCmd()
		{
			byParam = LOADMAPOK_DATA_USERCMD_PARA;
		}
	};
	//////////////////////////////////////////////////////////////
	/// �û��ƶ�ָ��忪ʼ
	//////////////////////////////////////////////////////////////
	const uint8 MOVE_USERCMD = 6;
	struct stMoveUserCmd : public stNullUserCmd{
		stMoveUserCmd()
		{
			byCmd = MOVE_USERCMD;
		}
	};
	/// �û��ƶ�
	const uint8_t USERMOVE_MOVE_USERCMD_PARA = 2;
	struct stUserMoveMoveUserCmd : public stMoveUserCmd {
		stUserMoveMoveUserCmd()
		{
			byParam = USERMOVE_MOVE_USERCMD_PARA;
		}

		uint32 dwUserTempID;			/**< �û���ʱ��� */

		uint8 byDirect;				/**< �ƶ����� */
		uint8 bySpeed;				/**< �ƶ��ٶ� */
		uint16 x;					/**< Ŀ������ */
		uint16 y;
	};
	/// Npc�ƶ�ָ��
	const uint8 NPCMOVE_MOVE_USERCMD_PARA = 20;
	struct stNpcMoveMoveUserCmd : public stMoveUserCmd{
		stNpcMoveMoveUserCmd()
		{
			byParam = NPCMOVE_MOVE_USERCMD_PARA;
		}

		uint32 dwNpcTempID;			/**< Npc��ʱ��� */

		uint8 byDirect;				/**< �ƶ����� */
		uint8 bySpeed;				/**< �ƶ����ٶ� */
		uint16 x;					/**< Ŀ������ */
		uint16 y;
	};

	/// ��ͼ��Ϣ
	const uint8 MAPSCREENSIZE_DATA_USERCMD_PARA = 28;
	struct stMapScreenSizeDataUserCmd : public stDataUserCmd{
		stMapScreenSizeDataUserCmd()
		{
			byParam = MAPSCREENSIZE_DATA_USERCMD_PARA;
		}

		uint32 width;     /**< ������ */
		uint32 height;        /**< ������ */
		char pstrMapName[MAX_NAMESIZE]; /**< ��½���� */
		uint16 mapID;                 //��ͼid
		uint32 setting;               /**< ��½��־ */
		uint32 rgb;                   /**< ��ɫ��־0x00rrggbb */
		char pstrGroupName[MAX_NAMESIZE];   /**< ������������ */
		char pstrCountryName[MAX_NAMESIZE]; /**< �������� */
		char pstrCityName[MAX_NAMESIZE];    /**< �������� */
		int mainRoleX;
		int mainRoleY;
		uint32 copy;                  //���ڽ��еĸ���
		uint16 forceCountryMap;       //ǿ����ʾ��ͼ
		short npc_count;
		struct {
			uint32 id, x, y;
		} npc_list[0];
		uint16 getLen() { return sizeof(*this) + npc_count*sizeof(npc_list[0]); }
	};
	
	struct t_MainUserData
	{
		uint32 dwUserID;					/// �û�id
		uint32 dwUserTempID;				/// �û���ʱID
		uint16 level;						/// ��ɫ�ȼ�
		uint32 hp;						/// ��ǰ����ֵ
		uint32 maxhp;						/// �������ֵ
		uint32 resumehp;					/// ����ֵ�ָ�
		uint32 mp;						/// ��ǰ����ֵ
		uint32 maxmp;						/// �����ֵ
		uint32 resumemp;					/// ����ֵ�ָ�
		uint32 sp;						/// ��ǰ����ֵ
		uint32 maxsp;						/// �������ֵ
		uint32 resumesp;					/// ����ֵ�ָ�
		uint32 pdamage;					/// ��С��������
		uint32 maxpdamage;				/// �����������
		uint32 mdamage;					/// ��С����������
		uint32 maxmdamage;				/// �����������
		uint32 pdefence;					/// ���������
		uint32 mdefence;					/// ����������
		uint64 exp;						/// ��ǰ����ֵ
		uint64 nextexp;					/// ��������ֵ
		uint16 attackrating;				/// ��������
		uint16 attackdodge;				/// �������
		uint16 bang;						/// ������
		uint16 bangdamage;				/// �����˺�
		uint16 bangdef;					/// ������
		uint16 tili;						/// ����ֵ
		uint16 charm;						/// ����ֵ
		uint8  profession;				/// ְҵ
		uint32 country;					/// ����
		uint16 pkmode;					/// pkģʽ

		uint16 wdTire;					/// ƣ��״̬ 0Ϊ�� 1Ϊƣ��
		uint32 platform;					/// ƽ̨���� 
		uint32 platform2;					/// ƽ̨����2 
		uint32 fivepoint;					/// ���е���
		uint32 honor;						/// ����ֵ
		uint32 maxhonor;					/// �������ֵ
		uint32 money;						/// ���
		uint32 ticket;					/// ��ȯ
		uint32 card;						/// ��
		uint32 bitmask;					/// ��ɫ����
		uint32 weiShouYiZhiExp;			/// β����־����
		uint32 weiShouYiZhiNextExp;		/// β����־�¼�����
		uint8 weiShouYiZhiLevel;			/// β����־�ȼ�
		uint32 weiShouYiZhiMakeObj;		/// β����־���Լӳ�

		uint16 ignorepdef;				/// �����������
		uint16 ignoremdef;				/// ����ħ������
		uint16 twodefence;				/// ���Է���
		uint16 fatalrate;					///	����һ��
		uint16 hitRecovery;				/// ���лظ�
		uint16 dpddam;					/// �����˺�����
		uint16 dmddam;					///	ħ���˺�����
		uint16 reflectp;					/// �˺�����
		uint16 stable;					/// ����
		uint16 stabledef;					/// ������
		uint16 slow;						/// ����
		uint16 slowdef;					/// ���ٿ���
		uint16 reel;						/// ѣ��
		uint16 reeldef;					/// ѣ�ο���
		uint16 chaos;						/// ����
		uint16 chaosdef;					/// ���ҿ���
		uint16 cold;						/// ����
		uint16 colddef;					/// ��������

		uint16 npcdamage;					/// npc�����˺�
		uint32 power;						/// ս����
		uint16 str;						/// ����
		uint16 inte;						/// ����
		uint16 dex;						/// ����

		uint32 dwBodyID;					///	����id 
		uint32 dwCardLevel;				/// �����ȼ� 

		uint16 maxtili;					/// �������ֵ
		uint16 realdam;					/// ��ʵ�˺�
		uint16 realdef;					/// �����˺�

		uint16 attPetCard;				/// ������
		uint32 attPetMode;				/// ���ģʽ

		uint8 vip;						/// vip�ȼ�
		uint16 tiliTime;					/// �����������
		uint16 powerRank;					/// ս��������
		uint16 levelRank;					/// �ȼ�����
		uint16 jingyingCopyTime;			/// ��Ӣ���� 
		uint16 leaderCopyTime;			/// ħ���������
		uint16 moneyTime;					/// �۱������
		uint16 honorTime;					/// ��������
		uint16 starTime;					/// ����ʯ����
		uint16 fumoTime;					/// ������
		uint16 heroTime;					/// Ӣ��ʯ����
		uint16 goldMineTime;				/// �������

		uint8 freeReliveTime;				/// ÿ�����ԭ�ش���
		uint32 vipMoney;					/// ��ֵԪ����
		uint32 vipGiveMoney;				/// vipֵ 

		uint8 loginDay;					/// ��½����
		uint32 ziluolan;					/// ����������
		uint16 pataCopyTime;				/// �����������
		uint16 gvePataCopyTime;			/// GEV�����������

		uint32 starcard;					/// �ǿ�����
		uint32 worldPower;				/// ����ս����
		uint32 monthLoginBit;				/// ��½�콱bit
		uint32 openTime;					/// ��������
		uint32 dayVipMoney;				// ÿ�ճ�ֵ
		uint16 moGuVipTime; //ħ��ɽvip�������
	};

	/// ���û�����
	const uint8 MAIN_USER_DATA_USERCMD_PARA = 1;
	struct stMainUserDataUserCmd : public stDataUserCmd {
		stMainUserDataUserCmd()
		{
			byParam = MAIN_USER_DATA_USERCMD_PARA;
		}
		t_MainUserData data;
	};

	
	//////////////////////////////////////////////////////////////
	/// ��ͼ����ָ��忪ʼ
	//////////////////////////////////////////////////////////////
	const uint8 MAPSCREEN_USERCMD = 5;
	struct stMapScreenUserCmd : public stNullUserCmd{
		stMapScreenUserCmd()
		{
			byCmd = MAPSCREEN_USERCMD;
		}
	};

	struct t_MapBuildingData{
		uint32_t dwMapBuildingTempID;			/**< ���������ʱID */
		uint32_t dwBuildingDataID;				/**< �����������ID */
		uint32_t x;
		uint32_t y;
		uint32_t dwContract;					/**< ������� */
		char pstrName[MAX_NAMESIZE];		/**< ����������� */
		char pstrUserName[MAX_NAMESIZE];	/**< ����������������� */
	};
	enum eSkillPosType
	{
		eSkillPosType_none,
		eSkillPosType_change,
		eSkillPosType_profession,
		eSkillPosType_weishou,
	};

	enum ePickType
	{
		ePickType_all = 1,			//ȫ��
		ePickType_user = 2,			//����
		ePickType_team = 3,			//����
		ePickType_union = 4,		//����
		ePickType_country = 5,		//����
		ePickType_camp = 6,			//��Ӫ
		ePickType_limit = 7, //boss����һ�������ƣ�1�����ڲ���ʰȡ��1���Ӻ�����������ʰȡ

	};

	//////////////////////////////////////////////////////////////
	/// ����ָ��忪ʼ
	//////////////////////////////////////////////////////////////
	/// ��������״̬
	enum {
		// debuff
		USTATE_DEATH = 0,						//	����
		USTATE_POISON = 1,						//	�ж�����ɫ��ȾΪ��ɫ��
		USTATE_ICE = 2,						//	��������ɫ��ȾΪ��ɫ��
		USTATE_FREEZE = 3,						//	���ᣨ��ɫ��ȾΪ��ɫ���޷��ƶ����޷��������޷�ʹ�õ��ߣ�
		USTATE_ROCK = 4,						//	ʯ������ɫ��ȾΪ�ػ�ɫ���޷��ƶ����޷��������޷�ʹ�õ��ߣ�
		USTATE_PALSY = 5,						//	��ԣ���ɫ��ȾΪ����ɫ���޷�������
		USTATE_NO_MOVE = 6,						//  �����ƶ�
		USTATE_STUN = 7,						//  ���ԣ��޷��ƶ����޷�������
		USTATE_SILENT = 8,						//  ��Ĭ��������ʩ���κμ��ܣ�������ͨ������
		USTATE_CONFUSION = 9,						//  ���ң����в����õ���Ч���޷�Ԥ֪��
		USTATE_CRAZY = 10,						//	������в�����Ч�����ҹ�����
		USTATE_BLIND = 11,						//	ʧ������ui��ȫ�ڣ�
		USTATE_SLOW = 12,						//  ����
		USTATE_DEF_DOWN = 13,						//  ����
		USTATE_MDEF_DOWN = 14,						//  ��ħ��
		USTATE_MANA_BURN = 15,						//  ����ȼ��
		USTATE_SATI_HALO = 16,						//  �׼��⻷
		USTATE_ICE_SHIELD = 17,						//  ��������
		USTATE_ICE_BURST = 18,						//  ����
		USTATE_SHOCKWAVE_SLOW = 19,						//  ���������
		USTATE_BLOOD_RING = 20,						//  ��Ѫ�⻷
		USTATE_BLADE_SHAPE = 21,						//  ����
		USTATE_DARK_MIND = 22,						//  �ڰ���ʶ
		USTATE_SOUL_BURN = 23,						//  ���ȼ��
		USTATE_TOXIC_SMOG = 24,						//  ����
		USTATE_PLAGUE = 25,						//  ����
		USTATE_DRAGON_CURSE = 26,						//  ��֮����
		USTATE_CORRIOSION = 27,						//  ��ʴƤ��
		USTATE_HOLLY_SIGN_1 = 28,						//  ��ʥ���
		USTATE_HOLLY_SIGN_2 = 29,						//  ��ʥ���2
		USTATE_BROKEN_BLADE = 30,						//  ����
		USTATE_BETRAYER_CLOAK = 31,						//  �����߶���
		USTATE_CORRIOSION_SKIN = 32,						//  ��ʴƤ��
		USTATE_SLEEP = 33,						//  ˯��״̬

		// buff
		USTATE_HIDE = 80,						//	����
		USTATE_HIDE_NOMOVE_STATE = 81,						//  �������ƶ�״̬
		USTATE_HIDE_PET = 82,						//  ��������״̬
		USTATE_RELIVERING = 83,						//	����⻷
		USTATE_WEISHOU = 84,						//  β�޻�
		USTATE_FURY_MODE = 85,						//  ��ŭ
		USTATE_KEEP_IMAGE = 86,						//  ��Ӱ	
		USTATE_LIQUID_MOVE = 87,						//  �������ж�̬�赲
		USTATE_PDEF_UP = 88,						//  ������
		USTATE_MDEF_UP = 89,						//  ħ�����
		USTATE_HP_UP = 90,						//  Ѫ�����
		USTATE_SPEED_UP = 91,						//  �ٶ����
		USTATE_GOD_MODE = 92,						//  �����·�
		USTATE_REDUCE_DMG = 93,						//	�����˺�
		USTATE_DODGE_UP = 94,					    //  ��������
		USTATE_RENEW_LIFE = 95,					    //  �ָ�����
		USTATE_RASH = 96,						//  ³ç
		USTATE_REDUCE_MDMG = 97,						//  ħ������
		USTATE_DOUBLE_HORNOR = 98,						//  ˫���ɾ�
		USTATE_TRIPLE_HORNOR = 99,						//  �����ɾ�
		USTATE_FOURTH_HORNOR = 100,						//  �ı��ɾ�
		USTATE_FIFTH_HORNOR = 101,						//  �屶�ɾ�
		USTATE_MAGIC_SHILED = 102,						//  ħ����
		USTATE_COURAGE_POWER = 103,						//  ����֮��
		USTATE_DEATH_STORM = 104,						//  �����籩
		USTATE_ICE_BARRIER = 105,						//  ��������
		USTATE_TRUE_GOD_MODE = 106,						//  �������·�
		USTATE_DEVIL_FORCE = 107,						//  ��ħ֮��
		USTATE_TITAN_MARK = 108,						//  ̩̹ӡ��
		USTATE_TWENTYFIFTH_HORNOR = 109,						//  ��ʮ�屶�ɾ�

		// quest
		USTATE_START_QUEST = 150,						//	����ʼ
		USTATE_DOING_QUEST = 151,						//	���������
		USTATE_FINISH_QUEST = 152,						//	�������

		// state
		USTATE_SITDOWN = 180,						//  ����	
		USTATE_PRIVATE_STORE = 181,						//  ��̯
		USTATE_RIDE = 182,						//	���

		// other
		USTATE_TEAM_ATTACK_ONE_DIM = 200,						//  ��Ӽ��й���ͬһ��ɫ״̬

		USTATE_RED_NAME = 444,						//  ����buff

		MAX_STATE = 256                 /// ���״̬��
	};
	enum enumMapDataType{
		MAPDATATYPE_NPC,
		MAPDATATYPE_USER,
		MAPDATATYPE_BUILDING,
		MAPDATATYPE_ITEM,
		MAPDATATYPE_PET
	};
	struct t_MapObjectData {
		uint32 dwMapObjectTempID;				/**< ��Ʒ����ʱID */
		uint32 dwObjectID;                       /**< ��Ʒ������ID */
		char pstrName[MAX_NAMESIZE];			/**<  ��Ʒ�� */
		uint32 x;
		uint32 y;
		uint32 wdNumber;                         /**< ���� */
		uint16  wdLevel;                          /**< �ȼ� */
		uint8 upgrade;
		uint8 kind;
		uint32 dwOwner;
		uint8 pickRange;					//ʰȡ�뾶 ��λ����
		uint8 pickType;
	};

	//���������
	enum petType
	{
		PET_TYPE_NOTPET = 0,	//���ǳ��Ҳ����������ʾ�����б�
		PET_TYPE_RIDE = 1,	//����
		PET_TYPE_PET = 2,	//����
		PET_TYPE_SUMMON = 3,	//�ٻ���
		PET_TYPE_TOTEM = 4,	//ͼ��
		PET_TYPE_GUARDNPC = 5,	//Ҫ������npc
		PET_TYPE_SEMI = 6,    //����������ͬ��ͬ����AI��ȫû��ϵ
		PET_TYPE_CARTOON = 7     //��ͨ���� //TODO NOUSE
	};

	struct t_NpcData {
		uint32 dwMapNpcDataPosition;	/**< npc������λ�� */
		uint32 dwNpcDataID;			/**< npc������ID */
		uint16  movespeed;		/// �ƶ��ٶ�
		uint16 attackspeed;		/// �����ٶ�

		uint8 byDirect;				/**< Npc�ķ��� */
		uint8 level;///�ȼ�
		uint8 campID;				//��Ӫid
		uint32 defType;			//������������eDefType
		uint16 forceAvatar;		//ǿ����ʾnpc
		uint16 pic;
	};
	struct t_MapNpcData :public t_NpcData
	{
		uint8 byState[(MAX_STATE + 7) / 8];			/**< Npc��״̬ */
	};
	struct t_MapPetData {
		uint32 tempID;//npc����ʱID

		char name[MAX_NAMESIZE]; //npc������
		uint8 masterType;	///���˵�����  0����� 1��npc
		uint32 masterID;	///���˵�id -1��û������
		char masterName[MAX_NAMESIZE];///���ε�����
		uint8 pet_type;//��������
	};

	/// ��ͼ����ɾ������
	const uint8 REMOVEUSER_MAPSCREEN_USERCMD_PARA = 11;
	struct stRemoveUserMapScreenUserCmd : public stMapScreenUserCmd{
		stRemoveUserMapScreenUserCmd()
		{
			byParam = REMOVEUSER_MAPSCREEN_USERCMD_PARA;
		}

		uint32 dwUserTempID;			/**< �û���ʱID */
	};
	/// ������Ʒ��������
	enum{
		OBJECTACTION_DROP,			/// ������Ʒ
		OBJECTACTION_UPDATE,		/// ������Ʒ����
	};
	/// �ڵ�ͼ��������Ʒ
	const uint8 ADDMAPOBJECT_MAPSCREEN_USERCMD_PARA = 20;
	struct stAddMapObjectMapScreenUserCmd : public stMapScreenUserCmd{
		stAddMapObjectMapScreenUserCmd()
		{
			byParam = ADDMAPOBJECT_MAPSCREEN_USERCMD_PARA;
			memset(&data,0, sizeof(data));
		}
		uint8	action;
		t_MapObjectData data;
	};

	/// �ڵ�ͼ��ɾ����Ʒ
	const uint8_t REMOVEMAPOBJECT_MAPSCREEN_USERCMD_PARA = 21;
	struct stRemoveMapObjectMapScreenUserCmd : public stMapScreenUserCmd{
		stRemoveMapObjectMapScreenUserCmd()
		{
			byParam = REMOVEMAPOBJECT_MAPSCREEN_USERCMD_PARA;
		}
		uint32 dwMapObjectTempID;	/**< ��Ʒ����ʱID */
		uint32 ownerID;			/**< ��Ʒ���� */
	};

	/// �ڵ�ͼ�����ӽ�����
	const uint8 ADDMAPBUILDING_MAPSCREEN_USERCMD_PARA = 30;
	struct stAddMapBuildingMapScreenUserCmd : public stMapScreenUserCmd{
		stAddMapBuildingMapScreenUserCmd()
		{
			byParam = ADDMAPBUILDING_MAPSCREEN_USERCMD_PARA;
		}

		t_MapBuildingData data;
	};


	/// �ڵ�ͼ��ɾ��������
	const uint8 REMOVEMAPBUILDING_MAPSCREEN_USERCMD_PARA = 31;
	struct stRemoveMapBuildingMapScreenUserCmd : public stMapScreenUserCmd{
		stRemoveMapBuildingMapScreenUserCmd()
		{
			byParam = REMOVEMAPBUILDING_MAPSCREEN_USERCMD_PARA;
		}

		uint32 dwMapBuildingTempID;	/**< ���������ʱID */
	};
	/// �ڵ�ͼ��ɾ��NPC
	const uint8 REMOVEMAPNPC_MAPSCREEN_USERCMD_PARA = 51;
	struct stRemoveMapNpcMapScreenUserCmd : public stMapScreenUserCmd{
		stRemoveMapNpcMapScreenUserCmd()
		{
			byParam = REMOVEMAPNPC_MAPSCREEN_USERCMD_PARA;
		}
		uint32 dwMapNpcDataPosition;	/**< npc������λ�� */
	};
	struct MapData_ItemHeader {
		uint16 size;// ����
		uint8 type;// ����  enumMapDataType
	};
	/// �û���������ݣ�������״̬
	struct t_MapUserSculpt{
		uint32 dwHorseID;					/// ���Ʊ��
		uint32 dwStarSet;					/// ���׼���
		uint32 dwBodyID;					///	����id 
		uint32 dwCardLevel;				/// �����Ǽ� 
		uint32 dwRightHandID;				/// ������ƷID
	};
	struct t_UserData
	{
		uint32 dwUserTempID;					//// �û���ʱID

		char  name[MAX_NAMESIZE + 1];			/// ��ɫ����
		uint8 chartype;						/// avrtar����
		uint8  profession;					/// ְҵ����
		uint8  sex;							/// �Ա�
		uint32 goodness;						/// �ƶ��

		t_MapUserSculpt sculpt;
		uint32 dwHairRGB;					/// ͷ����ɫ0xffrrggbb
		uint32 dwBodyColorSystem;			/// �·�ϵͳ��ɫ0xffrrggbb
		uint32 dwBodyColorCustom;			/// �·�������ɫ0xffrrggbb
		uint32 dwLeftWeaponColor;			/// ����������ɫ0xffrrggbb
		uint32 dwRightWeaponColor;			/// ����������ɫ0xffrrggbb
		uint16  attackspeed;					/// �����ٶ�
		uint16  movespeed;					/// �ƶ��ٶ�
		uint8 country;						/// ����
		uint32 dwChangeFaceID;				//���ݺ��NPC������ID
		//uint8_t live_skills[6]; 				//����ܵȼ�
		uint8 level;							//Level : 1(<10)   11(>=10)
		uint32 exploit;				// ��ѫֵ

		uint32 dwUnionID;			// ���ID
		uint32 dwSeptID;				// ����ID

		char  caption[MAX_NAMESIZE];		// ����ͷ�Σ�ĳ�ǳ�����ĳ��������
		uint32 dwTeamState;					// ����״̬
		uint32 dwArmyState;			//1Ϊ�ӳ���2Ϊ����,0Ϊδ�������

		uint8 TeamNum;					//�����Ա���� 
		uint8 autoIntoTeam;				//�Ƿ��������Զ����
		uint8 campID;					//��Ӫid
		uint32 hp;					//��ǰѪ
		uint32 maxhp;					//Ѫ������

		uint8 pkMode;					//pkģʽ
		uint32 teamId;				//���id

		uint32 cardSystemLevel;		//��̳�ȼ�
		uint8 countryPower[4];			//�û�����Ȩ��
		uint32 platformIDLevel;			// ƽ̨�ȼ�

	};
	struct t_MapUserData : public t_UserData
	{
		uint8_t state[(MAX_STATE + 7) / 8];	/// ��ɫ״̬
	};

	struct t_MapUserDataPos :public t_MapUserData
	{
		uint16_t x;					/**< ����*/
		uint16_t y;
		uint8_t byDir;					// �������Ϊ -1 ��ȷ������
	};
	struct t_MapNpcDataPos :public t_MapNpcData
	{
		uint16_t x;					/**< ����*/
		uint16_t y;
		uint8_t byDir;					// �������Ϊ -1 ��ȷ������
	};
	struct t_MapBuildingDataPos :public t_MapBuildingData
	{
		uint16_t x;					/**< ����*/
		uint16_t y;
		uint8_t byDir;					// �������Ϊ -1 ��ȷ������
	};
	// ��ͼ���ݴ������
	const uint8 MAPDATA_MAPSCREEN_USERCMD_PARA = 55;
	struct stMapDataMapScreenUserCmd : public stMapScreenUserCmd{
		stMapDataMapScreenUserCmd()
		{
			byParam = MAPDATA_MAPSCREEN_USERCMD_PARA;
			mdih.size = 0;
			mdih.type = 0;
		}
		MapData_ItemHeader mdih;
		union
		{
			t_MapUserDataPos mud[0];
			t_MapNpcDataPos mnd[0];
			t_MapBuildingDataPos mbd[0];
			t_MapPetData mpd[0];
		};
		uint16_t getLen()
		{
			if (mdih.type == MAPDATATYPE_USER)
				return sizeof(*this) + mdih.size*sizeof(mud[0]);
			if (mdih.type == MAPDATATYPE_NPC)
				return sizeof(*this) + mdih.size*sizeof(mnd[0]);
			if (mdih.type == MAPDATATYPE_BUILDING)
				return sizeof(*this) + mdih.size*sizeof(mbd[0]);
			if (mdih.type == MAPDATATYPE_PET)
				return sizeof(*this) + mdih.size*sizeof(mpd[0]);
			return 0;
		}
	};

#define SELECTED_HPMP_PROPERTY_USERCMD_PARA 56//֪ͨѡ���Լ������hp��mp�ı仯
	struct stRTSelectedHpMpPropertyUserCmd : public stMapScreenUserCmd
	{
		stRTSelectedHpMpPropertyUserCmd()
		{
			byParam = SELECTED_HPMP_PROPERTY_USERCMD_PARA;
		}
		uint8 byType;//enumMapDataType
		uint32 dwTempID;//��ʱ���
		uint64 dwHP;//��ǰѪ
		uint64 dwMaxHp;//���hp
		uint32 dwMP;//��ǰmp
		uint32 dwMaxMp;//���mp
	};
	/// �����ͼ����Ʒ����
	const uint8 CLEAR_OBJECTOWNER_MAPSCREEN_USERCMD_PARA = 58;
	struct stClearObjectOwnerMapScreenUserCmd : public stMapScreenUserCmd{
		stClearObjectOwnerMapScreenUserCmd()
		{
			byParam = CLEAR_OBJECTOWNER_MAPSCREEN_USERCMD_PARA;
		}
		uint32 dwMapObjectTempID;	/**< ��Ʒ����ʱID */
	};
	/// ����ɾ��NPCָ��
	const uint8 BATCHREMOVENPC_MAPSCREEN_USERCMD_PARA = 61;
	struct stBatchRemoveNpcMapScreenUserCmd : public stMapScreenUserCmd {
		stBatchRemoveNpcMapScreenUserCmd()
		{
			byParam = BATCHREMOVENPC_MAPSCREEN_USERCMD_PARA;
		}
		uint16	num;
		uint32   id[0];
		uint16 getLen() { return sizeof(*this) + num*sizeof(id[0]); }
	};

	/// ����ɾ��USERָ��
	const uint8 BATCHREMOVEUSER_MAPSCREEN_USERCMD_PARA = 62;
	struct stBatchRemoveUserMapScreenUserCmd : public stMapScreenUserCmd {
		stBatchRemoveUserMapScreenUserCmd()
		{
			byParam = BATCHREMOVEUSER_MAPSCREEN_USERCMD_PARA;
		}
		uint16	num;
		uint32   id[0];
		uint16 getLen() { return sizeof(*this) + num*sizeof(id[0]); }
	};

	/// ��ͼ���������������
	const uint8 ADDUSER_AND_POS_MAPSCREEN_STATE_USERCMD_PARA = 70;
	struct stAddUserAndPosMapScreenStateUserCmd : public stMapScreenUserCmd {
		stAddUserAndPosMapScreenStateUserCmd()
		{
			byParam = ADDUSER_AND_POS_MAPSCREEN_STATE_USERCMD_PARA;
			forceMove = 0;
		}
		t_MapUserDataPos data;
		uint8 forceMove;
	};
	/// �ڵ�ͼ������NPC������
	const uint8 ADDMAPNPC_AND_POS_MAPSCREEN_STATE_USERCMD_PARA = 71;
	struct stAddMapNpcAndPosMapScreenStateUserCmd : public stMapScreenUserCmd{
		stAddMapNpcAndPosMapScreenStateUserCmd()
		{
			byParam = ADDMAPNPC_AND_POS_MAPSCREEN_STATE_USERCMD_PARA;
			type = 0;
		}
		t_MapNpcDataPos data;
		uint8 type; //0������ 1����
	};

	//////////////////////////////////////////////////////////////
	/// ħ��ָ��忪ʼ
	//////////////////////////////////////////////////////////////
	const uint8 MAGIC_USERCMD = 18;
	struct stMagicUserCmd : public stNullUserCmd{
		stMagicUserCmd()
		{
			byCmd = MAGIC_USERCMD;
		}
	};
	/// ���幥������
	enum ATTACKTYPE{
		ATTACKTYPE_U2U, /// �û������û�
		ATTACKTYPE_U2N, /// �û�����Npc
		ATTACKTYPE_N2U, /// Npc�����û�
		ATTACKTYPE_U2B, /// �û���������
		ATTACKTYPE_U2P, /// �û�������
		ATTACKTYPE_N2N  /// Npc����Npc
	};
	/// ����ָ��
	const uint8 MAGIC_USERCMD_PARA = 5;
	struct stAttackMagicUserCmd : public stMagicUserCmd{

		stAttackMagicUserCmd()
		{
			byParam = MAGIC_USERCMD_PARA;
			dwUserTempID = dwDefenceTempID = wdMagicType = xDes = yDes = byDirect = byAttackType = byAction = bySeq = num = 0;
			xPixel = 0;
			yPixel = 0;
		}
		uint16 getLen() { return sizeof(*this) + num*sizeof(dwTempIDList[0]); }

		//BEGIN_MUTABLE_VARS
		uint32 dwUserTempID;          /**< ������ʱ��� */
		uint32 dwDefenceTempID;       /**< ������ʱ��� */

		uint16 wdMagicType;           /**< ħ������ */

		uint16 xDes;                  /**< Ŀ��� */
		uint16 yDes;                  /**< Ŀ��� */
		uint32 xPixel;                //��������x
		uint32 yPixel;                //��������y
		//END_MUTABLE_VARS

		uint8 bySeq;                  //��������

		uint8 byDirect;               /**< �������� */
		uint8 byAttackType;           /**< �������ͣ�ATTACKTYPE_U2U,ATTACKTYPE_U2N,ATTACKTYPE_N2U */
		uint8 byAction;               // ��������  AniTypeEnum ( Ani_Null Ϊ��ȷ���ģ��ɿͻ���ȷ�� )
		uint8 num;
		uint32 dwTempIDList[0];       // ����Ŀ���б�
	};
};

#pragma pack()