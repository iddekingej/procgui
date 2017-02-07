#ifndef __SORTPROXY_H_
#define __SORTPROXY_H_
#include <QSortFilterProxyModel>
#include <QVector>
#include <QModelIndex>

class TSortProxy:public QSortFilterProxyModel
{
private:
	QVector<int> *fields;
	
	char getSortType(int p_col) const;
protected:
	virtual bool lessThan(const QModelIndex &p_left,const QModelIndex &p_right) const override;
public:
	TSortProxy(QVector<int> *p_fields,QObject *p_parent);
};

#endif