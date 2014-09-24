#pragma once
#include "cocos2d.h"
#include "MyPack.h"
#include "ui/UILoadingBar.h"
/**
 * Ԥ���ػ���
 */
class PreLoaderScene;
class PreLoader:public IPackGetBack{
public:
	std::thread * ithread;
	PreLoaderScene *scene;
	PreLoader()
	{
		ithread = NULL;
		nowSize = 0;
		over = false;
	}
	/**
	 * ��������
	 */
	void go(PreLoaderScene *scene);
	/**
	 * ��ʼ����
	 */
	void loader();
	~PreLoader()
	{
		if (ithread)
		{
			ithread->detach();
			delete ithread;
		}
	}
	void init();
	bool over;
	void backGet(int size, int allSize);
	std::string info;
	int nowSize;
	std::vector<std::string> paths;
};
/**
 * Ԥ���س���
 */
class PreLoaderScene : public cocos2d::Layer{
public:
	static cocos2d::Scene* createScene();
	PreLoaderScene()
	{
		costDt = 0;
	}
	virtual bool init();

	CREATE_FUNC(PreLoaderScene);

	void update(float dt);

	void testListView();
public:
	PreLoader loader;
	cocos2d::LabelTTF *info;
	cocos2d::ui::LoadingBar *bar;
	float costDt;
	bool tag;
};