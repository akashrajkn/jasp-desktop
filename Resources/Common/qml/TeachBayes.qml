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

import QtQuick 2.8
import QtQuick.Layouts 1.3
import JASP.Controls 1.0
import JASP.Widgets 1.0


Form
{
	usesJaspResults: false

	Group
	{
		IntegerField { name: "k";	label: qsTr("k")	}
		IntegerField { name: "n";	label: qsTr("n")	}
	}

	Group
	{
		title	: qsTr("Prior Plots")

		CheckBox	{ name: "plotPriorModelProbabilities";	label: qsTr("Prior model probabilities")	}
		CheckBox
		{
			name	: "plotPriorPredictiveFit"
			label	: qsTr("Prior predictive fit")
			PercentField
			{
				name			: "plotPriorPredictiveFitN";
				label			: qsTr("n")
				defaultValue	: 1
				showPercent		: false
			}
		}
	}

	Divider { }

	RadioButtonGroup
	{
		title: qsTr("Hypothesis")
		name: "hypothesis"
		RadioButton { value: "notEqualToTestValue";		label: qsTr("\u2260 Test value"); checked: true	}
		RadioButton { value: "greaterThanTestValue";	label: qsTr("> Test value")						}
		RadioButton { value: "lessThanTestValue";		label: qsTr("< Test value")						}
	}

	Group
	{
		title: qsTr("Plots")
		CheckBox { name: "likelihood";			label: qsTr("Likelihood") }
		CheckBox { name: "marginalLikelihood";	label: qsTr("Marginal likelihood") }
		CheckBox { name: "posterior";			label: qsTr("Posterior") }
	}

	BayesFactorType { }

	Group
	{
		title: qsTr("Prior")
		DoubleField { name: "betaPriorParamA"; label: qsTr("Beta prior: parameter a"); defaultValue: 1 }
		DoubleField { name: "betaPriorParamB"; label: qsTr("Beta prior: parameter b"); defaultValue: 1 }
	}
}
