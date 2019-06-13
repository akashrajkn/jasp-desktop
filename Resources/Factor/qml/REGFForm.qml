import QtQuick			2.11
import QtQuick.Controls	2.4
import JASP.Widgets		1.0
import JASP.Controls	1.0
import JASP.Theme		1.0
import QtQuick.Layouts  1.3
import "." as SEM

JASPControl
{
	id:					    factorsForm
	controlType:		    "FactorsForm"
	implicitWidth:	        parent.width
	height:			        Theme.defaultVariablesFormHeight
	implicitHeight:         height
	useControlMouseArea:    false

	property var	model
	property string availableVariablesListName: "allAvailableVariables"
	property alias	availableVariablesList: availableVariablesList
	property bool   allowAll: false
	property int	initNumberFactors: 1
	property int    listWidth:			parent.width * 2 / 5
	property int    factorListHeight: Theme.defaultSingleItemListHeight * 2

	signal titleChanged(int index, string title);
	signal factorAdded(int index, var item);

	VariablesList
	{
		id:				availableVariablesList
		name:			availableVariablesListName
		width:			listWidth
		height:			parent.height
		anchors.top:	parent.top
		anchors.left:	parent.left
	}

	Column
	{
		id:             factorsFormColumn
		spacing:        10
		width:          parent.width * 3 / 5
		anchors.top:	parent.top
		anchors.right:	parent.right

		RowLayout
		{
			AssignButton
			{
				id: button1
				Layout.leftMargin:  (factorsFormColumn.width / 3 - width) / 2
				Layout.rightMargin: (factorsFormColumn.width / 3 - width) / 2
				leftSource:         factorsForm.availableVariablesList
				rightSource:        dependent
			}

			VariablesList
			{
				implicitWidth:      listWidth
				listViewType: "AssignedVariables"
				dropMode: "Insert"
				id: dependent
				name: "dependent"
				title: qsTr("Dependent Variable")
				singleVariable: true
				suggestedColumns: ["scale"]

				Component.onCompleted:
				{
					activeFocusChanged.connect(button1.setIconToLeft);
					availableVariablesList.activeFocusChanged.connect(button1.setIconToRight);
					hasSelectedItemsChanged.connect(button1.setState);
					availableVariablesList.hasSelectedItemsChanged.connect(button1.setState);
				}
			}
		}

		RowLayout
		{
			AssignButton
			{
				id: button2
				Layout.leftMargin:  (factorsFormColumn.width / 3 - width) / 2
				Layout.rightMargin: (factorsFormColumn.width / 3 - width) / 2
				leftSource:         factorsForm.availableVariablesList
				rightSource:        groupingFactors
			}

			VariablesList
			{
				implicitWidth:      listWidth
				listViewType: "AssignedVariables"
				dropMode: "Insert"
				height: Theme.defaultSingleItemListHeight * 2
				id: groupingFactors
				name: "groupingFactors"
				title: qsTr("Grouping Factors")
				suggestedColumns: ["ordinal", "nominal"]

				Component.onCompleted:
				{
					activeFocusChanged.connect(button2.setIconToLeft);
					availableVariablesList.activeFocusChanged.connect(button2.setIconToRight);
					hasSelectedItemsChanged.connect(button2.setState);
					availableVariablesList.hasSelectedItemsChanged.connect(button2.setState);
					// factorsForm.factorAdded(index, factorList);
					// factorsForm.addFactor()
				}
			}
		}

		Repeater
		{
			id: factorsFormRepeater
			// model: factorsForm.model

			model: groupingFactors.model
			RowLayout
			{
				spacing: 0
				AssignButton
				{
					id: button
					Layout.leftMargin:  (factorsFormColumn.width / 3 - width) / 2
					Layout.rightMargin: (factorsFormColumn.width / 3 - width) / 2
					leftSource:         factorsForm.availableVariablesList
					rightSource:        factorList
				}
				SEM.FactorsList
				{
					id:					factorList
					// name:               factorName
					editableTitle:      name
					dropMode:			"Replace"
					suggestedColumns:	allowAll ? [] : ["scale"]
					allowedColumns:     ["scale"]
					implicitWidth:      listWidth
					height:             factorsForm.factorListHeight

					onTitleIsChanged: factorsForm.titleChanged(index, editableTitle)
					Component.onCompleted:
					{
						activeFocusChanged.connect(button.setIconToLeft);
						availableVariablesList.activeFocusChanged.connect(button.setIconToRight);
						hasSelectedItemsChanged.connect(button.setState);
						availableVariablesList.hasSelectedItemsChanged.connect(button.setState);
						// factorsForm.factorAdded(index, factorList);
						factorsForm.calculateHeight();
					}
				}
			}

		}

		// Row
		// {
		// 	id:             factorButtons
		// 	anchors.right:  parent.right
		// 	spacing:        10
		//
		// 	Button
		// 	{
		// 		name: "add";
		// 		text: qsTr("+")
		// 		control.width: height
		// 		width: control.width
		// 		onClicked: addFactor()
		// 	}
		// 	Button
		// 	{
		// 		name: "remove";
		// 		text: qsTr("-")
		// 		control.width: height
		// 		width: control.width
		// 		onClicked: removeFactor() ;
		// 		enabled: factorsFormRepeater.count > 1
		// 	}
		// }

	}

	function addFactor()
	{
		factorsForm.model.addFactor()
		factorsForm.calculateHeight()
	}

	function removeFactor()
	{
		factorsForm.model.removeFactor()
		factorsForm.calculateHeight()
	}

	function calculateHeight()
	{
		// if (factorsFormRepeater.count > 3) {
			factorsForm.height = Theme.defaultVariablesFormHeight + (factorsFormRepeater.count - 3) * (factorsForm.factorListHeight + factorsFormColumn.spacing) + Theme.defaultSingleItemListHeight * 3
		// } else {
		// 	factorsForm.height = Theme.defaultVariablesFormHeight
		// }
	}

}
