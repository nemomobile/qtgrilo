equals(QT_MAJOR_VERSION, 4): GRILOQTLIB = grilo-qt
equals(QT_MAJOR_VERSION, 5): {
    GRILOQTLIB = grilo-qt5
    DEFINES += QT_VERSION_5
}
