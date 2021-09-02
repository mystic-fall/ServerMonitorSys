/*************************************************************************
	> File Name: color.h
	> Author: 
	> Mail: 
	> Created Time: Mon 16 Aug 2021 03:34:57 PM CST
 ************************************************************************/

#ifndef _COLOR_H
#define _COLOR_H

#define COLOR(a, b) "\033[" #b "m" a "\033[0m" 
#define COLOR_HL(a, b) "\033[1;" #b "m" a "\033[0m"

#define RED(a) COLOR(a, 31)
#define GREEN(a) COLOR(a, 32)
#define YELLOW(a) COLOR(a, 33)
#define BLUE(a) COLOR(a, 34)
#define PURPLE(a) COLOR(a, 35)

#define RED_HL(a) COLOR_HL(a, 31)
#define GREEN_HL(a) COLOR_HL(a, 32)
#define YELLOW_HL(a) COLOR_HL(a, 33)
#define BLUE_HL(a) COLOR_HL(a, 34)
#define PURPLE_HL(a) COLOR_HL(a, 35)

#endif
