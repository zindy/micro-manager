/*
 * AUTHOR:
 * Nenad Amodaj
 *
 * Copyright (c) 2011 Nenad Amodaj
 * Copyright (c) 2009 100X Imaging Inc
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.imaging100x.twophoton;

public class PMTSetting {
   public String name;
   public double volts;
   
   public PMTSetting() {
      name = new String("undef");
      volts = 0.0;
   }
}
