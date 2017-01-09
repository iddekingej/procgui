#include "config.h"
#include "fieldlist.h"
#include "utils.h"
#include <QVariant>
#include <iostream>

TConfig g_config; 

TConfig::TConfig()
{
}

TConfig::~TConfig()
{
	if(fields != nullptr) delete fields;
}


/**
 * Initialize config =>called in main after setting up QApplication
 * Check if there is a configuration file, if not setup a default.
 */
void TConfig::setup()
{
	config=KSharedConfig::openConfig();
	configGui=config->group("gui");
	KConfigGroup  l_mainGroup=config->group("main");
	QString l_check=l_mainGroup.readEntry("check");
	if(l_check.length()==0){
		QVariantList l_enableDeviceFields;
		for(int l_cnt=0;l_cnt<g_numFields;l_cnt++){
			l_enableDeviceFields.append(l_cnt);
		}
		setFields(l_enableDeviceFields);
		l_mainGroup.writeEntry("check","1");
		config->sync();
	}

}

/**
 *  After each change, call sync to save the data to the configuration file.
 */

void TConfig::sync()
{
	config->sync();
}


/**
 * Returns fields (and order of field) which are displayed in the process list.
 * see fieldlist.h
 * 
 * \return  QVector* with fields id. Don't delete this object=>life cycle of this object is managed by TConfig.
 */
QVector<int>* TConfig::getFields()
{
	if(fields==nullptr){
		QVariantList l_list=configGui.readEntry("fields",QVariantList());
		fields=new QVector<int>(l_list.size());
		QListIterator<QVariant> l_iter(l_list);
		int l_cnt=0;
		while(l_iter.hasNext()){
			(*fields)[l_cnt]=l_iter.next().toInt();
			l_cnt++;
		}
	}
	return fields;
}

/** 
 * \param  list of displayed fields
 */
void TConfig::setFields(QVariantList p_list)
{
	fields=nullptr;
	configGui.writeEntry("fields",p_list);
}

/**
 * When resizing the window, se size is saved in the config file.
 * After (re)starting the progam the size of the main window is restored to this size
 * 
 * \param p_width  - width of the main window
 * \param p_height - height of the main window
 */

void TConfig::setMainWindowSize(int p_width, int p_height)
{
	configGui.writeEntry(QStringLiteral("mainWidth"),p_width );
	configGui.writeEntry(QStringLiteral("mainHeight"),p_height);
}
/**
 * Get Mainwindow height as defined in config. 
 * 
 * \return Height of the main window. Returns -1 when no size set.
 */

int TConfig::getMainWindowHeight()
{
	return configGui.readEntry(QStringLiteral("mainHeight"),-1);
}

/**
 * Get mainwindow height as set in config. 
 * 
 * \return Width of the main window. Returns -1 when not set.
 */

int TConfig::getMainWindowWidth()
{
	return configGui.readEntry(QStringLiteral("mainWidth"),-1);
}

/** 
 *   The proces list can be displayed as a tree or a flat list. 
 *   The checkbox for this option is on the main window. When changing this
 *   option it's saved in the setup file.
 * 
 *   \param p_flag - True display processes as a tree, False as a flat list.
 */

void TConfig::setDisplayAsTree(bool p_flag)
{
	configGui.writeEntry(QStringLiteral("DisplayAsTree"),p_flag);
}


/** 
 *   The proces list can be displayed as a tree or a flat list.  
 * 
 *   \param p_flag - True display processes as a tree, False as a flat list.
 */

bool TConfig::getDisplayAsTree()
{
	return configGui.readEntry(QStringLiteral("DisplayAsTree"),true);
}
