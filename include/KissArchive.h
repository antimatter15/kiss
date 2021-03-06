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

#ifndef __KISSARCHIVE_H__
#define __KISSARCHIVE_H__

#include <QStringList>
#include <QIODevice>

#define KISS_ARCHIVE_VERSION 	1
#define KISS_ARCHIVE_FILE 	"installed"

/*! \struct KissReturn
 * \brief Holds a KissArchive command return value
 */
struct KissReturn 
{
	KissReturn(const bool& error, const QString& message = "") : error(error), message(message) {}
	
	/*! True if error occurred */
	bool error;
	/*! Holds message of error if an error occurred */
	QString message;
};

/*! \class KissArchive
 * \brief Holds methods to manipulate Kiss Archives
 */
class KissArchive 
{
public:
	/*! 
	 * Creates a kiss archive
	 * \param name Name of Package
	 * \param version Version of Package, for upgrades
	 * \param platforms 3 character code for platform(s) "osx", "win", "nix"
	 * \param files files, relatived
	 * \param out Output IODevice
	 * \return see KissReturn
	 *//*! 
	 * Installs a kiss archive. Stores information in file called "installed"
	 * \param in kiss archive pointer
	 * \return see KissReturn
	 */
	static KissReturn create(const QString& name, unsigned version, 
		const QStringList& platforms, const QStringList& files, 
		QIODevice* out);
	/*! 
	 * Installs a kiss archive. Stores information in file called "installed"
	 * \param in kiss archive pointer
	 * \return see KissReturn
	 */
	static KissReturn install(QIODevice* in);
	static KissReturn uninstall(const QString& name);
	static QStringList list(QIODevice* in);
	static const unsigned version(const QString& name);
	static QStringList installed();
};

#endif