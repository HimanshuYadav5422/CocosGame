#include "ui/UIPacker.h"
#include "MyPack.h"
class MyUIPacker :public UIPacker{
public:
	static MyUIPacker * getInstance();
	/**
	* ��ȡ����
	*/
	virtual Texture2D * getTexutre(const char *name);
	/**
	* ��ȡPlist
	*/
	virtual bool getContent(std::string &content);
	

private:
	std::vector<MyPack*> packs;
};