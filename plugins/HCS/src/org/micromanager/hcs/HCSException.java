/*
 * AUTHOR:
 * Nenad Amodaj, nenad@amodaj.com, June 9, 2008
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

public class HCSException extends Exception {
   private static final long serialVersionUID = -8829247065013272369L;
   private Throwable cause;

   /**
    * Constructs a MMAcqDataException with an explanatory message.
    * @param message - the reason for the exception.
    */
   public HCSException(String message) {
      super(message);
   }

   public HCSException(Throwable t) {
      super(t.getMessage());
      this.cause = t;
   }

   @Override
   public Throwable getCause() {
      return this.cause;
   }
}
