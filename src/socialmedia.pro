QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    add_post.cpp \
    admin_control.cpp \
    change_password.cpp \
    change_profile.cpp \
    edit_post.cpp \
    explorer.cpp \
    followings_posts.cpp \
    get_information.cpp \
    main.cpp \
    mainwindow.cpp \
    messages.cpp \
    other_page.cpp \
    personal_page.cpp \
    recommend.cpp \
    search.cpp \
    setting.cpp \
    show_followers.cpp \
    show_followings.cpp \
    show_post.cpp

HEADERS += \
    add_post.h \
    admin_control.h \
    change_password.h \
    change_profile.h \
    edit_post.h \
    explorer.h \
    followings_posts.h \
    get_information.h \
    mainwindow.h \
    messages.h \
    other_page.h \
    personal_page.h \
    recommend.h \
    search.h \
    setting.h \
    show_followers.h \
    show_followings.h \
    show_post.h

FORMS += \
    add_post.ui \
    admin_control.ui \
    change_password.ui \
    change_profile.ui \
    edit_post.ui \
    explorer.ui \
    followings_posts.ui \
    get_information.ui \
    mainwindow.ui \
    messages.ui \
    other_page.ui \
    personal_page.ui \
    recommend.ui \
    search.ui \
    setting.ui \
    show_followers.ui \
    show_followings.ui \
    show_post.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
