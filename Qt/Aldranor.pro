HEADERS = Aldranor_viewer.h Aldranor_opengl.h CUgl.h Object.h \
    plane.h
SOURCES = Aldranor.cpp Aldranor_viewer.cpp Aldranor_opengl.cpp CUgl.cpp Object.cpp \
    plane.cpp
QT += opengl
RESOURCES = Aldranor.qrc
LIBS += -lopengl32 -lglu32
