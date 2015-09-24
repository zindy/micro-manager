/*
 * Copyright (c) 2005-2015 Regents of the University of California
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


package org.micromanager.utils;

import java.awt.Color;
import java.awt.Component;
import java.awt.Font;
import javax.swing.JLabel;
import javax.swing.JTable;
import org.micromanager.ConfigGroupPad;

/**
 *
 * @author arthur
 */
/**
 * Rendering element for the property table.
 *
 */
public class StateGroupCellRenderer extends PropertyValueCellRenderer {

    private static final long serialVersionUID = 1L;
    // This method is called each time a cell in a column
    // using this renderer needs to be rendered.
    StateItem stateItem_;

   @Override
    public Component getTableCellRendererComponent(JTable table, Object value,
            boolean isSelected, boolean hasFocus, int rowIndex, int colIndex) {

        ConfigGroupPad.StateTableData data = (ConfigGroupPad.StateTableData) table.getModel();

        stateItem_ = data.getPropertyItem(rowIndex);

        Component comp;


        JLabel label = new JLabel();
        label.setOpaque(true);
        label.setFont(new Font("Arial", Font.BOLD, 11));
        label.setText((String) value);
        label.setToolTipText(stateItem_.descr);
        label.setHorizontalAlignment(JLabel.LEFT);
        comp = label;

        
        if (isSelected) {
            comp.setBackground(Color.LIGHT_GRAY);
        } else {
            comp.setBackground(Color.WHITE);
        }

        return comp;
    }
      // The following methods override the defaults for performance reasons
   @Override
      public void validate(){}
   @Override
      public void revalidate(){}
   @Override
      protected void firePropertyChange(String propertyName, Object oldValue, Object newValue) {}
   @Override
      public void firePropertyChange(String propertyName, boolean oldValue, boolean newValue) {}

   }
