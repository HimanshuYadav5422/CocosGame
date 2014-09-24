#include "ui/UIPacker.h"
#include "MyUIPacker.h"
UIPacker * me = NULL;

UIPacker* UIPacker::getInstance()
{
	if (!me) me = new MyUIPacker();
	return me;
}
MyUIPacker * MyUIPacker::getInstance()
{
	if (!me) me = new MyUIPacker();
	return (MyUIPacker*)me;
}

/**
* ��ȡ����
*/
Texture2D * MyUIPacker::getTexutre(const char *name)
{
	return NULL;
}
/**
* ��ȡPlist
*/
bool MyUIPacker::getContent(std::string &content)
{
	MyPack * my = NULL;
	
	if (my)
	{
		MyGroup * group = my->getGrpByPath("");
		if (group)
		{

			MyFileBinary *bin = my->getFileContent("");
			if (bin && bin->content.size())
			{
				content = bin->content;
				bin->unload();
			}
		}
		return false;
	}
	return true;
}