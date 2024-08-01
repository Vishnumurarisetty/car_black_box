/*
 * File:   menu.c
 * Author: VISHNU
 *
 * Created on July 9, 2024, 5:18 PM
 */

#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "matrix_keypad.h"
char menu_set[5][16] = {"View Log       ", "Download Log   ", "Clear Log      ", "Set Time       ", "Change Password"};
extern char main_f, menu_f;
static int i, enter_time, exit_time;
static char arrow_flag, m_key;

void menu(char key) {
    //logic for menu
    //    static char arrow_flag,m_key;
    //    static int i,enter_time,exit_time;

    m_key = read_switches(LEVEL_CHANGE);

    if (!arrow_flag) {
        clcd_putch(0X7E, LINE1(0));
        clcd_putch(0X20, LINE2(0));
    } else {
        clcd_putch(0X20, LINE1(0));
        clcd_putch(0X7E, LINE2(0));
    }

    clcd_print(menu_set[i], LINE1(1));
    clcd_print(menu_set[i + 1], LINE2(1));

    if (m_key == MK_SW5) {
        exit_time++;
    } else if (exit_time > 1 && exit_time < 500) {
        if (!arrow_flag) {
            arrow_flag = 1;
        } else if (i < 3) {
            i++;
        }
        exit_time = 0;
    } else
        exit_time = 0;

    if (exit_time >= 500) {
        i = 0;
        arrow_flag = 0;
        CLEAR_DISP_SCREEN;
        main_f = DASHBOARD;
    }

    if (m_key == MK_SW4) {
        enter_time++;
    } else if (enter_time > 1 && enter_time < 500) {
        if (arrow_flag) {
            arrow_flag = 0;
        } else if (i > 0) {
            i--;
        }
        enter_time = 0;
    } else
        enter_time = 0;

    if (enter_time >= 500) {


        menu_f = i + arrow_flag;
        main_f = MENU_ENTER;
        arrow_flag = 0;
        CLEAR_DISP_SCREEN;
    }

} 