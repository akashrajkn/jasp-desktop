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

#ifndef LINEARMIXEDMODELSFORM_H
#define LINEARMIXEDMODELSFORM_H

#include "../analysisform.h"

#include "widgets/tablemodelvariablesassigned.h"
#include "widgets/tablemodelanovamodel.h"

namespace Ui {
class LinearMixedModelsForm;
}

class LinearMixedModelsForm : public AnalysisForm
{
	Q_OBJECT

public:
	explicit LinearMixedModelsForm(QWidget *parent = 0);
	~LinearMixedModelsForm();

	virtual void bindTo(Options *options, DataSet *dataSet) OVERRIDE;

private:
	Ui::LinearMixedModelsForm *ui;

	TableModelVariablesAssigned *_dependentListModel;
	TableModelVariablesAssigned *_fixedFactorsListModel;
	TableModelVariablesAssigned *_randomFactorsListModel;
	TableModelVariablesAssigned *_wlsWeightsListModel;
	TableModelVariablesAssigned *_fixedCovariatesListModel;

	TableModelAnovaModel *_anovaModel;

	TableModelVariablesAvailable *_factorsAvailableListModel;

  TableModelVariablesAvailable *_plotFactorsAvailableTableModel;
  TableModelVariablesAssigned *_horizontalAxisTableModel;
  TableModelVariablesAssigned *_seperateLinesTableModel;
  TableModelVariablesAssigned *_seperatePlotsTableModel;

private slots:
	void factorsChanging();
	void factorsChanged();

};

#endif // LINEARMIXEDMODELSFORM_H
