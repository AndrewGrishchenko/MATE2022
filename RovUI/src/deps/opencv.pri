win32 {
    #win32
}

unix {
    INCLUDEPATH += $(HOME)/opencv-build/build2/include/opencv4/

    LIBS += -L$(HOME)/opencv-build/build2/lib \
            -lopencv_core \
            -lopencv_imgproc \
            -lopencv_imgcodecs \
            -lopencv_highgui \
            -lopencv_objdetect \
            -lopencv_videoio
}
