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

package propsandcovariants;

import java.text.DecimalFormat;

/**
 *
 * @author Henry
 */
public class CovariantValue  implements Comparable<CovariantValue>{
   
   private double doubleValue_;
   private int intValue_;
   private String stringValue_;
   private CovariantType type_;
   
   CovariantValue(double d) {
      doubleValue_ = d;
      type_ = CovariantType.DOUBLE; 
   }
   
   CovariantValue(int i) {
      intValue_ = i;
      type_ = CovariantType.INT; 
   }
   
   CovariantValue(String s) {
      stringValue_ = s;
      type_ = CovariantType.STRING;
   }
   
   public CovariantType getType() {
      return type_;
   }

   @Override
   public String toString() {
      if (stringValue_ != null) {
         return stringValue_;
      } else if (type_ == CovariantType.DOUBLE) {
          return new DecimalFormat("0.0000").format(doubleValue_); //formatted correctly for the core
      } else {
         return intValue_ + "";
      }
   }

   public double doubleValue() {
      return doubleValue_;
   }
   
   public int intValue() {
      return intValue_;
   }

   String stringValue() {
      return stringValue_;
   }

   @Override
   public boolean equals(Object o) {
      if (!(o instanceof CovariantValue)) {
         return false;
      }
      return this.compareTo((CovariantValue)o) == 0;
   }
   
   @Override
   public int compareTo(CovariantValue o) {
      if (type_ == CovariantType.STRING) {
         return this.stringValue().compareTo(o.stringValue());
      } else if (type_ == CovariantType.DOUBLE) {
         return new Double(this.doubleValue_).compareTo(o.doubleValue_);
      } else {
         return new Integer(this.intValue_).compareTo(o.intValue_);
      }
   }

   void restrainWithinLimits(Covariant cov) {
      if (cov.hasLimits()) {
         if (type_ == CovariantType.INT) {
            intValue_ = Math.min(cov.getUpperLimit().intValue(), Math.max(intValue_, cov.getLowerLimit().intValue()));
            
         } else if (type_ == CovariantType.DOUBLE) {            
            doubleValue_ = Math.min(cov.getUpperLimit().doubleValue(), Math.max(doubleValue_, cov.getLowerLimit().doubleValue()));
         }
      }
   }
}
