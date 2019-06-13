import QtQuick 2.8
import JASP.Controls 1.0 as JC
import JASP.Theme 1.0

JC.VariablesList
{
	property alias editableTitle: label.text
	signal titleIsChanged()
	title: " " //dummy

    // JC.TextField
    // {
	// 	id: titleField
	// 	isBound: false
    //     anchors.top: parent.top
    //     anchors.left: parent.left
    //     height: text.height
    //     fieldWidth: parent.width
    // }

	JC.Label
	{
		id:				label
		anchors.top:	parent.top
		anchors.left:	parent.left
		// font:			Theme.font
		// color:			enabled ? Theme.textEnabled : Theme.textDisabled
	}

    JC.ExtraControlColumn {
        type: "CheckBox"
        name: "isNuisance"
        title: qsTr("Add to null model")
        purpose: "nuisance"
    }

	// Component.onCompleted: titleField.editingFinished.connect(titleIsChanged);

}
