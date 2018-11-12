import QtQuick 2.8
import QtQuick.Layouts 1.3
import JASP.Controls 1.0

import "../widgets"

Form {
    id: form

    VariablesForm {
        height: 200
        defaultAssignedVariablesList {
            title: qsTr("Dependent Variables")
            allowedColumns: ["scale"]
        }
        AssignedVariablesList {
            name: "groupingVariable"
            title: qsTr("Grouping Variable")
            singleItem: true
            allowedColumns: ["ordinal", "nominal"]
        }
    }

    GridLayout {
        ColumnLayout {
            spacing: 15
            ButtonGroup {
                title: qsTr("Hypothesis")                       ; name: "hypothesis"
                RadioButton {   text: qsTr("Group 1 ≠ Group 2") ; name: "groupsNotEqual"       ; checked: true}
                RadioButton {   text: qsTr("Group 1 > Group 2") ; name: "groupOneGreater"    }
                RadioButton {   text: qsTr("Group 1 < Group 2") ; name: "groupTwoGreater"    }
            }

            ButtonGroup {
                title: qsTr("Bayes Factor")                     ; name: "bayesFactorType"
                RadioButton {  text: qsTr("BF\u2081\u2080")       ; name: "BF10"          ; checked: true}
                RadioButton {  text: qsTr("BF\u2080\u2081")       ; name: "BF01"  }
                RadioButton {  text: qsTr("Log(BF\u2081\u2080)")  ; name: "LogBF10"  }
            }

            ButtonGroup {
                title: qsTr("Tests")                            ; name: "testStatistic"
                RadioButton {   text: qsTr("Student")           ; name: "Student";  checked: true}
                RadioButton {   text: qsTr("Mann-Whitney")      ; name: "Wilcoxon"; id: wilcoxon}
                Row {
                    spacing: 5
                    enabled: wilcoxon.checked
                    Layout.leftMargin: 25
                    Label { text: qsTr("No. samples")}
                    TextField { text: "1000"; name: "wilcoxonSamplesNumber";inputType: "integer"; validator: IntValidator {bottom: 100; top: 10000}}
                }
            }

            GroupBox {
                title: qsTr("Assumption checks")
                CheckBox {  text: qsTr("Descriptives")                 ; name: "descriptives"   }
            }
        }

        ColumnLayout {
            spacing: 15
            GroupBox {
                title: qsTr("Plots")
                CheckBox {  text: qsTr("Prior and posterior") ; name: "plotPriorAndPosterior"; id: plotPriorAndPosterior }
                CheckBox {  text: qsTr("Additional info")     ; name: "plotPriorAndPosteriorAdditionalInfo"; Layout.leftMargin: 20; checked: true; enabled: plotPriorAndPosterior.checked}

                CheckBox {  text: qsTr("Bayes factor robustness check") ; name: "plotBayesFactorRobustness"; id: plotBayesFactorRobustness }
                CheckBox {  text: qsTr("Additional info")     ; name: "plotBayesFactorRobustnessAdditionalInfo"; Layout.leftMargin: 20; checked: true; enabled: plotBayesFactorRobustness.checked}

                CheckBox {  text: qsTr("Sequential analysis") ; name: "plotSequentialAnalysis"; id: plotSequentialAnalysis }
                CheckBox {  text: qsTr("Robustness check")    ; name: "plotSequentialAnalysisRobustness"; Layout.leftMargin: 20; enabled: plotSequentialAnalysis.checked}

                CheckBox {  text: qsTr("Descriptives plots")  ; name: "descriptivesPlots"; id: descriptivesPlots }
                PercentField { label.text: qsTr("Credible interval") ; name: "descriptivesPlotsCredibleInterval"; defaultValue: 95; Layout.leftMargin: 20; enabled: descriptivesPlots.checked}
            }

            ButtonGroup {
                title: qsTr("Missing Values"); name: "missingValues"
                RadioButton {   text: qsTr("Exclude cases analysis by analysis")    ; name: "excludeAnalysisByAnalysis" ; checked: true }
                RadioButton {   text: qsTr("Exclude cases listwise")                ; name: "excludeListwise"    }
            }
        }
    }

    SubjectivePriors { }
}
