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

#ifndef __PLUGINMANAGER_H__
#define __PLUGINMANAGER_H__

#include "Kiss.h"
#include "Singleton.h"

#include <QMap>
#include <QPluginLoader>
#include <QString>
#include <QDir>
#include <QDebug>

#include <typeinfo>

typedef QMap<QString, QPluginLoader*> PluginMap;

/*! \class PluginManager
 * \brief Manages loading/unloading of Qt plugins.
 *
 * Takes M, the name of the Manager, and T, the name of the Plugin Interface
 * Maps the plugins to PluginMap, expecting each to be referenced using a unique name.
 */
template<typename M, typename T>
class PluginManager : public Singleton<M>
{
public:
	//! Get plugin instance, loads if not found
	T* get(const QString& name) {
		if(!m_plugins.contains(name)) 
			if(!loadPlugin(name)) return 0;
		return qobject_cast<T*>(m_plugins[name]->instance());
	}

	//! Unloads all plugins
	void unloadAll() { while (!m_plugins.isEmpty()) unloadPlugin(m_plugins.begin().key()); }
	
	//! Loads plugin by given name
	bool loadPlugin(const QString& name) {
		if(m_plugins.contains(name)) return false;

		QPluginLoader* plugin = new QPluginLoader();
		QDir pluginPath(QDir::currentPath());
		pluginPath.cd(getExpectedLocation(name));
		

		const QString& pluginPathString = pluginPath.absoluteFilePath("lib" + name + "_plugin." + OS_LIB_EXT);
		qWarning() << "Path:" << pluginPathString;

		plugin->setFileName(pluginPathString);
		if(!plugin->load()) {
			qWarning("PluginManager::loadPlugin: %s", qPrintable(plugin->errorString()));
			delete plugin;
			return false;
		}

		T* instance = qobject_cast<T*>(plugin->instance());
		if(!instance) {
			qWarning("PluginManager::loadPlugin: %s", qPrintable(plugin->errorString()));
			plugin->unload();
			delete plugin;
			qWarning("PluginManager::loadPlugin: Plugin \"%s\" failed qobject_cast to \"%s\"", qPrintable(name), typeid(T).name());
			return false;
		}

		m_plugins[name] = plugin;
		
		qWarning() << "Calling pluginLoaded";
		pluginLoaded(instance);
		return true;
	}

	//! Unloads plugin by given name
	void unloadPlugin(const QString& name) {
		if(!m_plugins.contains(name)) return;
		pluginUnloaded(qobject_cast<T*>(m_plugins[name]->instance()));
		QPluginLoader* plugin = m_plugins.take(name);
		if(plugin) {
			qWarning() << "Unloaded" << name;
			plugin->unload();
			delete plugin;
		}
	}
	
protected:
	virtual QString getExpectedLocation(const QString& name) const = 0;
	virtual void pluginLoaded(T* plugin) = 0;
	virtual void pluginUnloaded(T* plugin) = 0;
	
private:
	PluginMap m_plugins;
};

#endif 
