import QtQuick 2.8
import QtQuick.Layouts 1.3
import JASP.Controls 1.0


GridLayout {
    ColumnLayout {

        ButtonGroup {
            title: qsTr("")
            name: "hypothesis"
            RadioButton {   text: qsTr("Default") ; name: "notEqualToTestValue"       ; checked: true}
            RadioButton {   text: qsTr("Informed") ; name: "greaterThanTestValue"    }
        }

    }
}
