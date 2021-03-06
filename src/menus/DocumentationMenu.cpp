/**************************************************************************
 *  Copyright 2007-2012 KISS Institute for Practical Robotics             *
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

#include "DocumentationMenu.h"

#ifdef BUILD_DOCUMENTATION_TAB

#include "TargetMenu.h"
#include "MainWindow.h"
#include "Documentation.h"

DocumentationMenu::DocumentationMenu(MainWindow* mainWindow) : ConcreteMenuable(menuName()), m_mainWindow(mainWindow)
{
	MenuNode* targetMenu = new MenuNode("Target");
	MenuNode* doc = node(action("report", "Documentation"));
	targetMenu->children.append(MenuNode::separator());
	targetMenu->children.append(doc);
	m_actions.append(targetMenu);
	
	m_help.append(doc);
	m_help.append(MenuNode::separator());
}

void DocumentationMenu::triggered()
{
	QList<Documentation*> docs = m_mainWindow->tabs<Documentation>();
	if(docs.size() >= 1) m_mainWindow->moveToTab(docs[0]);
	else m_mainWindow->addTab(new Documentation(m_mainWindow));
}

QString DocumentationMenu::menuName()
{
	return "Documentation";
}

#endif