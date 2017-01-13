#include "utils.h"
#include <QDir>
#include <QTextStream>
#include <QIODevice>
#include <QStringBuilder>

/**
 * Used for reading files in /sys/block
 * Read file containing ulong (like /sys/block/sda/size)
 * \param p_path  - Path 
 * \param p_name  - File name under path to read
 * \param p_value - Returned value
 * \Return    true when successful  false file reading error
*/

bool readLong(QString p_path,QString p_name,unsigned long &p_value)
{
	QString l_string;
	p_value=0;
	if(!readString(p_path,p_name,l_string)) return false;
	p_value=l_string.toULong();
	return true;
}

/**
 * Used for reading files in /sys/block
 * Read file containing string (like /sys/block/sda/mode)
 * \param p_path  - Path 
 * \param p_name  - File name under path to read
 * \param p_value - Returned value
 * \return        true when successful  false file reading error
*/
bool readString(QString p_path,QString p_name,QString &p_value)
{
	bool l_succes;	
	QFile l_file(p_path%"/"%p_name);
	if(!l_file.open(QIODevice::ReadOnly|QIODevice::Text)){
		p_value= QStringLiteral("");
		return false;
	}
	QTextStream l_stream(&l_file);
	p_value=l_stream.readLine();
	l_succes=(l_stream.status() ==  QTextStream::Ok);
	l_file.close();
	
	return l_succes;
}


/**
 *  When setting a new model, the old model and the selection model needs to be deleted (see qt docs)
 * 
 * \param p_view - View who's model needs to be set.
 * \param p_model - p_view is set with model p_model
 */

void setViewModel(QAbstractItemView *p_view,QStandardItemModel *p_model)
{
	QAbstractItemModel *l_oldModel=p_view->model();
	QItemSelectionModel *l_oldSelection=p_view->selectionModel();
	p_view->setModel(p_model);
	delete l_oldModel;
	delete l_oldSelection;
}
