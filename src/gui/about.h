/*
 * About.h
 * Information about program
 */
#ifndef __ABOUT_H_
#define __ABOUT_H_
#include "about.h"
#include "ui_about.h"
#include <QDialog>
class TAbout:public QDialog
{
private:
	Ui::about ui;
public:
	TAbout();
	
};
#endif