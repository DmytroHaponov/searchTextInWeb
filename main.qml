import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

Window {
    visible: true
    width: 800
    height: 600
    title: qsTr("Searcher")

    Column {
        id: _inputsRow
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.left: parent.left
        anchors.leftMargin: 60
        spacing: 50

        InputAndLabel {
            id: _mainUrl
            text: qsTr("input URL")
        }
        InputAndLabel {
            id: _downloadThreads
            text: qsTr("download threads quantity")
        }
        InputAndLabel {
            id: _targetText
            text: qsTr("text to search")
        }
        InputAndLabel {
            id: _maxURLs
            text: qsTr("max URL quantity")
        }
    }

    RoundButton {
        id: _launchBtn
        anchors.centerIn: parent
        height: 60
        width: 60
        radius: 30
        text: qsTr("search")
        onClicked: {
            if (_mainUrl.input.length === 0) {
                _errorText.text = qsTr("please type starting URL")
                _errorPopup.visible = true;
                return;
            }
            if (_downloadThreads.input.length === 0) {
                _errorText.text = qsTr("please type int of threads to download")
                _errorPopup.visible = true;
                return;
            }
            if (_targetText.input.length === 0) {
                _errorText.text = qsTr("please type text to search")
                _errorPopup.visible = true;
                return;
            }
            if (_maxURLs.input.length === 0) {
                _errorText.text = qsTr("please type int of max URL count")
                _errorPopup.visible = true;
                return;
            }
            search_engine.set_max_threads_count(_downloadThreads.input);
            search_engine.set_target_text(_targetText.input);
            search_engine.set_max_URL_quantity(_maxURLs.input);
            search_engine.on_main_URL_received(_mainUrl.input);
        }
    }

    Rectangle {
        id: _errorPopup
        anchors.centerIn: parent
        height: 400
        width: 400
        color: "red"
        Text {
            id: _errorText
            anchors.centerIn: parent
        }
        Button {
            id: _errorBtn
            anchors.centerIn: parent
            anchors.verticalCenterOffset: 30
            text: qsTr("OK")
            onPressed: {
                _errorPopup.visible = false;
                _errorText.text = "";
            }
        }
        visible: false
    }

    ProgressBar {
        id: _firstProgress
        anchors.right: parent.right
        anchors.rightMargin: 50
        anchors.top: parent.top
        height: 10
        width: 100
    }

    Connections {
        target: search_engine
        onError_msg: {
            _errorText.text = msg;
            _errorPopup.visible = true;
        }

        onDownload_progress_changed: {
            _firstProgress.value = part;
            _firstProgress.to = max;
        }
    }
}

