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

#include "boundtablewidget.h"

#include <QDebug>

BoundTableWidget::BoundTableWidget(QWidget *parent)
	: QTableWidget(parent)
{
	_boundTo = NULL;

	// connect(this, SIGNAL(cellChanged()), this, SLOT(callNotifyChanged()));
}

void BoundTableWidget::bindTo(Option *option)
{
	_boundTo = dynamic_cast<OptionsTable *>(option);
	qDebug() << "-----------------------------------------";
	qDebug() << "_boundTo";
	qDebug() << "-----------------------------------------";

	if (_boundTo != NULL) {
		_groups = _boundTo->value();
	}
}

void BoundTableWidget::callNotifyChanged()
{
	qDebug() << "In callNotifyChanged";

	if (_boundTo != NULL) {
		refresh();
		_boundTo->callNotifyChanged();
	}
}

void BoundTableWidget::refresh()
{
	// beginResetModel();

	int columnCount = this->columnCount();
	int rowCount = this->rowCount();

	qDebug() << "-----------------------";
	qDebug() << QString::number(columnCount);
	qDebug() << QString::number(rowCount);

	for (int i = 0; i < rowCount; ++i) {
		for (int j = 0; j < columnCount; ++j) {
			QTableWidgetItem *a = this->item();
			QString content = "None";
			if (a != NULL) {
				qDebug() << "Not None";
				content = a->text();
			}
			qDebug() << "row " << QString::number(i) << ", " << "column " << QString::number(j) << ":: " << content; // << this->item(i, j)->text();
		}
	}

	qDebug() << "-----------------------";

	size_t i;

	// hypothesis
	OptionString *nameTemplateOption = static_cast<OptionString *>(_boundTo->rowTemplate()->get("name"));
	QString nameTemplate = tq(nameTemplateOption->value());

    for (i = 0; i < _groups.size(); i++)
	{
		Options *group = _groups.at(i);
		OptionString *nameOption = static_cast<OptionString *>(group->get("name"));
		std::string oldName = nameOption->value();

		OptionVariables *factorsOption = static_cast<OptionVariables *>(group->get("factors"));
		OptionVariables *valuesOptions = static_cast<OptionVariables *>(group->get("factor_values"));
		// _rows.append(Row(tq(oldName), false, i));
		qDebug() << "oldName = " << tq(oldName);

		std::vector<std::string> factors = factorsOption->variables();

		size_t j;
		for (j = 0; j < factors.size(); j++)
			qDebug() << QString::number(j) << ": " << tq(factors.at(j));
			// _rows.append(Row(tq(factors.at(j)), false, i, j));

		// _rows.append(Row(QString("Level %1").arg(j + 1), true, i, j));
	}

	QString name = nameTemplate.arg(i + 1);
	// _rows.append(Row(name, true, i));

	// endResetModel();
}
