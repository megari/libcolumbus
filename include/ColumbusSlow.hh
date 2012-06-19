/*
 * Copyright (C) 2012 Canonical, Ltd.
 *
 * Authors:
 *    Jussi Pakkanen <jussi.pakkanen@canonical.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * This file contains helper functions that for some reason or
 * another must include libc++ stuff. Whenever this file is included
 * compilation times slow down.
 *
 * If a function can be removed from here, it should.
 */

#ifndef COLUMBUSSLOW_HH
#define COLUMBUSSLOW_HH

#include<string>

std::string findDataFile(std::string baseName);

#endif
