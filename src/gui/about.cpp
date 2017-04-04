/*
 * About.cpp
 * Information about program
 */
#include "about.h"
#include "src/base/version.h"

/**
 * Display the about dialog
 * The gui is defined in about.ui
 */
TAbout::TAbout(){
	ui.setupUi(this);
	connect(ui.closeButton,SIGNAL(clicked()),this,SLOT(close()));
	ui.versionLabel->setText(VERSION);
}