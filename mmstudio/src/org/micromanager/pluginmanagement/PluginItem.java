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

package org.micromanager.pluginmanagement;

import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.regex.Pattern;

import org.micromanager.api.MMBasePlugin;
import org.micromanager.api.MMPlugin;
import org.micromanager.api.MMProcessorPlugin;
import org.micromanager.MMStudio;
import org.micromanager.utils.ReportingUtils;

/**
 * Utility class used to to assemble information about the plugin
 */
public class PluginItem {
   // raw class as input by caller
   private Class<?> pluginClass_ = null; 
   // MMBasePlugin instance generated in PluginItem
   private MMBasePlugin plugin_ = null;
   // Enum indicating the type of this plugin for when we need to treat
   // different kinds differently (e.g. standard plugin vs. ProcessorPlugin).
   private PluginType pluginType_ = PluginType.PLUGIN_STANDARD;
   // className deduced from pluginClass
   private String className_ = "";
   // menuText deduced from plugin_
   private String menuItem_ = "undefined";
   // tooltip deduced from plugin_
   private String tooltip_ = "";
   // dir in which the class lives (relative to plugin root dir)
   private String directory_ = "";
   // message generated during inspection of pluginClass_
   private String msg_ = "";

   public PluginItem(Class<?> pluginClass, String className, 
         PluginType pluginType, String menuItem, String tooltip, 
         String directory, String msg) {
      pluginClass_ = pluginClass;
      className_ = className;
      pluginType_ = pluginType;
      menuItem_ = menuItem;
      tooltip_ = tooltip;
      directory_ = directory;
      msg_ = msg;
   }
   
   public PluginItem(PluginItem pio) {
      pluginClass_ = pio.pluginClass_;
      className_ = pio.className_;
      pluginType_ = pio.pluginType_;
      menuItem_ = pio.menuItem_;
      plugin_ = pio.plugin_;
      directory_ = pio.directory_;
      msg_ = pio.msg_;
   }
   
   public PluginType getPluginType() {return pluginType_; }
   public String getMenuItem() { return menuItem_; }
   public String getMessage() { return msg_; }
   public String getClassName() { return className_; }
   public String getTooltip() {return tooltip_; }
   public MMBasePlugin getPlugin() {return plugin_; }

   /**
    * Return the menu hierarchy path, including the leaf item name.
    */
   public List<String> getMenuPath() {
      final String sepPat = Pattern.quote(File.separator);
      List<String> menuPath = new ArrayList<String>(Arrays.asList(directory_.split(sepPat)));
      if (directory_.equals("")) {
         // String.split returns a length 1 array containing the empty
         // string when invoked on the empty string. We don't want that.
         menuPath.clear();
      }
      for (int i = 0; i < menuPath.size(); i++) {
         menuPath.set(i, menuPath.get(i).replace('_', ' '));
      }
      menuPath.add(menuItem_);
      return menuPath;
   }
   
   public void instantiate() {
      try {
         if (plugin_ == null) {
            switch (pluginType_) {
               case PLUGIN_STANDARD:
                  plugin_ = (MMPlugin) pluginClass_.newInstance();
                  break;
               case PLUGIN_PROCESSOR:
                  plugin_ = (MMProcessorPlugin) pluginClass_.newInstance();
                  break;
               default:
                  ReportingUtils.logError("Can't instantiate unrecognized plugin type " + pluginType_);
            }
         }
      } catch (InstantiationException e) {
         ReportingUtils.logError("Failed instantiating plugin: " + e);
      } catch (IllegalAccessException e) {
         ReportingUtils.logError("Failed instantiating plugin: " + e);
      }
      if (pluginType_ == PluginType.PLUGIN_STANDARD) {
         ((MMPlugin) plugin_).setApp(MMStudio.getInstance());
      }
   }

   public void dispose() {
      if (pluginType_ == PluginType.PLUGIN_STANDARD && 
            plugin_ != null) {
         ((MMPlugin) plugin_).dispose();
      }
   }
}
