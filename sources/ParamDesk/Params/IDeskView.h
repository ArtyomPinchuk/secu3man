/* SECU-3  - An open source, free engine control unit
   Copyright (C) 2007 Alexey A. Shabelnikov. Ukraine, Kiev

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

   contacts:
              http://secu-3.org
              email: shabelnikov@secu-3.org
*/

/** \file IDeskView.h
 * \author Alexey A. Shabelnikov
 */

#pragma once

#include "common/FastDelegate.h"
#include "common/unicodesupport.h"

//Base interface for Views
class IDeskView
{
 public:
  typedef fastdelegate::FastDelegate0<> EventHandler;

  virtual bool IsEnabled(void) = 0;                        //Check view if it is enabled or disabled
  virtual void Enable(bool enable) = 0;                    //Enable/disable view
  virtual void Show(bool show) = 0;                        //Show/hide content of view
};
