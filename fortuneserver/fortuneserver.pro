0.
.
00
QT += network widgets sql core gui multimedia
QMAKE_LFLAGS_WINDOWS += /LARGEADDRESSAWARE

HEADERS       = server.h \
    threadbmp.h
SOURCES       = server.cpp \
                main.cpp \
                threadbmp.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/sql/fortuneserver
INSTALLS += target
