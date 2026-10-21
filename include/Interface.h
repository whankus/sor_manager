/**
* @file Interface.h
 * @brief TUI (Terminal User Interface) dla systemu S.O.R.
 * @author Wiktor Hankus (158159)
 */
#ifndef INTERFACE_H
#define INTERFACE_H

#include "SorManager.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

class Interface {
private:
    SorManager& manager;

public:
    Interface(SorManager& m);
    void uruchom();
};

#endif