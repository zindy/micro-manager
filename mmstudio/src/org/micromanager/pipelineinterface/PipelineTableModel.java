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

package org.micromanager.pipelineinterface;

import com.google.common.eventbus.Subscribe;
import java.util.List;
import javax.swing.table.AbstractTableModel;
import mmcorej.TaggedImage;
import org.micromanager.acquisition.AcquisitionEngine;
import org.micromanager.api.DataProcessor;
import org.micromanager.events.EventManager;
import org.micromanager.events.PipelineEvent;
import org.micromanager.events.ProcessorEnabledEvent;


public class PipelineTableModel extends AbstractTableModel {
   static final int ENABLED_COLUMN = 0;
   static final int NAME_COLUMN = 1;
   static final int CONFIGURE_COLUMN = 2;
   private static final int NUM_COLUMNS = 3;

   private final AcquisitionEngine engine_;
   private List<DataProcessor<TaggedImage>> pipeline_;

   PipelineTableModel(AcquisitionEngine engine) {
      engine_ = engine;
      pipeline_ = engine_.getImageProcessorPipeline();
      EventManager.register(this);
   }

   @Subscribe
   public void pipelineChanged(PipelineEvent event) {
      pipeline_ = event.getPipeline();
      // It is not ideal that we redraw the entire table, but since we don't
      // (yet) have a way to receive insert/delete information as events, it
      // is the simplest thing to do at the moment.
      fireTableDataChanged();
   }

   @Subscribe
   public void processorEnabledChanged(ProcessorEnabledEvent event) {
      int i = pipeline_.indexOf(event.getProcessor());
      if (i >= 0) {
         fireTableCellUpdated(i, ENABLED_COLUMN);
      }
   }

   @Override
   public int getRowCount() {
      return pipeline_.size();
   }

   @Override
   public int getColumnCount() {
      return NUM_COLUMNS;
   }

   @Override
   public Class<?> getColumnClass(int column) {
      switch (column) {
         case ENABLED_COLUMN:
            return Boolean.class;
         case NAME_COLUMN:
            return String.class;
         case CONFIGURE_COLUMN:
            return DataProcessor.class;
      }
      return Object.class;
   }

   @Override
   public String getColumnName(int column) {
      switch (column) {
         case ENABLED_COLUMN:
            return "Enabled";
         case NAME_COLUMN:
            return "Processor";
         case CONFIGURE_COLUMN:
            return "Settings";
      }
      return "";
   }

   @Override
   public boolean isCellEditable(int row, int column) {
      switch (column) {
         case ENABLED_COLUMN: return true;
         case NAME_COLUMN: return false;
         case CONFIGURE_COLUMN: return true;
      }
      return false;
   }

   @Override
   public Object getValueAt(int row, int column) {
      switch (column) {
         case ENABLED_COLUMN:
            return pipeline_.get(row).getIsEnabled();
         case NAME_COLUMN:
            return getProcessorName(pipeline_.get(row));
         case CONFIGURE_COLUMN:
            return pipeline_.get(row);
      }
      return null;
   }

   @Override
   public void setValueAt(Object value, int row, int column) {
      if (column == ENABLED_COLUMN) {
         boolean enabled = (Boolean) value;
         pipeline_.get(row).setEnabled(enabled);
      }
   }

   private String getProcessorName(DataProcessor<TaggedImage> processor) {
      @SuppressWarnings("unchecked")
      Class<? extends DataProcessor<TaggedImage>> procCls
            = (Class) processor.getClass();
      return engine_.getNameForProcessorClass(procCls);
   }
}
