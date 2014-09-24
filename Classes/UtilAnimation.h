#pragma once
#include "cocos2d.h"
#include "MyPack.h"
#include <bitset>
USING_NS_CC;
class IRole;
/**
 * ������Ϣ
 */
class CollideInfo{
public:
	/**
	 * ��ײ������Ϣ ����Ϊ 4 * 4
	 */
	std::bitset<16> infos; 
	/**
	 * ���ĵ��Ƿ�����Ϣ
	 */
	bool checkPoint(const Point &point);
};
/**
 * ����������Դ FilePacker ����ú� �Ϳ���ֱ�Ӷ�ȡ��
 * ���ļ��д�����
 * PERSONNAME: // ��������
 *		_WALK_4_8/ 8 ���� 4֡
 *		_IDLE_4_8/ 8���� 4֡	  
 */
class CacheAnimation{
public:
	Animation *animation; // ����
	int dirs; // ��������
	CacheAnimation()
	{
		animation = 0;
		dirs = 0;
	}
	AnimationFrame * getFrame(int frame,int dir);
	/**
	 * ��ȡ��ײ��Ϣ
	 */
	CollideInfo * getCollideInfo(int frame, int dir);
};
class UtilParser{
public:
	/**
	 * ��ȡΨһʵ��
	 */
	static UtilParser * getInstance();
	/**
	 * ��ȡ����
	 */
	Animation * getAnimation(const char *name, const char *aniName, int dir,int dirs = 5);

	/**
	 * ��ȡ���涯�� Effect
	 * \param name �ļ�����
	 * \param aniName ��������
	 * \param ���ػ������
	 */
	CacheAnimation getCacheEffectAnimation(const char *name, const char *aniName);
	
	/**
	* ��ȡ���涯�� Animation
	* \param name �ļ�����
	* \param aniName ��������
	* \param ���ػ������
	*/
	CacheAnimation getCacheAnimation(const char *name, const char *aniName);

	/**
	 * ��ȡ��Ч ��out����
	 * \param fileName ���ڵĴ���ļ�
	 * \param aniName ��Ч������
	 * \param dir ��ȡָ���ķ����֡
	 * \param out dir�����Ч�ķ�������
	 * \return ����֡
	 */
	Animation * getEffect(const char *fileName, const char *aniName, int dir, int &dirCount, IPackGetBack * iCall = NULL);

	/**
	 * ͨ�����ֻ�ȡ������
	 */
	bool getDirsByName(int &fileName, int &dirs, const std::string &name);


	/**
	 * Ԥ���ش���ļ�
	 */
	void preLoadPack(const char *fileName, IPackGetBack * iCall = NULL);

	/**
	 * ͨ��pList ��ȡ��������
	 */
	bool getCacheAnimationByPlist(MyPack *my, const char *aniName, MyFileBinary * bin, CacheAnimation & animation);
public:
	// ���涯���Ȳ���
	std::map<std::string, MyPack*> anisPacks; //��������ļ�����
	typedef std::map<std::string, MyPack*>::iterator ANISPACKS;

	std::map<std::string, Animation*> animations;
	typedef std::map<std::string, Animation*>::iterator ANIMATIONS_ITER;
	std::map<std::string, CacheAnimation> cacheAnimations;// �������漯��
	typedef std::map<std::string, CacheAnimation>::iterator CACHEANIMATIONS_ITER; // ������
	static UtilParser * me;
};

class GameDirs{
public:
	enum{
		//ǰ 6�㷽��
		FRONT_DIR = 0,

		//��ǰ 5�㷽��
		FRONT_RIGHT_DIR  = 1,

		//�� 3�㷽��
		RIGHT_DIR  =2,

		//�Һ� 1�㷽��
		BACK_RIGHT_DIR = 3,

		//�� 0�㷽��
		BACK_DIR  = 4,

		//��� 11�㷽��
		BACK_LEFT_DIR = 5,

		//�� 9�㷽��
		LEFT_DIR  = 6,

		//��ǰ 7�㷽��
		FRONT_LEFT_DIR = 7,
	};
	// ����һ��������һ����ķ���,�ѷ��򻮷ֳ�8��
	// ȡֵ��SkillConst�ļ�����ķ���(FRONT_DIR~~FRONT_LEFT_DIR)
	// @param centerx ��ɫվ����λ�õ�X���� 
	// @param centery ��ɫվ����λ�õ�Y����
	// @param calcx ����ĵ��X����
	// @param calcy ����ĵ��Y����
	static int getDirection(int centerx, int centery, int calcx, int calcy)
	{
		//����Math.atan2�ļ�����
		//�ұ�Ϊ0����,���ΪPI����(Ҳ�ɿ�����-PI����),�ϱ�(����)��-PI/2����,�±�(��ǰ��)��PI/2����
		//������Բ�ֳ�16��(2*PI���ȷֳ�16��),ÿ������ռ��2��,����atan2�Ľ���жϷ���
		const float RADIAN_1_16 = 3.1415926 / 8;
		const float RADIAN_RIGHT = 0;
		const float RADIAN_BACK = -3.1415926 * .5;
		const float RADIAN_LEFT = 3.1415926;
		const float RADIAN_FRONT = 3.1415926 * .5;
		int dir = -1;
		float dx = calcx - centerx;
		float dy = -calcy + centery;
		float radians = atan2(dy, dx);
		if (0 > radians)
		{
			if (RADIAN_BACK > radians)
			{
				//RADIAN_BACK��-PI/2,��ȥPI/16
				if ((RADIAN_BACK - RADIAN_1_16) < radians)
				{
					dir = BACK_DIR;
				}
				else if (-(RADIAN_LEFT - RADIAN_1_16) > radians)
				{
					dir = LEFT_DIR;
				}
				else
				{
					dir = BACK_LEFT_DIR;
				}
			}
			else
			{
				if ((RADIAN_BACK + RADIAN_1_16) > radians)
				{
					dir = BACK_DIR;
				}
				else if ((RADIAN_RIGHT - RADIAN_1_16) < radians)
				{
					dir = RIGHT_DIR;
				}
				else
				{
					dir = BACK_RIGHT_DIR;
				}
			}
		}
		else
		{
			//����PI/2�ֳ�����
			//RADIAN_FRONT < radians��ʾλ�����
			if (RADIAN_FRONT < radians)
			{
				//RADIAN_FRONT��PI/2,����PI/16
				if ((RADIAN_FRONT + RADIAN_1_16) > radians)
				{
					dir = FRONT_DIR;
				}
				else if ((RADIAN_LEFT - RADIAN_1_16) < radians)
				{
					dir = LEFT_DIR;
				}
				else
				{
					dir = FRONT_LEFT_DIR;
				}
			}
			else
			{
				if ((RADIAN_FRONT - RADIAN_1_16) < radians)
				{
					dir = FRONT_DIR;
				}
				else if ((RADIAN_RIGHT + RADIAN_1_16) > radians)
				{
					dir = RIGHT_DIR;
				}
				else
				{
					dir = FRONT_RIGHT_DIR;
				}
			}
		}
		return dir;
	}
};

class Tools{
public:
	static bool isClickTheRealSprite(Sprite *p_sprite, const Point& point);
};

/**
 * ��bin�ж�ȡ��Դ
 */
class ReadFromPlistAnimation{
public:
	void parseFromBin(MyPack *my, const char *aniName, MyFileBinary * bin);
	void addSpriteFramesWithDictionary(ValueMap& dictionary, Texture2D* texture);
	bool getAnimtion(CacheAnimation &animation);
private:
	std::vector<SpriteFrame*> _frames;
};