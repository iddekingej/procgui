#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <KSharedConfig>
#include <KConfigGroup>
#include <KConfig>
#include <QVariantList>
#include <QVector>
#include <QVariant>
/*
 * Singelton to manage configuration
 */


class TConfig
{
private:
	KSharedConfig::Ptr config;
	KConfigGroup configGui;
	QVector<int> *fields;
	void setFieldsByVariant(QVariantList &p_list);
public:
	TConfig();
	~TConfig();
	void setup();
	void sync();
	void setFields(QVariantList p_list);
	void setMainWindowSize(int p_width, int p_height);
	int getMainWindowHeight();
	int getMainWindowWidth();
	QVector<int>* getFields();
	void setDisplayAsTree(bool p_flag);
	bool getDisplayAsTree();
};

extern TConfig g_config;

#endif
