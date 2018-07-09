import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

Window {
    visible: true
    width: 800
    height: 600
    title: qsTr("search text in urls")

    Column {
        id: _inputsRow
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter

        spacing: 20

        InputAndLabel {
            id: _mainUrl
            text: qsTr("input URL")
            inputWidth: 600
        }
        InputAndLabel {
            id: _targetText
            text: qsTr("text to search")
            inputWidth: 600
        }
        Row {
            id: _innerRow

            anchors.horizontalCenter: parent.horizontalCenter
            spacing: parent.width - _downloadThreads.width - _maxURLs.width

            InputAndLabel {
                id: _downloadThreads
                text: qsTr("download threads quantity")
            }
            InputAndLabel {
                id: _maxURLs
                text: qsTr("maximum URL quantity")
            }
        }
    }

    RoundButton {
        id: _launchBtn

        anchors.bottom: _inputsRow.bottom
        anchors.bottomMargin: -radius
        anchors.horizontalCenter: _inputsRow.horizontalCenter
        anchors.horizontalCenterOffset: 30

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

    RoundButton {
        id: _stopBtn

        anchors.bottom: _launchBtn.bottom
        anchors.horizontalCenter: _inputsRow.horizontalCenter
        anchors.horizontalCenterOffset: -30

        height: 60
        width: 60
        radius: 30
        text: qsTr("stop")
        onClicked: {
            search_engine.stop();
        }
    }

    // TODO
    ProgressBar {
        id: _progressBar

        anchors.right: parent.right
        anchors.rightMargin: 50
        anchors.top: parent.top

        padding: 2

        background: Rectangle {
            implicitWidth: 200
            implicitHeight: 6
            color: "#e6e6e6"
            radius: 3
        }

        contentItem: Item {
            implicitWidth: 200
            implicitHeight: 4

            Rectangle {
                width: _progressBar.visualPosition * parent.width
                height: parent.height
                radius: 2
                color: "#17a81a"
            }
        }
        height: 15
        width: 100
    }

    ResultsView {
        id: _results

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: _launchBtn.bottom
        anchors.topMargin: 40
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

    Connections {
        target: search_engine
        onError_msg: {
            _errorText.text = msg;
            _errorPopup.visible = true;
        }

        onDownload_progress_changed: {
            if (_progressBar.value === _progressBar.to) {
                // ingore spurious indeterminate signals after download is complete
                return;
            }
            _progressBar.value = part;
            _progressBar.to = max;
            _progressBar.indeterminate = (max === -1)
        }
    }
}

