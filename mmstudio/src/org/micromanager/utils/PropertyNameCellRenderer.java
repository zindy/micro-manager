/*
 * AUTHOR:
 * Arthur Edelstein, 2009
 *
 * Copyright (c) 2009 Regents of the University of California
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
import javax.swing.JLabel;
import javax.swing.JTable;
import javax.swing.table.TableCellRenderer;

/**
 *
 * @author arthur
 */
public class PropertyNameCellRenderer implements TableCellRenderer {

    PropertyItem item_;

    @Override
    public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int rowIndex, int column) {
       MMPropertyTableModel data = (MMPropertyTableModel) table.getModel();
       item_ = data.getPropertyItem(rowIndex);
       JLabel lab = new JLabel();
       lab.setOpaque(true);
       lab.setHorizontalAlignment(JLabel.LEFT);
       lab.setText((String) value);

       if (item_.readOnly) {
           //comp.setForeground(Color.DARK_GRAY);
           lab.setBackground(Color.LIGHT_GRAY);
        } else {
           //comp.setForeground(Color.BLACK);
           lab.setBackground(Color.WHITE);
        }    
        return lab;
    }

    // The following methods override the defaults for performance reasons
    public void validate() {}
    public void revalidate() {}
    protected void firePropertyChange(String propertyName, Object oldValue, Object newValue) {}
    public void firePropertyChange(String propertyName, boolean oldValue, boolean newValue) {}
}
