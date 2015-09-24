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

///////////////////////////////////////////////////////////////////////////////
//FILE:          WaitDialog.java
//PROJECT:       Micro-Manager
//SUBSYSTEM:     mmstudio
//-----------------------------------------------------------------------------
//
// CVS:          $Id$
//
package org.micromanager.utils;
import java.awt.Color;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.Toolkit;

import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JWindow;

/**
 * @author Jurij Henne,Maxim Bauer
 *
 * This is an implementation of a "Loading.."(or just "please wait") dialog. It pops up, when the GUI invokes some 
 * backend operations, like search or similar. 
 */
public class WaitDialog  extends JWindow  {
   private static final long serialVersionUID = 5356404305699524826L;
   // single instance of this class, used through out the scope of the application
	  private final static Cursor defaultCursor=Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR);
	  private final static Cursor waitCursor=Cursor.getPredefinedCursor( Cursor.WAIT_CURSOR );

	  public WaitDialog(String message) {
         JPanel root = new JPanel();
         root.setBorder(BorderFactory.createLineBorder(new Color(0,0,0)));
         JLabel label = new JLabel(message);
         Dimension labelSize = label.getPreferredSize(); 
         root.setPreferredSize(new Dimension(labelSize.width+26,labelSize.height+16));
         root.add(label);
         this.setSize(root.getPreferredSize());                            
         this.getContentPane().add(root);
         Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
         this.setLocation(screenSize.width/2 - (labelSize.width/2),
                           screenSize.height/2 - (labelSize.height/2));
	  }

	  /**
	   * This static method uses pre-created dialog, positions it in the center
	   * and displays it to the user.
	   */
	  public void showDialog() {	
	      setCursor( waitCursor );  
	  	   setVisible(true);
	  	   paint(getGraphics());
                   update(getGraphics());
	  }

	  /**

	   * This static method closes the wait dialog.
	   */
	  public void closeDialog()   {
	     setCursor(defaultCursor );  
	  	  dispose();
	  }
	 
}
	
