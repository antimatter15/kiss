/**************************************************************************
 *  Copyright 2007-2011 KISS Institute for Practical Robotics             *
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

#include "LexerManager.h"
#include "Lexer.h"
#include "Kiss.h"

#include <QDir>
#include <QDebug>

#define DELIM " "

LexerManager::LexerManager() { loadLexers(); }
LexerManager::~LexerManager() { unloadAll(); }

LexerSpec* LexerManager::lexerSpec(const QString& ext) { return m_lexers.contains(ext) ? m_lexers[ext]->lexerSpec() : 0; }

void LexerManager::pluginLoaded(LexerProvider* plugin)
{
	plugin->init();
	foreach(const QString& ext, plugin->extensions()) m_lexers[ext] = plugin;
}

void LexerManager::pluginUnloaded(LexerProvider* plugin)
{
	foreach(const QString& ext, plugin->extensions()) m_lexers.remove(ext);
}

void LexerManager::loadLexers()
{
	foreach(const QString& str, QDir(LEXER_FOLDER).entryList(QStringList() << (QString("*.") + OS_LIB_EXT))) {
		const QString& base = QFileInfo(str).baseName();
		const QString& withPlugin = base.right(base.length() - 3);
		const QString& name = withPlugin.left(withPlugin.lastIndexOf('_'));
		qWarning() << get(name);
	}
}

QString LexerManager::getExpectedLocation(const QString&) const { return LEXER_FOLDER; }