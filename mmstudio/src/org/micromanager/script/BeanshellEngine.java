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

package org.micromanager.script;

import org.micromanager.utils.MMScriptException;

import bsh.EvalError;
import bsh.Interpreter;
import bsh.ParseException;
import bsh.TargetError;

public class BeanshellEngine implements ScriptingEngine {
   Interpreter interp_;
   boolean running_ = false;
   boolean error_ = false;
   EvalThread evalThd_;
   boolean stop_ = false;
   private ScriptingGUI gui_;
   private Interpreter interp_old_;

   public class EvalThread extends Thread {
      String script_;
      String errorText_;

      public EvalThread(String script) {
         script_ = script;
         errorText_ = new String();
      }

      @Override
      public void run() {
         stop_ = false;
         running_ = true;
         errorText_ = new String();
         try {
            interp_.eval(script_);
         } catch (TargetError e){
            int lineNo = e.getErrorLineNumber(); 
            gui_.displayError(formatBeanshellError(e, lineNo), lineNo);
         } catch (ParseException e) {
            // special handling of the parse errors beacuse beanshell error object
            // has bugs and does not return line numbers
            String msg = e.getMessage();
            String lineNumberTxt = msg.substring(20, msg.indexOf(','));
            gui_.displayError("Parse error: " + msg, Integer.parseInt(lineNumberTxt));
         } catch (EvalError e) {
            int lineNo = e.getErrorLineNumber(); 
            gui_.displayError(formatBeanshellError(e, lineNo), lineNo);
         } finally {
            running_ = false;
         }
      }

      public String getError() {
         return errorText_;
      }
   }

   
   @Override
   public void setInterpreter(Interpreter interp) {
	   interp_old_ = interp_;
	   interp_ = interp;
   }
   
   @Override
   public void resetInterpreter() {
	   interp_ = interp_old_;
   }
   
   public BeanshellEngine(ScriptingGUI gui) {
      //interp_ = new Interpreter();
      running_ = false;
      evalThd_ = new EvalThread("");
      gui_ = gui;
   }

   @Override
   public void evaluate(String script) throws MMScriptException {
      try {
         interp_.eval(script);
    	 // interp_.set("micro_manager_script",script);
      } catch (EvalError e) {
         throw new MMScriptException(formatBeanshellError(e, e.getErrorLineNumber()));
      }
   }

   @Override
   public void joinEvalThread() throws InterruptedException {
      if (evalThd_.isAlive()) {
         evalThd_.join();
      }
   }
   
   @Override
   public void evaluateAsync(String script) throws MMScriptException {
      if (evalThd_.isAlive())
         throw new MMScriptException("Another script execution in progress!");

      evalThd_ = new EvalThread(script);
      evalThd_.start();
   }

   @Override
   public void insertGlobalObject(String name, Object obj) throws MMScriptException {
      try {
         interp_.set(name, obj);
      } catch (EvalError e) {
         throw new MMScriptException(e);
      }
   }

   @SuppressWarnings("deprecation")
   @Override
   public void stopRequest(boolean shouldInterrupt) {
      if (evalThd_.isAlive()) {
         if (shouldInterrupt) {
            evalThd_.interrupt();
         }
         else {
            // HACK: kill the thread.
            evalThd_.stop();
            stop_ = true;
         }
      }
   }

   @Override
   public boolean stopRequestPending() {
      if (evalThd_.isAlive() && stop_)
         return stop_;
      stop_ = false;
      return stop_;
   }
   
   private String formatBeanshellError(EvalError e, int line) {
      if (e instanceof TargetError) {
         Throwable t = ((TargetError)e).getTarget();
         return "Line " + line + ": run-time error : " + (t != null ? t.getMessage() : e.getErrorText());       
      } else if (e instanceof ParseException) {
         return "Line " + line + ": syntax error : " + e.getErrorText();         
      } else {
         Throwable t = e.getCause();
         return "Line " + line + ": general error : " + (t != null ? t.getMessage() : e.getErrorText());
      }
      
   }

   @Override
   public void sleep(long ms) throws MMScriptException {
      try {
         Thread.sleep(ms);
      } catch (InterruptedException e) {
         throw new MMScriptException	("Execution interrupted by the user");
      }
   }
}
