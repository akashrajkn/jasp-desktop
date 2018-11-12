import QtQuick 2.8
import QtQuick.Layouts 1.3
import JASP.Controls 1.0

Form {
    id: form

    GridLayout {
        ColumnLayout {
            spacing: 15
            GridLayout {
                Label { text: qsTr("t") }           TextField { text: "0" ; name: "tStatistic" ; inputType: "number"  }
                Label { text: qsTr("Group size") }  TextField { text: ""  ; name: "n1Size"     ; inputType: "integer" }
            }
        }
    }

    Divider{}

    GridLayout {
        ColumnLayout {
            spacing: 15

            ButtonGroup {
                title: qsTr("Hypothesis")
                name: "hypothesis"

                RadioButton { text: qsTr("Measure 1 \u2260 Measure 2"); name: "groupsNotEqual" ; checked: true }
                RadioButton { text: qsTr("Measure 1 > Measure 2")     ; name: "groupOneGreater"                }
                RadioButton { text: qsTr("Measure 1 < Measure 2")     ; name: "groupTwoGreater"                }
            }

            ButtonGroup {
                title: qsTr("Bayes Factor")                      ; name: "bayesFactorType"

                RadioButton { text: qsTr("BF\u2081\u2080")       ; name: "BF10"; checked: true }
                RadioButton { text: qsTr("BF\u2080\u2081")       ; name: "BF01"                }
                RadioButton { text: qsTr("Log(BF\u2081\u2080)")  ; name: "LogBF10"             }
            }
        }

        ColumnLayout {
            spacing: 15

            GroupBox {
                title: qsTr("Plots")
                CheckBox {  text: qsTr("Prior and posterior")           ; name: "plotPriorAndPosterior"                  ; id: plotPriorAndPosterior }
                CheckBox {  text: qsTr("Additional info")               ; name: "plotPriorAndPosteriorAdditionalInfo"    ; Layout.leftMargin: 20; checked: true; enabled: plotPriorAndPosterior.checked}

                CheckBox {  text: qsTr("Bayes factor robustness check") ; name: "plotBayesFactorRobustness"              ; id: plotBayesFactorRobustness }
                CheckBox {  text: qsTr("Additional info")               ; name: "plotBayesFactorRobustnessAdditionalInfo"; Layout.leftMargin: 20; checked: true; enabled: plotBayesFactorRobustness.checked}
            }
        }
    }

    SubjectivePriorsComponent { }
}
