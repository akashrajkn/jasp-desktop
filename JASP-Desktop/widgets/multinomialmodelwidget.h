//
// Copyright (C) 2013-2017 University of Amsterdam
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public
// License along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
//

#ifndef MULTINOMIALMODELWIDGET_H
#define MULTINOMIALMODELWIDGET_H

#include <QWidget>
#include <QStringListModel>

#include <QStringList>

#include <QItemDelegate>
#include <QPainter>
#include <QToolTip>

#include "bound.h"

#include "dataset.h"

#include "options/optionvariables.h"
#include "availablefields.h"

#include "tablemodelvariablesavailable.h"
#include "tablemodelanovamodel.h"


class AnovaHoverDelegate : public QItemDelegate
{
public:
	AnovaHoverDelegate(QObject *parent=0) : QItemDelegate(parent){}
	void paint ( QPainter *painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
	{
		if(option.state & QStyle::State_MouseOver)
		{
			painter->fillRect(option.rect, Qt::lightGray);
			QString str = index.data().toString();
			QToolTip::showText(QCursor::pos(), str);
		}
		QItemDelegate::paint(painter, option, index);
	}
};

namespace Ui {
class MultinomialModelWidget;
}

class MultinomialModelWidget : public QWidget, public Bound
{
	Q_OBJECT

public:
	explicit MultinomialModelWidget(QWidget *parent = 0);
	~MultinomialModelWidget();

	virtual void bindTo(Option *option) OVERRIDE;
	virtual void setModel(TableModelAnovaModel *model);

	void setFactorsLabel(const QString &label);
	void addFixedFactors();
	// void set_dataSet(Dataset *dataSet);

private slots:

	void variablesAvailableChanged();
	void sourceSelectionChanged();

	void on_addColumnButton_clicked(bool checked);
	void on_deleteColumnButton_clicked(bool checked);

private:
	Ui::MultinomialModelWidget *ui;

	OptionsTable *_boundTo;

	// DataSet *_dataSet;
	QStringList verticalLabels;
	QStringList horizontalLabels;

	TableModelVariablesAvailable *_tableModelVariablesAvailable;
	TableModelAnovaModel *_tableModelAnovaModel;
};

#endif // MULTINOMIALMODELWIDGET_H
