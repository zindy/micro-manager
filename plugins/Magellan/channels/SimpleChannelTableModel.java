/*
 * AUTHOR:
 * Henry Pinkard, henry.pinkard@gmail.com
 *
 * Copyright (c) 2015 Regents of the University of California
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

package channels;

import com.google.common.eventbus.Subscribe;
import java.awt.Color;
import java.util.ArrayList;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;
import javax.swing.table.AbstractTableModel;
import main.Magellan;
import misc.GlobalSettings;
import mmcorej.CMMCore;
import org.micromanager.api.events.ExposureChangedEvent;

/**
 *
 * @author Henry
 */
public class SimpleChannelTableModel extends AbstractTableModel implements TableModelListener {

   
      private ArrayList<ChannelSetting> channels_ = new ArrayList<ChannelSetting>();
      private final CMMCore core_;
      private String channelGroup_ = null;
      private final boolean exploreTable_;
      public final String[] COLUMN_NAMES = new String[]{
         "Use",
         "Configuration",
         "Exposure",
          "Color"
   };
      
      
   public SimpleChannelTableModel(ArrayList<ChannelSetting> channels) {
      exploreTable_ = true;
      core_ = Magellan.getCore();   
      channels_ = channels;
      Magellan.getScriptInterface().registerForEvents(this);
   }

   public SimpleChannelTableModel() {
      exploreTable_ = false;
      core_ = Magellan.getCore();
      refreshChannels();
      Magellan.getScriptInterface().registerForEvents(this);
   }
   
   public void shutdown() {
      Magellan.getScriptInterface().unregisterForEvents(this);
   }
   
   public boolean anyChannelsActive() {
      for (ChannelSetting c : channels_) {
         if (c.use_) {
            return true;
         }
      }
      return false;
   }

   public void setChannelGroup(String group) {
      channelGroup_ = group;
   }
   
   public void setChannels(ArrayList<ChannelSetting> channels) {
      channels_ = channels;
   }
   
   public ArrayList<ChannelSetting> getChannels() {
       return channels_;
   }

   public void refreshChannels() {
      channels_ = ChannelUtils.getAvailableChannels(channelGroup_);
   }
      
   public String[] getActiveChannelNames() {
      int count = 0;
      for (ChannelSetting c : channels_) {
         count += c.use_ ? 1 : 0;
      }
      
      String[] channelNames = new String[count];
      for (int i = 0; i < channelNames.length; i++) {
         channelNames[i] = channels_.get(i).name_;
      }
      return channelNames;
   }

   @Override
   public int getRowCount() {
      if (channels_ == null) {
         return 0;
      } else {
         return channels_.size();
      }
   }

   @Override
   public int getColumnCount() {
      return COLUMN_NAMES.length - (exploreTable_ ? 1 : 0);
   }

   @Override
   public String getColumnName(int columnIndex) {
      return COLUMN_NAMES[columnIndex];
   }

   @Override
   public Object getValueAt(int rowIndex, int columnIndex) {
            //use name exposure, color
      if (columnIndex == 0) {
         return channels_.get(rowIndex).use_;
      } else if (columnIndex == 1) {
         return channels_.get(rowIndex).name_;
      } else if (columnIndex == 2) {
         return channels_.get(rowIndex).exposure_;
      } else {
         return channels_.get(rowIndex).color_;
      }
   }

   @Override
   public Class getColumnClass(int columnIndex) {
            if (columnIndex == 0) {
         return Boolean.class;
      } else if (columnIndex == 1) {
         return String.class;
      } else if (columnIndex == 2) {
         return Double.class;
      } else {
         return Color.class;
      }
   }

   @Override
   public void setValueAt(Object value, int row, int columnIndex) {
      //use name exposure, color  
      int numCamChannels = (int) (GlobalSettings.getInstance().getDemoMode() ? 6 : core_.getNumberOfCameraChannels());
      
      if (columnIndex == 0) {                   
         channels_.get(row).use_ = (Boolean) value;
         //same for all other channels of the same camera_
         if (numCamChannels > 1) {
            for (int i = (int) (row - row % numCamChannels); i < (row /numCamChannels + 1) * numCamChannels;i++ ) {
               channels_.get(i).use_ = (Boolean) value;
            }
            fireTableDataChanged();
         }       
      } else if (columnIndex == 1) {       
         //cant edit channel name
      } else if (columnIndex == 2) {
         channels_.get(row).exposure_ = Double.parseDouble((String) value);
         //same for all other channels of the same camera_
         if (numCamChannels > 1) {
            for (int i = (int) (row - row % numCamChannels); i < (row / numCamChannels + 1) * numCamChannels; i++) {
               channels_.get(i).exposure_ = Double.parseDouble((String) value);
            }
            fireTableDataChanged();
         }
      } else {
         channels_.get(row).color_ = (Color) value;
      }
      ChannelUtils.storeChannelInfo(channels_);
   }

   @Override
   public boolean isCellEditable(int nRow, int nCol) {
      return nCol != 1;
   }

   @Override
   public void tableChanged(TableModelEvent e) {
   }

    @Subscribe
    public void onExposureChanged(ExposureChangedEvent event) {
        for (ChannelSetting c : channels_) {
            c.exposure_ = event.getNewExposureTime();
        }
        fireTableDataChanged();
    }



 }

