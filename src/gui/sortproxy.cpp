#include "sortproxy.h"
#include "src/base/fieldlist.h"


TSortProxy::TSortProxy(QVector< int >* p_fields, QObject* p_parent): QSortFilterProxyModel(p_parent)
{
	fields=p_fields;
}

char TSortProxy::getSortType(int p_col) const
{
	if(p_col>=0 && p_col<fields->size()){
		int l_fieldNo=(*fields)[p_col];
		if(l_fieldNo>=0 && l_fieldNo < g_numFields){
			return g_fields[l_fieldNo].type;
		}
	}
	return 'S';
}

/**
 * Uses for sorting. Return true when left value < right value   
 * Checks first if column contains a number. If the display value for left and right is a valid number, values a compared as number
 * otherwise the values are compared as string
 * 
 * @Param p_left    ModelIndex of left value
 * @Param p_right   ModelInex of right value
 * @return   true when left < right , false otherwise
 * */


bool TSortProxy::lessThan(const QModelIndex& p_left, const QModelIndex &p_right) const
{
	char l_type=getSortType(p_left.column());
	bool l_ok;
	if(l_type=='N'){
		int l_left=p_left.data(Qt::DisplayRole).toInt(&l_ok);
		if(l_ok){
			int l_right=p_right.data(Qt::DisplayRole).toInt(&l_ok);
			if(l_ok){
				return l_left<l_right;
			}
		}
	}
	return QSortFilterProxyModel::lessThan(p_left,p_right);
}
