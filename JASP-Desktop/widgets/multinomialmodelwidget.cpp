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

#include "multinomialmodelwidget.h"
#include "ui_multinomialmodelwidget.h"

#include <boost/foreach.hpp>
#include <QMenu>
#include <QDebug>

#include "analysisforms/analysisform.h"

using namespace std;

MultinomialModelWidget::MultinomialModelWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MultinomialModelWidget)
{
	ui->setupUi(this);

	// _boundTo = NULL;
	// _tableModelAnovaModel = NULL;
	//
	// _tableModelVariablesAvailable = new TableModelVariablesAvailable(this);
	// _tableModelVariablesAvailable->setSupportedDragActions(Qt::CopyAction);
	// _tableModelVariablesAvailable->setSupportedDropActions(Qt::MoveAction);
	// _tableModelVariablesAvailable->setMimeType("application/vnd.list.term");
	// ui->listFactorsAvailable->setModel(_tableModelVariablesAvailable);
	// ui->listFactorsAvailable->setDoubleClickTarget(ui->listModelTerms);
	// ui->listFactorsAvailable->selectionUpdated.connect(boost::bind(&MultinomialModelWidget::sourceSelectionChanged, this));
	//
	// ui->listModelTerms->setDoubleClickTarget(ui->listFactorsAvailable);
	// ui->listModelTerms->horizontalHeader()->setStretchLastSection(true);
	//
	// ui->buttonAssignCross->setSourceAndTarget(ui->listFactorsAvailable, ui->listModelTerms);
	// ui->buttonAssignMenu->setSourceAndTarget(ui->listFactorsAvailable, ui->listModelTerms);
	//
	// _assignInteraction = new QAction("Interaction", this);
	// _assignMainEffects = new QAction("Main Effects", this);
	// _assign2ways = new QAction("All 2 way", this);
	// _assign3ways = new QAction("All 3 way", this);
	// _assign4ways = new QAction("All 4 way", this);
	// _assign5ways = new QAction("All 5 way", this);
	//
	// connect(_assignInteraction, SIGNAL(triggered()), this, SLOT(assignInteraction()));
	// connect(_assignMainEffects, SIGNAL(triggered()), this, SLOT(assignMainEffects()));
	// connect(_assign2ways, SIGNAL(triggered()), this, SLOT(assign2ways()));
	// connect(_assign3ways, SIGNAL(triggered()), this, SLOT(assign3ways()));
	// connect(_assign4ways, SIGNAL(triggered()), this, SLOT(assign4ways()));
	// connect(_assign5ways, SIGNAL(triggered()), this, SLOT(assign5ways()));
	//
	// QList<QAction *> actions;
	// actions.append(_assignInteraction);
	// actions.append(_assignMainEffects);
	// actions.append(_assign2ways);
	// actions.append(_assign3ways);
	// actions.append(_assign4ways);
	// actions.append(_assign5ways);
	//
	// QMenu *menu = new QMenu(ui->buttonAssignMenu);
	// menu->addActions(actions);
	// ui->buttonAssignMenu->setMenu(menu);
	//
	// ui->listModelTerms->horizontalHeader()->setDefaultSectionSize(230);
	// ui->listModelTerms->horizontalHeader()->setStretchLastSection(true);
}

MultinomialModelWidget::~MultinomialModelWidget()
{
	delete ui;
}

void MultinomialModelWidget::bindTo(Option *option)
{
	_boundTo = dynamic_cast<OptionsTable *>(option);

	// if (_tableModelAnovaModel != NULL && _boundTo != NULL)
	// {
	// 	_tableModelAnovaModel->bindTo(_boundTo);
	//
	// 	ui->columnLabel0->setText(_tableModelAnovaModel->headerData(0, Qt::Horizontal, Qt::DisplayRole).toString());
	// 	if (_tableModelAnovaModel->columnCount() > 1)
	// 		ui->columnLabel1->setText(_tableModelAnovaModel->headerData(1, Qt::Horizontal, Qt::DisplayRole).toString());
	// }
}

void MultinomialModelWidget::setModel(TableModelAnovaModel *model)
{
	// _tableModelAnovaModel = model;
	//
	// if (_boundTo != NULL)
	// 	_tableModelAnovaModel->bindTo(_boundTo);
	//
	// ui->buttonAssignMenu->setVisible(_tableModelAnovaModel->piecesCanBeAssigned());
	// ui->listModelTerms->setModel(model);
	//
	// variablesAvailableChanged();
	// connect(_tableModelAnovaModel, SIGNAL(variablesAvailableChanged()), this, SLOT(variablesAvailableChanged()));
}

void MultinomialModelWidget::setFactorsLabel(const QString &label)
{
	// ui->factorsLabel->setText(label);
}

void MultinomialModelWidget::variablesAvailableChanged()
{
	// if (_tableModelAnovaModel == NULL)
	// 	return;
	//
	// const Terms &variables = _tableModelAnovaModel->variables();
	// _tableModelVariablesAvailable->setVariables(variables);
}

void MultinomialModelWidget::sourceSelectionChanged()
{
	// int selectedCount = ui->listFactorsAvailable->selectionModel()->selectedIndexes().size();
	//
	// _assignInteraction->setEnabled(selectedCount >= 2);
	// _assignMainEffects->setEnabled(selectedCount >= 1);
	// _assign2ways->setEnabled(selectedCount >= 2);
	// _assign3ways->setEnabled(selectedCount >= 3);
	// _assign4ways->setEnabled(selectedCount >= 4);
	// _assign5ways->setEnabled(selectedCount >= 5);
}

// void MultinomialModelWidget::set_dataSet(Dataset *dataSet)
// {
// 	qDebug() << "MultinomialModelWidget::set_dataSet";
//
// 	// _dataSet = dataSet;
// 	//
// 	// // Re-paint the tableWidget
// 	// addFixedFactors();
// }

void MultinomialModelWidget::addFixedFactors()
{
	qDebug() << "MultinomialModelWidget::addFixedFactors";

	// // Clear table contents before updating it with values
	// ui->tableWidget->clearContents();
	// ui->tableWidget->setRowCount(0);
	// ui->tableWidget->setColumnCount(0);
	//
	// // Get the column from dataSet
	// if (_dataSet != NULL && factorModel->assigned().size() > 0) {
	// 	Labels labels = _dataSet->column(factorModel->assigned().asString()).labels();
	// 	verticalLabels = QStringList();
	// 	int rowCount = 0;
	//
	// 	// labels for the current column
	// 	for (LabelVector::const_iterator it = labels.begin(); it != labels.end(); ++it, rowCount++)
	// 	{
	// 		const Label &label = *it;
	// 		verticalLabels << QString::fromStdString(label.text());
	// 	}
	//
	// 	ui->tableWidget->setRowCount(rowCount);
	// 	ui->tableWidget->setVerticalHeaderLabels(verticalLabels);
	// 	addColumnToTable();
	// }
}

void MultinomialModelWidget::on_addColumnButton_clicked(bool checked)
{
	int columnCount = ui->tableWidget->columnCount() + 1;

	horizontalLabels << "H" + QString::number(columnCount);
	ui->tableWidget->setColumnCount(columnCount);
	ui->tableWidget->setHorizontalHeaderLabels(horizontalLabels);
	ui->deleteColumnButton->setEnabled(true);

	// If there are 5 hypotheses, then disable addColumnButton button
	if (ui->tableWidget->columnCount() == 5) {
		ui->addColumnButton->setEnabled(false);
	}
}

void MultinomialModelWidget::on_deleteColumnButton_clicked(bool checked)
{
	int currentColumn = ui->tableWidget->currentColumn();

	if (currentColumn == -1) {
		return;
	}

	int columnCount = ui->tableWidget->columnCount();

	ui->tableWidget->removeColumn(currentColumn);
	ui->addColumnButton->setEnabled(true);

	horizontalLabels = QStringList();
	columnCount = ui->tableWidget->columnCount();

	// Assign the new hypothesis labels
	for (int i = 0; i < columnCount; ++i) {
		horizontalLabels << "H" + QString::number(i + 1);
	}

	ui->tableWidget->setHorizontalHeaderLabels(horizontalLabels);

	if (columnCount == 1) {
		ui->deleteColumnButton->setEnabled(false);
	}
}
