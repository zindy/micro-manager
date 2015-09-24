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

import java.awt.Component;

import javax.swing.JTable;
import javax.swing.table.TableCellRenderer;


public class SliderCellRenderer implements TableCellRenderer {
	// This method is called each time a cell in a column
	// using this renderer needs to be rendered.

	public SliderCellRenderer() {
        super();
	}

	public Component getTableCellRendererComponent(JTable table, Object value,
			boolean isSelected, boolean hasFocus, int rowIndex, int colIndex) {

		PMTDataModel data = (PMTDataModel)table.getModel();
		
		Component comp;
      SliderPanel slider = new SliderPanel();
      slider.setLimits(data.getMinValue(rowIndex), data.getMaxValue(rowIndex));
      double setting = data.getPMTSetting(rowIndex);
      slider.setText(Double.toString(setting));
      comp = slider;
		return comp;
	}

	// The following methods override the defaults for performance reasons
	public void validate() {}
	public void revalidate() {}
	protected void firePropertyChange(String propertyName, Object oldValue, Object newValue) {}
	public void firePropertyChange(String propertyName, boolean oldValue, boolean newValue) {}

}
