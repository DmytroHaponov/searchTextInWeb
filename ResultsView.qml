import QtQuick 2.7
import QtQuick.Controls 2.4

Item {
    id: root
    objectName: "results"

    height: _resultsRow.height + _titleRect.height
    width: 200

    Rectangle {
        id: _titleRect
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        height: 12
        color: "#1a1818"
        Text {
            id: _resultTitle
            anchors.centerIn: parent
            text: qsTr("results:")
        }
    }
    ListView {
        id: _resultsRow
        anchors.left: parent.left
        anchors.top: _titleRect.bottom
        anchors.topMargin: 5
        clip: true
        height: 200
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
                    font.pixelSize: 14
                }


                Row {
                    id: _columnsRow
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 2
                    anchors.left: _columnTitle.right
                    anchors.leftMargin: 2
                    height: parent.height/2 - 2
                    spacing: 5
                    Repeater {
                        id: _column
                        model: modelData.columns

                        Text {
                            id: name
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: 2
                            text: qsTr(modelData)
                            font.pixelSize: 13
                        }
                    }
                }
            }
        }

        model: search_engine.results
        delegate: _resultsDelegate

        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AlwaysOn
            visible: parent.count > 0
        }
    }
}

