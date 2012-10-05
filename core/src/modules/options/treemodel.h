#ifndef TREEMODEL_H
#define TREEMODEL_H

#include "../../commonheaders.h"

class TreeItemDelegate : public QStyledItemDelegate
{
public:
	TreeItemDelegate()
	{}

	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
	void paint(QPainter* painter,
			   const QStyleOptionViewItem& option, const QModelIndex& index ) const;
};

class TreeModel : public QAbstractItemModel
{
	Q_OBJECT
private:
	TreeItem*	rootItem;

	TreeItem*	getItem(const QModelIndex& index) const;

public:
	TreeModel(const QString& header = 0, const QString& id = QString(), QWidget* widget = 0,
			  const int index = 0, QObject* parent = 0);
	~TreeModel();

	QModelIndex match(const QModelIndex& startIndex, const QString& header) const;
	QModelIndex index(int row, int column, const QModelIndex& parentIndex = QModelIndex()) const;
	QModelIndex parent(const QModelIndex& itemIndex) const;

	QVariant data(const QModelIndex& itemIndex, int role) const;

	int rowCount(const QModelIndex& parentIndex = QModelIndex()) const;
	int columnCount(const QModelIndex& parentIndex = QModelIndex()) const;

	int getLayoutIndex(const QModelIndex& itemIndex) const;

	Qt::ItemFlags flags(const QModelIndex& itemIndex) const;

	bool setData(const QModelIndex& itemIndex, const QVariant& value, int role = Qt::EditRole);
	bool insert(const QModelIndex& parentIndex = QModelIndex(), QString& header = QString(),
				QString& id = QString(), QWidget* widget = 0, int index = 0);
	bool remove(const QModelIndex& itemIndex = QModelIndex());

};

#endif // TREEMODEL_H
