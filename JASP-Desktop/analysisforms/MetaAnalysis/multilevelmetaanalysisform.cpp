//
// Copyright (C) 2013-2018 University of Amsterdam
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

#include "multilevelmetaanalysisform.h"
#include "ui_multilevelmetaanalysisform.h"

MultiLevelMetaAnalysisForm::MultiLevelMetaAnalysisForm(QWidget *parent) :
		AnalysisForm("MultiLevelMetaAnalysisForm", parent),
		ui(new Ui::MultiLevelMetaAnalysisForm)
{
	ui->setupUi(this);

	ui->listAvailableFields->setModel(&this->_availableVariablesModel);

	_dependentModel = new TableModelVariablesAssigned();
	_dependentModel->setSource(&_availableVariablesModel);
	_dependentModel->setVariableTypesSuggested(Column::ColumnTypeScale);
	_dependentModel->setVariableTypesAllowed(Column::ColumnTypeScale | Column::ColumnTypeNominal | Column::ColumnTypeOrdinal);
	ui->dependent->setModel(_dependentModel);

	QString methods0 = "Fixed Effects,Maximum Likelihood,Restricted ML,DerSimonian-Laird,Hedges,Hunter-Schmidt,Sidik-Jonkman,Empirical Bayes,Paule-Mandel";
	QStringList methods = methods0.split(",");
	ui->method->addItems(methods);

	_covariatesFixedModel = new TableModelVariablesAssigned();
	_covariatesFixedModel->setSource(&_availableVariablesModel);
	_covariatesFixedModel->setVariableTypesAllowed(Column::ColumnTypeScale | Column::ColumnTypeNominal | Column::ColumnTypeOrdinal);
	_covariatesFixedModel->setVariableTypesSuggested(Column::ColumnTypeScale);
	ui->covariatesFixed->setModel(_covariatesFixedModel);

	_factorsFixedModel = new TableModelVariablesAssigned();
	_factorsFixedModel->setSource(&_availableVariablesModel);
	_factorsFixedModel->setVariableTypesSuggested(Column::ColumnTypeNominal | Column::ColumnTypeOrdinal);
	ui->factorsFixed->setModel(_factorsFixedModel);

	_covariatesRandomModel = new TableModelVariablesAssigned();
	_covariatesRandomModel->setSource(&_availableVariablesModel);
	_covariatesRandomModel->setVariableTypesAllowed(Column::ColumnTypeScale | Column::ColumnTypeNominal | Column::ColumnTypeOrdinal);
	_covariatesRandomModel->setVariableTypesSuggested(Column::ColumnTypeScale);
    ui->covariatesRandom->setModel(_covariatesRandomModel);

	_factorsRandomModel = new TableModelVariablesAssigned();
	_factorsRandomModel->setSource(&_availableVariablesModel);
	_factorsRandomModel->setVariableTypesSuggested(Column::ColumnTypeNominal | Column::ColumnTypeOrdinal);
	ui->factorsRandom->setModel(_factorsRandomModel);

	_wlsWeightsModel = new TableModelVariablesAssigned();
	_wlsWeightsModel->setSource(&_availableVariablesModel);
	_wlsWeightsModel->setVariableTypesSuggested(Column::ColumnTypeScale);
	_wlsWeightsModel->setVariableTypesAllowed(Column::ColumnTypeScale | Column::ColumnTypeNominal | Column::ColumnTypeOrdinal);
	ui->wlsWeights->setModel(_wlsWeightsModel);

	_studyLabelModel = new TableModelVariablesAssigned();
	_studyLabelModel->setSource(&_availableVariablesModel);
	_studyLabelModel->setVariableTypesSuggested(Column::ColumnTypeNominal | Column::ColumnTypeOrdinal);
	ui->studyLabels->setModel(_studyLabelModel);


	ui->buttonAssignDependent->setSourceAndTarget(ui->listAvailableFields, ui->dependent);
	ui->buttonAssignCovariatesFixed->setSourceAndTarget(ui->listAvailableFields, ui->covariatesFixed);
	ui->buttonAssignFactorsFixed->setSourceAndTarget(ui->listAvailableFields, ui->factorsFixed);
	ui->buttonAssignCovariatesRandom->setSourceAndTarget(ui->listAvailableFields, ui->covariatesRandom);
	ui->buttonAssignFactorsRandom->setSourceAndTarget(ui->listAvailableFields, ui->factorsRandom);
	ui->buttonAssignWlsWeights->setSourceAndTarget(ui->listAvailableFields, ui->wlsWeights);
	ui->buttonAssignStudyLabels->setSourceAndTarget(ui->listAvailableFields, ui->studyLabels);

	_modelModel = new TableModelAnovaModel(this);
	_modelModel->setPiecesCanBeAssigned(false);
	ui->modelTerms->setModel(_modelModel);
	ui->modelTerms->hide();

	connect(_covariatesRandomModel, SIGNAL(assignmentsChanging()), this, SLOT(factorsChanging()));
	connect(_covariatesRandomModel, SIGNAL(assignmentsChanged()),  this, SLOT(factorsChanged()));
	connect(_covariatesRandomModel, SIGNAL(assignedTo(Terms)), _modelModel, SLOT(addCovariates(Terms)));
	connect(_covariatesRandomModel, SIGNAL(unassigned(Terms)), _modelModel, SLOT(removeVariables(Terms)));

	connect(_factorsRandomModel, SIGNAL(assignmentsChanging()), this, SLOT(factorsChanging()));
	connect(_factorsRandomModel, SIGNAL(assignmentsChanged()),  this, SLOT(factorsChanged()));
	connect(_factorsRandomModel, SIGNAL(assignedTo(Terms)), _modelModel, SLOT(addFixedFactors(Terms)));
	connect(_factorsRandomModel, SIGNAL(unassigned(Terms)), _modelModel, SLOT(removeVariables(Terms)));

	ui->panelStatistics->hide();
	ui->panelIncludeConstant->hide();
	ui->panelAssumptionChecks->hide();
}

MultiLevelMetaAnalysisForm::~MultiLevelMetaAnalysisForm()
{
	delete ui;
}

void MultiLevelMetaAnalysisForm::factorsChanging()
{
	if (_options != NULL)
		_options->blockSignals(true);
}

void MultiLevelMetaAnalysisForm::factorsChanged()
{
	if (_options != NULL)
		_options->blockSignals(false);
}

void MultiLevelMetaAnalysisForm:: bindTo(Options *options, DataSet *dataSet)
{
	AnalysisForm::bindTo(options, dataSet);
	_modelModel->setVariables(Terms(), Terms(), _covariatesFixedModel->assigned());
}
