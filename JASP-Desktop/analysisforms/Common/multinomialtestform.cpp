//
// Copyright (C) 2017 University of Amsterdam
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

#include <QDebug>

#include "multinomialtestform.h"
#include "ui_multinomialtestform.h"

MultinomialTestForm::MultinomialTestForm(QWidget *parent) :
	AnalysisForm("MultinomialTestForm", parent),
	ui(new Ui::MultinomialTestForm)
{
	ui->setupUi(this);

	ui->listAvailableVariables->setModel(&_availableVariablesModel);
	ui->listAvailableVariables->setDoubleClickTarget(ui->factor);

	factorModel = new TableModelVariablesAssigned(this);
	factorModel->setSource(&_availableVariablesModel);
	factorModel->setVariableTypesSuggested(Column::ColumnTypeNominal | Column::ColumnTypeOrdinal);
	factorModel->setVariableTypesAllowed(Column::ColumnTypeNominal | Column::ColumnTypeOrdinal | Column::ColumnTypeNominalText);

	ui->factor->setModel(factorModel);
	ui->factor->setDoubleClickTarget(ui->listAvailableVariables);
	ui->assignFactor->setSourceAndTarget(ui->listAvailableVariables, ui->factor);

	TableModelVariablesAssigned *countModel = new TableModelVariablesAssigned(this);
	countModel->setSource(&_availableVariablesModel);
	countModel->setVariableTypesSuggested(Column::ColumnTypeOrdinal | Column::ColumnTypeScale);
	countModel->setVariableTypesAllowed(Column::ColumnTypeNominal | Column::ColumnTypeScale);

	ui->counts->setModel(countModel);
	ui->counts->setDoubleClickTarget(ui->listAvailableVariables);
	ui->assignCounts->setSourceAndTarget(ui->listAvailableVariables, ui->counts);


	TableModelVariablesAssigned *probVarModel = new TableModelVariablesAssigned(this);
	probVarModel->setSource(&_availableVariablesModel);
	probVarModel->setVariableTypesSuggested(Column::ColumnTypeOrdinal | Column::ColumnTypeScale);
	probVarModel->setVariableTypesAllowed(Column::ColumnTypeNominal | Column::ColumnTypeScale);

	ui->exProbVar->setModel(probVarModel);
	ui->exProbVar->setDoubleClickTarget(ui->listAvailableVariables);
	ui->assignExProbVar->setSourceAndTarget(ui->listAvailableVariables, ui->exProbVar);

	// TableModelMultinomialModel *multinomialTable = new TableModelMultinomialModel(this);
	// qDebug() << "setModel";
	//
	// ui->tableView->setModel(multinomialTable);

	connect(factorModel, SIGNAL(assignmentsChanged()), this, SLOT(addFixedFactors()));
	connect(ui->tableWidget, SIGNAL(cellChanged(int, int)), this, SLOT(cellChangedHandler()));

	// Define Multinomial Table
	// ui->tableWidget->setRowCount(2);
	// verticalLabels << "Male" << "Female";
	// ui->tableWidget->setVerticalHeaderLabels(verticalLabels);

	// Initialize Table with one hypothesis
	// addColumnToTable();

	// ui->tableWidget->setColumnCount(1);
	// horizontalLabels << "H1";
	// ui->tableWidget->setHorizontalHeaderLabels(horizontalLabels);

	// QTableWidgetItem *newItem = new QTableWidgetItem(tr("Male").arg((0+1)*(0+1)));
	// ui->tableWidget->setItem(0, 0, newItem);
	//
	// QTableWidgetItem *newItem2 = new QTableWidgetItem(tr("Female").arg((1+1)*(0+1)));
	// ui->tableWidget->setItem(1, 0, newItem2);
}

MultinomialTestForm::~MultinomialTestForm()
{
	delete ui;
}

void MultinomialTestForm::bindTo(Options *options, DataSet *dataSet)
{
	AnalysisForm::bindTo(options, dataSet);

	_dataSet = dataSet;
	// FIXME: If a jasp file is opened which already has factors assigned,
	// the below statement is required to construct the multinomial table
	addFixedFactors();

	// ui->hypothesisTable->set_dataSet(dataSet);

	// factorsChanging();
	Option *factor = options->get("tableWidget");

	// factor->rowTemplate()->get("name")
	qDebug() << "here";

	if (factor != NULL) {
		qDebug() << "woah";
	}

	// factorsChanged();
}

void MultinomialTestForm::cellChangedHandler()
{
	ui->tableWidget->callNotifyChanged();
}

void MultinomialTestForm::addFixedFactors() {

	// qDebug() << "-----------------------------------------";
	// qDebug() << QString::fromStdString(factorModel->assigned().asString());
	// qDebug() << "-----------------------------------------";

	// Clear table contents before updating it with values
	ui->tableWidget->clearContents();
	ui->tableWidget->setRowCount(0);
	ui->tableWidget->setColumnCount(0);

	// Get the column from dataSet
	if (_dataSet != NULL && factorModel->assigned().size() > 0) {
		Labels labels = _dataSet->column(factorModel->assigned().asString()).labels();
		verticalLabels = QStringList();
		int rowCount = 0;

		// labels for the current column
		for (LabelVector::const_iterator it = labels.begin(); it != labels.end(); ++it, rowCount++)
		{
			const Label &label = *it;
			verticalLabels << QString::fromStdString(label.text());
		}

		ui->tableWidget->setRowCount(rowCount);
		ui->tableWidget->setVerticalHeaderLabels(verticalLabels);
		addColumnToTable();
	}

	ui->tableWidget->callNotifyChanged();
}

void MultinomialTestForm::addColumnToTable() {
	int columnCount = ui->tableWidget->columnCount() + 1;

	horizontalLabels << "H" + QString::number(columnCount);
	ui->tableWidget->setColumnCount(columnCount);
	ui->tableWidget->setHorizontalHeaderLabels(horizontalLabels);
	ui->deleteColumn->setEnabled(true);

	// If there are 5 hypotheses, then disable addColumn button
	if (ui->tableWidget->columnCount() == 5) {
		ui->addColumn->setEnabled(false);
	}

	ui->tableWidget->callNotifyChanged();
}

void MultinomialTestForm::deleteColumnFromTable() {
	int currentColumn = ui->tableWidget->currentColumn();

	if (currentColumn == -1) {
		return;
	}
	int columnCount = ui->tableWidget->columnCount();

	ui->tableWidget->removeColumn(currentColumn);
	ui->addColumn->setEnabled(true);

	horizontalLabels = QStringList();
	columnCount = ui->tableWidget->columnCount();

	// Assign the new hypothesis labels
	for (int i = 0; i < columnCount; ++i) {
		horizontalLabels << "H" + QString::number(i + 1);
	}

	ui->tableWidget->setHorizontalHeaderLabels(horizontalLabels);

	if (columnCount == 1) {
		ui->deleteColumn->setEnabled(false);
	}

	ui->tableWidget->callNotifyChanged();
}

void MultinomialTestForm::on_addColumn_clicked(bool checked)
{
	addColumnToTable();
}

void MultinomialTestForm::on_deleteColumn_clicked(bool checked)
{
	deleteColumnFromTable();
}
