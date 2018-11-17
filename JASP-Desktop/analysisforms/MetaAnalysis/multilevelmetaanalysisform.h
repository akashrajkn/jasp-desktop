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

#ifndef MULTILEVELMETANALYSISFORM_H
#define MULTILEVELMETANALYSISFORM_H

#include "../analysisform.h"
#include "widgets/tablemodelvariablesassigned.h"
#include "widgets/tablemodelvariableslevels.h"
#include "widgets/tablemodelanovamodel.h"

namespace Ui {
class MultiLevelMetaAnalysisForm;
}

class MultiLevelMetaAnalysisForm : public AnalysisForm
{
	Q_OBJECT

public:
    explicit MultiLevelMetaAnalysisForm(QWidget *parent = 0);
    ~MultiLevelMetaAnalysisForm();

	void bindTo(Options *options, DataSet *dataSet) OVERRIDE;

private slots:
	void factorsChanging();
	void factorsChanged();

private:
    Ui::MultiLevelMetaAnalysisForm *ui;

	TableModelVariablesAssigned *_dependentModel;
    TableModelVariablesAssigned *_covariatesModel;
    TableModelVariablesAssigned *_factorsModel;
	TableModelVariablesAssigned *_wlsWeightsModel;
    TableModelVariablesAssigned *_studyLabelModel;

	TableModelAnovaModel *_modelModel;
};

#endif // MULTILEVELMETANALYSISFORM_H
