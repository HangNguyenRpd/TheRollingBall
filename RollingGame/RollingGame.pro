TEMPLATE = app
CONFIG -= console c++11
CONFIG -= app_bundle
CONFIG +=windows
QT+=gui opengl core
TARGET=RollingGame
DESTDIR=./


SOURCES +=  src/main.cpp \
            src/Mainwindow.cpp \
            src/GameScene.cpp \
            src/SceneSlotSignal.cpp \
            src/SceneMouseControl.cpp

FORMS +=    ui/Mainwindow.ui \

HEADERS +=  include/Mainwindow.h \
            include/GameScene.h \

OTHER_FILES+= README.md \
              shaders/ToonFrag.glsl \
              shaders/ToonVert.glsl \
              shaders/ShadowFrag.glsl \
              shaders/ShadowVert.glsl \

INCLUDEPATH+=include
OBJECTS_DIR=obj
MOC_DIR=moc
UI_DIR = ui

cache()

#library flags on different platforms win32:LIBS += -lglfw3dll -lopengl32 -lGLU32 -lfreeglut -lglew32

#use NGL
NGLPATH=$$(NGLDIR)
isEmpty(NGLPATH){ # note brace must be here
        message("including $HOME/NGL")
        include($(HOME)/NGL/UseNGL.pri)
}
else{ # note brace must be here
        message("Using custom NGL location")
        include($(NGLDIR)/UseNGL.pri)
}

INCLUDEPATH +=$$NGLPATH/include/
