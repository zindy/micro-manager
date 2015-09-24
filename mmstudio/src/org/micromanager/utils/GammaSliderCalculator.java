/*
 * AUTHOR:
 * Nico Stuurman, nico@cmp.ucsf.edu., November 2009
 * Algorithm thought up by Bob Brown (Loveland, CO)
 *
 * Copyright (c)  Regents of the University of California
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

import java.util.Hashtable;
import java.text.ParseException;

public class GammaSliderCalculator {
   final double c0_ = 5.0 * 0.02;
   final double c1_ = 5.0 * 0.00542857;
   final double c2_ = 5.0 * 0.00297143;
   final double c3_ = 5.0 * 0.000628571;

   double min_ = 0.1;
   double max_ = 5;
   int low_ = 0;
   int high_ = 100;

   Hashtable<Integer, Double> gammas_;
   Hashtable<Double, Integer>  values_;

   public GammaSliderCalculator(int low, int high) {
      gammas_ = new Hashtable<Integer, Double>(high - low + 1, 1.0f);
      values_ = new Hashtable<Double, Integer>(high - low + 1, 1.0f);
      for (int i = low; i <= high; i++) {
         try {
            double gamma = NumberUtils.displayStringToDouble(NumberUtils.doubleToDisplayString(calculateGamma(i)));
            gammas_.put(new Integer(i), gamma);
            values_.put(gamma, new Integer(i));
         } catch(ParseException p) {
            ReportingUtils.logMessage("Caught ParseException");
         }
      }
      low_ = low;
      high_ = high;
      min_ = gammas_.get(low);
      max_ = gammas_.get(high);
   }

   public double sliderToGamma(int sliderSetting) {
      // TODO: catch problems not finding these values
      return gammas_.get(sliderSetting);
   }

   public int gammaToSlider(double gamma) {
      if (gamma < min_)
         gamma = min_;
      if (gamma > max_)
         gamma = max_;
      try {
         gamma = NumberUtils.displayStringToDouble(NumberUtils.doubleToDisplayString(gamma));
         Integer test = values_.get(gamma);
         if (test != null)
            return test;
      } catch (ParseException p) {
         ReportingUtils.logMessage("Caught ParseException");
      }
      int guess = (low_ + high_) / 2;
      int lastGuess = low_;
      int diff = guess;
      int i = 0;
      while (diff > 1 && i < 10) {
         diff = Math.abs(guess - lastGuess);
         lastGuess = guess;
         if (gamma > gammas_.get(guess))
             guess += diff/2;
         else
            guess -= diff/2;
         i++;
      }
      return guess;
   }

   private double calculateGamma(int sliderSetting) {
      double x = (double) sliderSetting / 10.0;
      return calculateGamma10(x);
   }

   private double calculateGamma10(double x) {
      return  c0_ + (c1_ * x) + (c2_ * x * x) + (c3_ * x * x * x) ;
   }

   /*
    * Not used, but nice example of estimate a value from a function
    */
   private int calculateSlider(double gamma) {
      double estimate = 5.0;
      int i=0;
      while (Math.abs(calculateGamma10(estimate) - gamma) > 0.002 && i < 5) {
         double denominator = (3 * c3_ * estimate * estimate) + (2 * c2_ * estimate) + c1_;
         estimate -= (calculateGamma10(estimate) - gamma) / denominator;
         i++;                                                                
      }                                                                      
      return (int) (10 * estimate);                                          
   }
}
