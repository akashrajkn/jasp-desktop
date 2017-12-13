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
	int columnCount = this->columnCount();
	int rowCount = this->rowCount();

	qDebug() << "-----------------------";

	_groups.clear();

	// Options *newRow = static_cast<Options *>(_boundTo->rowTemplate()->clone());
	std::vector<std::string> verticalHeaders;
	OptionVariables *headers = static_cast<OptionVariables *>(_boundTo->rowTemplate()->clone()->get("factors"));

	// Get verticalHeaders
	for (int i = 0; i < rowCount; ++i) {
		QTableWidgetItem *verticalHeaderItem = this->horizontalHeaderItem(i);
		QString header = "";
		if (verticalHeaderItem != NULL) {
			header = verticalHeaderItem->text();
		}

		verticalHeaders.push_back(fq(header));
	}

	for (int i = 0; i < columnCount; ++i) {
		Options *newRow = static_cast<Options *>(_boundTo->rowTemplate()->clone());  // column is the row here
		OptionString *factorName = static_cast<OptionString *>(newRow->get("name"));
		QString header = "";
		QTableWidgetItem *horizontalHeaderItem = this->horizontalHeaderItem(i);
		if (horizontalHeaderItem != NULL) {
			header = horizontalHeaderItem->text();
		} else {
			// TODO: Revisit this part
			continue;
		}
		factorName->setValue(fq(header));

		OptionVariables *option = static_cast<OptionVariables *>(newRow->get("factor_values"));
		std::vector<std::string> factor_values;
		std::vector<std::string> factors;

		for (int j = 0; j < rowCount; ++j) {
			QTableWidgetItem *rowItem = this->item(j, i);
			QString content = "";

			if (rowItem != NULL) {
				content = rowItem->text();
			}

			factor_values.push_back(fq(content));

			qDebug() << "row " << QString::number(i) << ", " << "column " << QString::number(j) << ":: " << content; // << this->item(i, j)->text();
		}

		option->setValue(factor_values);

		_groups.push_back(newRow);
	}

// 	qDebug() << "------BoundTable constructor------";
// 	Options *newRow = static_cast<Options *>(_boundTo->rowTemplate()->clone());
// 	OptionString *factorName = static_cast<OptionString *>(newRow->get("name"));
// 	factorName->setValue(fq("NewHypothesis"));
//
// 	OptionVariables *option = static_cast<OptionVariables *>(newRow->get("factors"));
// 	std::vector<std::string> test;
// 	test.push_back("alpha");
// 	test.push_back("beta");
// 	option->setValue(test);
//
// 	_groups.push_back(newRow);
// 	_boundTo->setValue(_groups);
// 	_groups.clear();
// 	qDebug() << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$";
	//
	// qDebug() << "-----------------------";

	// size_t i;
	//
	// // hypothesis
	// OptionString *nameTemplateOption = static_cast<OptionString *>(_boundTo->rowTemplate()->get("name"));
	// QString nameTemplate = tq(nameTemplateOption->value());
	//
	// for (i = 0; i < _groups.size(); i++)
	// {
	// 	Options *group = _groups.at(i);
	// 	OptionString *nameOption = static_cast<OptionString *>(group->get("name"));
	// 	std::string oldName = nameOption->value();
	//
	// 	OptionVariables *factorsOption = static_cast<OptionVariables *>(group->get("factors"));
	// 	OptionVariables *valuesOptions = static_cast<OptionVariables *>(group->get("factor_values"));
	// 	// _rows.append(Row(tq(oldName), false, i));
	// 	qDebug() << "oldName = " << tq(oldName);
	//
	// 	std::vector<std::string> factors = factorsOption->variables();
	//
	// 	size_t j;
	// 	for (j = 0; j < factors.size(); j++)
	// 		qDebug() << QString::number(j) << ": " << tq(factors.at(j));
	// 		// _rows.append(Row(tq(factors.at(j)), false, i, j));
	//
	// 	// _rows.append(Row(QString("Level %1").arg(j + 1), true, i, j));
	// }
	//
	// QString name = nameTemplate.arg(i + 1);
	// _rows.append(Row(name, true, i));


	// Options *newRow = static_cast<Options *>(_boundTo->rowTemplate()->clone());
	// OptionString *factorName = static_cast<OptionString *>(newRow->get("name"));
	// factorName->setValue(fq("NewHypothesis"));

	// OptionVariables *option = static_cast<OptionVariables *>(newRow->get("levels"));
	// vector<string> levels = option->variables();

	// _groups.push_back(newRow);

	// FIXME: Below sentence should be used
	_boundTo->setValue(_groups);
}
