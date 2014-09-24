#pragma once
#include "cocos2d.h"
#include "ui/UILoadingBar.h"
#include "ui/UIButton.h"
#include <map>
#include <vector>
#include "tinyxml2\tinyxml2.h"
USING_NS_CC;
/**
 * ����Ԫ��
 */
class UIElement{
public:
	int type; // ����Ԫ������
	void * value;
	enum{
		LOADING_BAR = 1,
		SPRITE =2,
		LABELTTF = 3,
	};
	UIElement()
	{
		type = 0;
		value = NULL;
	}
	Node *toNode(){ return (Node*)value; }
	template<typename T>
	T *to(){ return (T*)value; }
};
/**
 * ���������
 */
class UIPanel:public Node{
public:
	/**
	 * 
	 */
	CREATE_FUNC(UIPanel);
	/**
	 * ͨ��Index ��ѯ����Ԫ��
	 */
	UIElement * getElementByID(int index);

	std::vector<UIElement> elements; // ����Ԫ�ؼ���

	void addElement(int id, UIElement ele);
};
/**
 * һ���ֹ���д�� ����UI ������
 */
class NodeUICreator{
public:
	static NodeUICreator * me;
	static NodeUICreator * getInstance();
	NodeUICreator()
	{
		bindCreateUIFunctions();
	}
	/**
	 * ������������
	 */
	typedef UIElement(NodeUICreator::*CREATE_FUNCTION)(tinyxml2::XMLElement *ele);

	/**
	 * ���ļ��д���UI
	 */
	UIPanel* parseFromFile(const char *dirName, const char *configName);

	/**
	 * �󶨴�������
	 */
	void bindCreateUIFunctions();
	
	/**
	 * ����������
	 */
	UIElement createLoadingBar(tinyxml2::XMLElement *ele);

	/**
	 * ��������
	 */
	UIElement createSprite(tinyxml2::XMLElement *ele);

	/**
	 * �����ı�
	 */
	UIElement createLabelTTF(tinyxml2::XMLElement *ele);
	/**
	 * ͨ�����ִ���Ԫ��
	 * \param name ����
	 * \return ���ؽ���Ԫ��
	 */
	UIElement createElementByName(const std::string &name, tinyxml2::XMLElement *ele);

	/**
	 * ����Ԫ�ش�����Node
	 */
	Node * createChilds(const char *dirName,UIPanel *panel, tinyxml2::XMLElement * nodeElement);
	/*
	 * Ϊ�齨��ֵ
	 **/
	void assignNodePosition(Node *node, UIPanel *panel, tinyxml2::XMLElement * nodeElement);
private:
	std::map<std::string, CREATE_FUNCTION> elements; // ������������
	typedef std::map<std::string, CREATE_FUNCTION>::iterator ELEMENTS_ITER;
};
/**
 * ��ȡ�ļ�
 * ��ȡ����
 */
class NodeUIPackLoader{
public:
		
private:

};