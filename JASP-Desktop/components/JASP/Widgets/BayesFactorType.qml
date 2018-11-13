import QtQuick 2.8
import QtQuick.Layouts 1.3
import JASP.Controls 1.0


ButtonGroup {
    title: qsTr("Bayes Factor")                     ; name: "bayesFactorType"

    RadioButton { text: qsTr("BF\u2081\u2080")      ; name: "BF10"; checked: true }
    RadioButton { text: qsTr("BF\u2080\u2081")      ; name: "BF01"                }
    RadioButton { text: qsTr("Log(BF\u2081\u2080)") ; name: "LogBF10"             }
}
