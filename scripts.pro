######################################################################
# Automatically generated by qmake (2.01a) Wed Jun 25 14:48:16 2014
######################################################################

TEMPLATE = app
TARGET = patent
DEPENDPATH += . includes
INCLUDEPATH += . includes
QMAKE_CXXFLAGS += -std=c++11

# Input
HEADERS += includes/pugiconfig.hpp includes/pugixml.hpp \
    includes/sqlite3x.hpp \
    includes/sqlite3x-master/sqlite3x.hpp \
    bayes.hpp \
    patent.hpp \
    trie.hpp \
    xmlpatentparser.hpp
SOURCES += parse.cpp \
    includes/sqlite3x-master/sqlite3x_command.cpp \
    includes/sqlite3x-master/sqlite3x_connection.cpp \
    includes/sqlite3x-master/sqlite3x_exception.cpp \
    includes/sqlite3x-master/sqlite3x_reader.cpp \
    includes/sqlite3x-master/sqlite3x_transaction.cpp \
    bayes.cpp \
    patent.cpp \
    trie.cpp \
    xmlpatentparser.cpp


unix|win32: LIBS += -lsqlite3
unix|win32: LIBS += -lsqlite3x

#Pugixml

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/release/ -lpugixml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/debug/ -lpugixml
else:unix: LIBS += -L$$PWD/ -lpugixml

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/release/libpugixml.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/debug/libpugixml.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/release/pugixml.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/debug/pugixml.lib
else:unix: PRE_TARGETDEPS += $$PWD/libpugixml.a



unix|win32: LIBS += -lboost_regex

unix|win32: LIBS += -lboost_program_options
