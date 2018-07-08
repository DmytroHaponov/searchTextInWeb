import QtQuick 2.7
import QtQuick.Controls 2.4

Rectangle {
    id: root
    objectName: "results"

    height: 250 // effective _allResults.height
            + _titleRect.height
    radius: 8
    gradient: Gradient {
        GradientStop {
            position: 0.05;
            color: "#f75050";
        }
        GradientStop {
            position: 0.56;
            color: "#382121";
        }
        GradientStop {
            position: 1.00;
            color: "#accc1b";
        }
    }
    border.color: "#fbf813"
    border.width: 15
    width: 600

    Rectangle {
        id: _titleRect

        anchors.top: root.top
        anchors.horizontalCenter: root.horizontalCenter

        height: 15
        color: "#1a1818"
        Text {
            id: _resultTitle
            anchors.centerIn: parent
            text: qsTr("results:")
            font.pixelSize: 15
        }
    }

    ListView {
        id: _allResults

        anchors.fill: root
        anchors.margins: root.border.width + 3

        clip: true
        orientation: ListView.Horizontal
        spacing: 10

        model: search_engine.results
        delegate: _fileResultsDelegate

        ScrollBar.horizontal: ScrollBar {
            policy: ScrollBar.AlwaysOn
            visible: _allResults.count > 0

            parent: _allResults.parent
            anchors.left: _allResults.left
            anchors.bottom: _allResults.bottom
            anchors.bottomMargin: -8
            width: _allResults.width
        }

        Component {
            id: _fileResultsDelegate

            Rectangle {
                id: _fileResultsWrapper

                border.color: "black"
                border.width: 2

                height: parent.height
                width: 200

                Label {
                    id: _fileName
                    anchors.top: parent.top
                    anchors.horizontalCenter: parent.horizontalCenter

                    text: modelData.url
                    height: 12
                    width: parent.width
                    elide: Text.ElideRight
                    ToolTip {
                        id: _urlTip
                        text: parent.text
                        visible: false
                    }
                    MouseArea {
                        id: _hoverArea
                        anchors.fill: parent
                        onEntered: _urlTip.visible = true
                        onExited: _urlTip.visible = false
                        hoverEnabled: true
                    }
                }

                ListView {
                    id: _resultsRow

                    anchors.left: parent.left
                    anchors.top: _fileName.bottom
                    anchors.topMargin: 5

                    header: Rectangle {
                        id: _urlHeader
                        color: "black"
                        width: parent.width
                        height: visible ? 40 : 0
                        Text {
                            id: _noResults
                            anchors.centerIn: parent
                            text: qsTr("no Results")
                            color: "red"
                        }
                        visible: modelData.results_of_url.length === 0
                    }

                    clip: true
                    height: parent.height - 2*_fileResultsWrapper.border.width - _fileName.height
                    width: parent.width
                    spacing: 5

                    Component {
                        id: _resultsDelegate

                        Rectangle {
                            id: _resultWrapper
                            height: 40
                            color: "yellow"
                            border.color: "green"
                            border.width: 2
                            width: parent.width

                            Rectangle {
                                id: _lineRect

                                anchors.top: parent.top
                                anchors.topMargin: 2
                                anchors.horizontalCenter: parent.horizontalCenter

                                height: parent.height/2 - 2
                                width: parent.width - 4
                                color: "#9cdcf5"

                                Text {
                                    id: lineText
                                    anchors.centerIn: parent
                                    text: qsTr( "line: " + modelData.line)
                                    font.pixelSize: 14
                                }
                            }
                            Text {
                                id: _columnTitle

                                anchors.left: parent.left
                                anchors.leftMargin: 2
                                anchors.bottom: parent.bottom
                                anchors.bottomMargin: 2

                                text: qsTr("columns: ")
                                font.pixelSize: 15
                            }

                            ListView {
                                id: _columnsRow

                                anchors.bottom: parent.bottom
                                anchors.bottomMargin: 2
                                anchors.left: _columnTitle.right
                                anchors.leftMargin: 4

                                clip: true
                                height: parent.height/2 - 2
                                width: parent.width
                                       - _columnTitle.width
                                       - _columnTitle.anchors.leftMargin
                                       - _columnsRow.anchors.leftMargin
                                       - 2*_resultWrapper.border.width

                                orientation: ListView.Horizontal
                                spacing: 5

                                model: modelData.columns
                                delegate: Text {
                                    id: _columnNumber
                                    anchors.verticalCenter: parent.verticalCenter
                                    text: qsTr(modelData)
                                    font.pixelSize: 13
                                }
                                ScrollBar.horizontal: ScrollBar {
                                    parent: _columnsRow.parent
                                    anchors.left: _columnsRow.left
                                    anchors.bottom: _columnsRow.bottom
                                    anchors.bottomMargin: -8
                                    width: _columnsRow.width
                                }
                            }
                        }
                    }

                    model: modelData.results_of_url
                    delegate: _resultsDelegate

                    ScrollBar.vertical: ScrollBar {
                        policy: ScrollBar.AlwaysOn
                        visible: parent.count > 0
                    }
                }
            }
        }
    }
}

