import QtQuick 2.8
import JASP.Controls 1.0 as JC
import JASP.Theme 1.0

JC.VariablesList
{
	property string editableTitle
	// property alias editableTitle: titleField.value
	signal titleIsChanged()
	title: " " //dummy

	JC.Label
	{
		id: titleField
		// isBound: false
		anchors.top: parent.top
        anchors.left: parent.left
        height: text.height
		text: "Hello component"
        // fieldWidth: parent.width
	}

    // JC.TextField
    // {
	// 	id: titleField
	// 	isBound: false
    //     anchors.top: parent.top
    //     anchors.left: parent.left
    //     height: text.height
    //     fieldWidth: parent.width
    // }
	//
	// Component.onCompleted: titleField.editingFinished.connect(titleIsChanged);

}
