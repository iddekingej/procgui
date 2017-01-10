#ifndef _FIELDCONFIG_H_
#define _FIELDCONFIG_H_
#include <QObject>
#include <QDialog>
#include <QStandardItemModel>
#include <KSharedConfig>
#include <KConfigGroup>
#include <KConfig>
#include "ui_fields.h"
#include "src/base/fieldlist.h"

/**
 *  Dialog for setting the columns displayed on in the process list.
 * 
 */

class TFieldsConfig:public QDialog
{
	Q_OBJECT
private slots:
	void save();
	void addLabel();
	void removeLabel();
	void moveUp();
	void moveDown();
private:
	QStandardItemModel *modelAvailable;
	QStandardItemModel *modelSelected;
	Ui::FieldsConfig ui;

	void hideByCode(QListView *p_view,int p_code,bool p_flag);
	void moveItem(int p_dif);
	void enableRemoveButton();
	void fillAvailableList();
	void fillSelectedList();
	int getFAVisibleRow(int p_index);
public:
	TFieldsConfig();
	~TFieldsConfig();
}
;

#endif
