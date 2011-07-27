/**************************************************************************
 *  Copyright 2007,2008,2009 KISS Institute for Practical Robotics        *
 *                                                                        *
 *  This file is part of KISS (Kipr's Instructional Software System).     *
 *                                                                        *
 *  KISS is free software: you can redistribute it and/or modify          *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  KISS is distributed in the hope that it will be useful,               *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with KISS.  Check the LICENSE file in the project root.         *
 *  If not, see <http://www.gnu.org/licenses/>.                           *
 **************************************************************************/

#include "EditorSettingsDialog.h"

#include <QSettings>

EditorSettingsDialog::EditorSettingsDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);

	// Set up the Auto Completion Group
	ui_autoCompletionAPISourceCheckBox->connect(ui_autoCompletionEnabledCheckBox, SIGNAL(toggled(bool)), SLOT(setEnabled(bool)));
	ui_autoCompletionDocumentSourceCheckBox->connect(ui_autoCompletionEnabledCheckBox, SIGNAL(toggled(bool)), SLOT(setEnabled(bool)));
	ui_autoCompletionThresholdSpinBox->connect(ui_autoCompletionEnabledCheckBox, SIGNAL(toggled(bool)), SLOT(setEnabled(bool)));
	
	// Set up the auto Indent Group
	ui_autoIndentMaintainStyleRadioButton->connect(ui_autoIndentEnabledCheckBox, SIGNAL(toggled(bool)), SLOT(setEnabled(bool)));
	ui_autoIndentIntelligentStyleRadioButton->connect(ui_autoIndentEnabledCheckBox, SIGNAL(toggled(bool)), SLOT(setEnabled(bool)));
	ui_autoIndentWidthSpinBox->connect(ui_autoIndentEnabledCheckBox, SIGNAL(toggled(bool)), SLOT(setEnabled(bool)));

	// Make sure the defaults are saved
	readSettings();
	saveSettings();
}

EditorSettingsDialog::~EditorSettingsDialog() {}

// Show the dialog and save if ok is clicked
int EditorSettingsDialog::exec()
{
	readSettings();
	if(!QDialog::exec()) return QDialog::Rejected;
	saveSettings();
	return QDialog::Accepted;
}

// Save the settings from the dialog
void EditorSettingsDialog::saveSettings()
{
	QSettings settings;
	settings.beginGroup("Editor");
	settings.setValue("font", ui_fontBox->currentFont().family());
	settings.setValue("fontsize", ui_fontSizeSpinBox->value());
	
	settings.beginGroup("autocompletion");
	settings.setValue("enabled", ui_autoCompletionEnabledCheckBox->isChecked());
	settings.setValue("apisource", ui_autoCompletionAPISourceCheckBox->isChecked());
	settings.setValue("docsource", ui_autoCompletionDocumentSourceCheckBox->isChecked());
	settings.setValue("threshold", ui_autoCompletionThresholdSpinBox->value());
	settings.endGroup();

	settings.beginGroup("autoindent");
	settings.setValue("enabled", ui_autoIndentEnabledCheckBox->isChecked());
	if(ui_autoIndentMaintainStyleRadioButton->isChecked())
		settings.setValue("style", "Maintain");
	else if (ui_autoIndentIntelligentStyleRadioButton->isChecked())
		settings.setValue("style", "Intelligent");
	settings.setValue("width", ui_autoIndentWidthSpinBox->value());
	settings.endGroup();

	settings.setValue("calltips", ui_callTipsCheckBox->isChecked());
	settings.setValue("bracematching", ui_braceMatchingCheckBox->isChecked());
	settings.setValue("linenumbers", ui_marginLineNumbersCheckBox->isChecked());
	settings.endGroup();
	settings.sync();
}

// Read settings from the app config, setting defaults if necessary
void EditorSettingsDialog::readSettings()
{
	// Open up the applications settings and look in the editor group
	QSettings settings;
	settings.beginGroup(EDITOR);

	// Figure out the font and set it
	#ifdef Q_OS_WIN32
	QString fontString = settings.value(FONT, "Courier New").toString();
	#elif defined(Q_OS_MAC)
	QString fontString = settings.value(FONT, "Monaco").toString();
	#else
	QString fontString = settings.value(FONT, "Monospace").toString();
	#endif
	ui_fontBox->setCurrentFont(QFont(fontString));
	
	// Figure out the font size and set the widget
	#ifdef Q_OS_MAC
	ui_fontSizeSpinBox->setValue(settings.value(FONT_SIZE, 12).toInt());
	#else
	ui_fontSizeSpinBox->setValue(settings.value(FONT_SIZE, 10).toInt());
	#endif

	// Set the auto completion settings from the application config
	settings.beginGroup(AUTO_COMPLETION);
	ui_autoCompletionEnabledCheckBox->setChecked(settings.value(ENABLED, false).toBool());
	ui_autoCompletionAPISourceCheckBox->setChecked(settings.value(API_SOURCE, false).toBool());
	ui_autoCompletionDocumentSourceCheckBox->setChecked(settings.value(DOC_SOURCE, false).toBool());
	ui_autoCompletionThresholdSpinBox->setValue(settings.value(THRESHOLD, 4).toInt());
	settings.endGroup();

	// Set the auto indent settings from the application config
	settings.beginGroup(AUTO_INDENT);
	ui_autoIndentEnabledCheckBox->setChecked(settings.value(ENABLED, true).toBool());
	if(settings.value(STYLE).toString() == MAINTAIN)
		ui_autoIndentMaintainStyleRadioButton->setChecked(true);
	else if(settings.value(STYLE).toString() == INTELLIGENT)
		ui_autoIndentIntelligentStyleRadioButton->setChecked(true);
	ui_autoIndentWidthSpinBox->setValue(settings.value(WIDTH, 4).toInt());
	settings.endGroup();

	// Set the misc options from the application config
	ui_callTipsCheckBox->setChecked(settings.value(CALL_TIPS, true).toBool());
	ui_braceMatchingCheckBox->setChecked(settings.value(BRACE_MATCHING, true).toBool());
	ui_marginLineNumbersCheckBox->setChecked(settings.value(LINE_NUMBERS, true).toBool());

	settings.endGroup();
}

