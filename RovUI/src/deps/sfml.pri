win32 {
    INCLUDEPATH += "$$PWD\..\..\thirdpaty\sfml\include"
    INCLUDEPATH ~= s,/,\\,g

    DEPENDPATH += "$$PWD\..\..\thirdpaty\sfml\include"
    DEPENDPATH ~= s,/,\\,g

    CONFIG(release, debug | release) : {
        LIBS += -L"$$PWD\..\..\thirdpaty\sfml\lib"
        LIBS += \
            -lsfml-main \
            -lsfml-system \
            -lsfml-window
        LIBS ~= s,/,\\,g
    }

    CONFIG(debug, debug | release) : {
        LIBS += -L"$$PWD\..\..\thirdpaty\sfml\lib"
        LIBS += \
            -lsfml-main-d \
            -lsfml-system-d \
            -lsfml-window-d
        LIBS ~= s,/,\\,g
    }
}

unix {
    INCLUDEPATH += "$$PWD/../../thirdparty/sfml/include"

    DEPENDPATH += "$$PWD/../../thirdparty/sfml/include"

    CONFIG(release, debug | release) : {
        LIBS += -L"$$PWD/../../thirdparty/sfml/lib"
        LIBS += \
            -lsfml-system \
            -lsfml-window
    }

    CONFIG(debug, debug | release) : {
        LIBS += -L"$$PWD/../../thirdparty/sfml/lib"
        LIBS += \
            -lsfml-system \
            -lsfml-window
    }
}
