//
// Copyright (C) 2013-2019 University of Amsterdam
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

import QtQuick 2.8
import QtQuick.Layouts 1.3
import JASP.Controls 1.0
import JASP.Widgets 1.0
import JASP.Theme 1.0
import "." as LMM


Form
{
	VariablesForm
	{
		height: Theme.defaultVariablesFormHeight + 200

		AvailableVariablesList { name: "allVariablesList" }
		AssignedVariablesList
		{
			name: "dependent"
			title: qsTr("Dependent Variable")
			singleVariable: true
			suggestedColumns: ["scale"]
		}

		AssignedVariablesList
		{
			name: "fixedCovariates"
			title: qsTr("Fixed Factors and Covariates")
			suggestedColumns: ["scale", "ordinal", "nominal"]
		}

		AssignedVariablesList
		{
			name: "groupingFactors"
			title: qsTr("Grouping Factors")
			suggestedColumns: ["ordinal", "nominal"]
		}

		LMM.REGF
		{
			// name			: "randomEffectsGroupingFactors"
			title			: qsTr("Random Effects\nGrouping Factors")
			height			: 100

			// groupingFactors	: ['groupingFactors']
			// covariates		: ['fixedCovariates']
		}

		// AssignedVariablesList
		// {
		// 	name: "randomFactors"
		// 	titleHeight: Theme.variablesListTitle * 2
		// 	title: qsTr("Random Effects\nGrouping Factors")
		//
		// 	suggestedColumns: ["ordinal", "nominal", "scale"]
		//
		// 	ExtraControlColumn {
		// 		type: "CheckBox"
		// 		name: "intercept"
		// 		title: "Inter.  "
		// 	}
		// 	ExtraControlColumn {
		// 		type: "CheckBox"
		// 		name: "correlation"
		// 		title: "Corr.  "
		// 	}
		// }
	}

	ComboBox
	{
		name: "estimation"
		currentIndex: 2
		label: qsTr("Estimation method")

		model: ListModel
		{
			ListElement { key: "A"; value: "type1"}
			ListElement { key: "B"; value: "type2"}
			ListElement { key: "C"; value: "type3"}
			ListElement { key: "D"; value: "type4"}
		}
	}


	Section
	{
		title: qsTr("Fixed Effects")

		VariablesForm
		{
			height: 200
			AvailableVariablesList
			{
				title: qsTr("Components")
				name: "components"
				source: ["fixedFactors", "fixedCovariates"]
			}
			AssignedVariablesList
			{
				title: qsTr("Model Terms")
				name: "modelTerms"
				listViewType: "Interaction"
			}
		}

		CheckBox { label: qsTr("Test intercept"); name: "testIntercept" }
	}

	Section
	{
		title: qsTr("Random Effects")

		// TODO: Widget similar to repeated measures anova

		VariablesForm
		{
			height: 200
			AvailableVariablesList
			{
				name: "randomEffectsAvailable"
				source: ["fixedFactors", "fixedCovariates"]
			}
			AssignedVariablesList
			{
				name: "randomEffectsChosen"
				listViewType: "Interaction"
			}
		}
	}
}
