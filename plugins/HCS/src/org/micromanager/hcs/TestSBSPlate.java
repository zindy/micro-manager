/*
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, June 3, 2008
 *
 * Copyright (c) 2008 100X Imaging Inc, www.100ximaging.com
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

package org.micromanager.hcs;

import org.micromanager.api.MultiStagePosition;

public class TestSBSPlate {

   public static void main(String[] args) {

      SBSPlate plate = new SBSPlate();
      plate.initialize(SBSPlate.SBS_96_WELL);
      WellPositionList wpl[] = plate.generatePositions(SBSPlate.DEFAULT_XYSTAGE_NAME);
      System.out.println("Created plate with " + wpl.length + " wells.");
      for (int i=0; i<wpl.length; i++) {
         for (int j=0; j<wpl[i].getSitePositions().getNumberOfPositions(); j++) {
            MultiStagePosition mps = wpl[i].getSitePositions().getPosition(j);
            System.out.println("Well " + mps.getLabel() + " : X=" + mps.getX() + ", Y=" + mps.getY());
         }
      }
   }

}
