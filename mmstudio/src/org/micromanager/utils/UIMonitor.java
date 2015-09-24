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

import java.awt.AWTEvent;
import java.awt.Button;
import java.awt.Component;
import java.awt.Toolkit;
import java.awt.Window;
import java.awt.event.AWTEventListener;
import java.awt.event.ActionEvent;
import java.awt.event.MouseEvent;
import javax.swing.AbstractButton;
import javax.swing.JCheckBoxMenuItem;
import javax.swing.JList;
import javax.swing.JSlider;
import javax.swing.JTabbedPane;
import javax.swing.JToggleButton;
import javax.swing.SwingUtilities;

/**
 *
 * @author Arthur
 */
public class UIMonitor {

   private static AWTEventListener clickListener_ = null;

   private static String getComponentText(Component component) {
      if (component instanceof AbstractButton) {
         return ((AbstractButton) component).getText();
      } else if (component instanceof Button) {
         return ((Button) component).getLabel();
      } else {
         return "";
      }
   }

   private static String getComponentName(Component component) {
      String text = component.getName();
      if (text == null || text.isEmpty()) {
         text = getComponentText(component);
      }
      return text;
   }

   private static String getClickAction(Component component) {
      if (component instanceof JToggleButton) {
         return "toggled " + (((JToggleButton) component).isSelected() ? "on" : "off");
      } else if (component instanceof JCheckBoxMenuItem) {
         return "toggled " + (((JCheckBoxMenuItem) component).isSelected() ? "on" : "off");
      } else if (component instanceof JList) {
          return null;
      } else if (component instanceof JSlider) {
         return "set to \"" + ((JSlider) component).getValue() + "\"";
      } else if (component instanceof JTabbedPane) {
         JTabbedPane tabbedPane = (JTabbedPane) component;
         return "set to \"" + tabbedPane.getTitleAt(tabbedPane.getSelectedIndex()) + "\"";
      } else {
         return "clicked";
      }
   }

   private static String getReleaseAction(Component component) {
      if (component instanceof JList) {
         try {
            final JList list = (JList) component;
            return "set to " + "\"" + list.getSelectedValue().toString() + "\"";
         } catch (NullPointerException npe) {
            ReportingUtils.logError("NullPOinterException in UIMonitor-getReleaseAction");
         }
      }
      return null;
   }
   

   private static void handleAWTEvent(AWTEvent event) {
      final int eventID = event.getID();
      String identifier = null;
      String action = null;

      if (0 != (eventID & (ActionEvent.ACTION_EVENT_MASK))) {
         Object source = event.getSource();

         if (source instanceof Component) {
            Component component = (Component) source;
            if (component.isEnabled()) {
               try {
                  String text = getComponentName(component);
                  if (text != null && !text.isEmpty()) {
                     text = "\"" + text + "\" ";
                  }
                  identifier = source.getClass().getSimpleName() + " " + text;
                  if (eventID == MouseEvent.MOUSE_CLICKED) {
                     action = getClickAction(component);
                  }
                  if (eventID == MouseEvent.MOUSE_RELEASED) {
                     action = getReleaseAction(component);
                  }
               } catch (NullPointerException npe) {
                  ReportingUtils.logError("Null pointer Exception in UIMonitor-handleAWTEvent");
               }
            }
            if (identifier != null && action != null) {
               try {
                  String message = "[UI] " + identifier + action + " in " + ((Window) SwingUtilities.getAncestorOfClass(Window.class, (Component) source)).getClass().getSimpleName() + ".";
                  ReportingUtils.logMessage(message);
               } catch (NullPointerException npe) {}
            }
         }
      }
   }

   private static void enable() {
      if (clickListener_ != null) {
         return;
      }
      clickListener_ = new AWTEventListener() {
         @Override
         public void eventDispatched(AWTEvent event) {
            handleAWTEvent(event);
         }
      };
      Toolkit.getDefaultToolkit().addAWTEventListener(clickListener_, AWTEvent.MOUSE_EVENT_MASK);
   }

   private static void disable() {
      Toolkit.getDefaultToolkit().removeAWTEventListener(clickListener_);
      clickListener_ = null;
   }

   public static void enable(final boolean on) {
      SwingUtilities.invokeLater(
              new Runnable() {
                 @Override
                 public void run() {
                    if (on) {
                       enable();
                    } else {
                       disable();
                    }
                 }
              });
   }
}
