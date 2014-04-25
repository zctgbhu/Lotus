#-------------------------------------------------
#
# Project created by QtCreator 2013-07-04T19:30:32
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lotus
TEMPLATE = app
#CONFIG +=static
target.path = /usr/bin
DEFINES+=QT_NO_DEBUG_OUTPUT
INSTALLS += target \

SOURCES += src/main.cpp\
    src/loginwin.cpp \
    src/loginloadwin.cpp \
    src/tablabel.cpp \
    src/lotus.cpp \
    src/rosterItemModel.cpp \
    src/rosterItem.cpp \
    src/personalchatdlg.cpp \
    src/groupchatdlg.cpp \
    src/searchdlg.cpp \
    src/roomitemmodel.cpp \
    src/roomitem.cpp \
    src/registerdialog.cpp \
    src/uservcard.cpp \
    src/lotus_view.cpp \
    src/lotus_event.cpp \
    src/rosteritemsortfilter.cpp \
    src/settings.cpp \
    src/personalchatdlg_view.cpp \
    src/personalchatdlg_event.cpp \
    src/groupchatdlg_view.cpp \
    src/groupchatdlg_event.cpp \
    src/roomuseritem.cpp \
    src/aboutdialog.cpp \
    src/accountcache.cpp \
    src/facepanel.cpp \
    src/giftextedit.cpp \
    src/facepreview.cpp \
    src/roomfavoritecache.cpp \
    src/statustextline.cpp \
    src/chatslog.cpp

HEADERS  += \
	include/loginwin.h \
	include/loginloadwin.h \
	include/tablabel.h \
	include/lotus.h \
	include/rosterItemModel.h \
	include/rosterItem.h \
	include/personalchatdlg.h \
	include/groupchatdlg.h \
	include/searchdlg.h \
	include/roomitemmodel.h \
	include/roomitem.h \
	include/registerdialog.h \
	include/uservcard.h \
	include/rosteritemsortfilter.h \
	include/settings.h \
	include/roomuseritem.h \
	include/aboutdialog.h \
	include/accountcache.h \
        include/facepanel.h \
        include/giftextedit.h \
        include/facepreview.h \
        include/roomfavoritecache.h \
        include/statustextline.h \
        include/chatslog.h

FORMS    += \
    src/loginwin.ui \
    src/loginloadwin.ui \
    src/lotus.ui \
    src/personalchatdlg.ui \
    src/groupchatdlg.ui \
    src/searchdlg.ui \
    src/registerdialog.ui \
    src/settings.ui \
    src/aboutdialog.ui \
    src/facepanel.ui \
    src/facepreview.ui


LIBS +=  -lqxmpp

INCLUDEPATH += /usr/include/qxmpp \
	include



RESOURCES += pixmap.qrc
