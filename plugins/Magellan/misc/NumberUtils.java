/*
 * AUTHOR:
 * Nico Stuurman, nico@cmp.ucsf.edu, March 21, 2009
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

package misc;

import java.math.RoundingMode;
import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.text.ParseException;
import java.util.Locale;

public class NumberUtils {
	private static final NumberFormat format_;
	private static final DecimalFormat coreDoubleFormat_;
	private static final DecimalFormat coreIntegerFormat_;

	static {
		// The display is supposed to use local formating (e.g., switch commas with periods in Locale.GERMANY).
		format_ = NumberFormat.getInstance();
		format_.setRoundingMode(RoundingMode.HALF_UP);
      format_.setMaximumFractionDigits(4);

		// The core always uses four decimal places in its double strings, and a dot for the decimal separator.
		// This is equivalent to the US locale settings.
		coreDoubleFormat_ = (DecimalFormat) DecimalFormat.getInstance(Locale.US);
		coreDoubleFormat_.setRoundingMode(RoundingMode.HALF_UP);
		coreDoubleFormat_.applyPattern("0.0000"); 
		
		coreIntegerFormat_ = (DecimalFormat) DecimalFormat.getInstance(Locale.US);
		coreIntegerFormat_.applyPattern("0");
	}

	// Display string methods
	
	public static String intToDisplayString(int number) {
		return format_.format(number);
	}
   
   public static String longToDisplayString(long number) {
		return format_.format(number);
	}

	public static String doubleToDisplayString(double number) {
		return format_.format(number);
	}

	public static int displayStringToInt(Object numberString) throws ParseException {
		return format_.parse((String) numberString).intValue();
	}

   public static long displayStringToLong(Object numberString) throws ParseException {
		return format_.parse((String) numberString).longValue();
	}
   
	public static double displayStringToDouble(Object numberString) throws ParseException {
		return format_.parse((String) numberString).doubleValue();
	}

	
    // Core string methods
	
	public static String intToCoreString(long number) {
		return coreIntegerFormat_.format(number);
	}

   public static String longToCoreString(long number) {
      return coreIntegerFormat_.format(number);
   }

	public static String doubleToCoreString(double number) {
		return coreDoubleFormat_.format(number);
	}

	public static int coreStringToInt(Object numberString) throws ParseException {
		return coreIntegerFormat_.parse((String) numberString).intValue();
	}

   public static long coreStringToLong(Object numberString) throws ParseException {
		return coreIntegerFormat_.parse((String) numberString).longValue();
	}

	public static double coreStringToDouble(Object numberString) throws ParseException {
		return coreDoubleFormat_.parse((String) numberString).doubleValue();
	}

	
	// Conversion between display and core strings.
	
	public static String doubleStringDisplayToCore(Object numberDouble) throws ParseException {
		return doubleToCoreString(displayStringToDouble(numberDouble));
	}

	public static String doubleStringCoreToDisplay(Object numberDouble) throws ParseException {
		return doubleToDisplayString(coreStringToDouble(numberDouble));
	}

	public static String intStringDisplayToCore(Object numberInt) throws ParseException {
		return intToCoreString(displayStringToInt(numberInt));
	}

	public static String intStringCoreToDisplay(Object numberInt) throws ParseException {
		return intToDisplayString(coreStringToInt(numberInt));
	}



}
