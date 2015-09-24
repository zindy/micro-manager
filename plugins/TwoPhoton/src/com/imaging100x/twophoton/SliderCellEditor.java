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
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.AbstractCellEditor;
import javax.swing.JTable;
import javax.swing.table.TableCellEditor;

public class SliderCellEditor extends AbstractCellEditor implements TableCellEditor {

    private static final long serialVersionUID = 1L;
    SliderPanel slider_ = new SliderPanel();
    TwoPhotonControl app_;

    public SliderCellEditor(TwoPhotonControl app) {
        super();
        app_ = app;

        slider_.addEditActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                fireEditingStopped();
            }
        });

        slider_.addSliderMouseListener(new MouseAdapter() {
            public void mouseReleased(MouseEvent e) {
                fireEditingStopped();
                if (app_ != null)
                   app_.removeDepthListSelection();
            }
        });
    }

    // This method is called when a cell value is edited by the user.
    public Component getTableCellEditorComponent(JTable table, Object value,
            boolean isSelected, int rowIndex, int colIndex) {

        PMTDataModel data = (PMTDataModel) table.getModel();
        double setting = data.getPMTSetting(rowIndex);

        slider_.setLimits(data.getMinValue(rowIndex), data.getMaxValue(rowIndex));
        slider_.setText(Double.toString(setting));
        return slider_;
   }

    // This method is called when editing is completed.
    // It must return the new value to be stored in the cell.
    public Object getCellEditorValue() {
       return slider_.getText();
    }
}
