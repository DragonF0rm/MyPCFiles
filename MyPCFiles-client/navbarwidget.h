#ifndef NAVBARWIDGET_H
#define NAVBARWIDGET_H


#define BOOST_LOG_DYN_LINK 1

#include <boost/log/trivial.hpp>

#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>

class NavbarWidget: public QMenuBar {
    Q_OBJECT
public:
    NavbarWidget(QWidget* parent = nullptr);
    NavbarWidget(const NavbarWidget& other) = delete;
    NavbarWidget(NavbarWidget&& other) = delete;
    ~NavbarWidget() = default;

    NavbarWidget&operator=(const NavbarWidget& other) = delete;
    NavbarWidget&operator=(NavbarWidget& other) = delete;
private slots:
    void settings_menu_on_click();
};

#endif
