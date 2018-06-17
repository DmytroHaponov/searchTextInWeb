import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

Window {
    visible: true
    width: 800
    height: 600
    title: qsTr("Searcher")



    Rectangle {
        id: _editRect
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 30
        width: 100
        height: 30
        color: "green"
        border.color: "red"
        border.width: 2
        TextEdit {
            id: _url
            anchors.fill: parent
            color: "purple"
            padding: 5
        }
    }
    Label {
        id: _URLlabel
        text: "input URL"
        background: Rectangle {
            id: _URLlabelBack
            gradient: Gradient {
                GradientStop { position: 0.3; color: "blue" }
                GradientStop { position: 1.0; color: "yellow" }
            }
        }

        anchors.bottom: _editRect.top
        anchors.bottomMargin: 3
        anchors.horizontalCenter: _editRect.horizontalCenter
        width: contentWidth
        height: 20
    }

    RoundButton {
        anchors.centerIn: parent
        width: 60
        height: 60
        text: "search"
        radius: 30
        onClicked: {
            search_engine.on_main_URL_received(_url.text);
        }
    }
}
