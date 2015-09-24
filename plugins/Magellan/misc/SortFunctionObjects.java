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

package misc;

import java.text.ParseException;
import java.util.Comparator;

public class SortFunctionObjects
{

/* comparators for GUI ordering of numeric strings
 * *
 */
// Comparators for sorting of the possible values
public static class IntStringComp implements Comparator<String>{
 @Override
 public int compare(String strA, String strB)
 {
	 int valA = 0;
	 int valB = 0;
	 try {
		valA = NumberUtils.coreStringToInt(strA);
		valB = NumberUtils.coreStringToInt(strB);
	 
	} catch (ParseException e) {
        Log.log(e);
	}
     return valA - valB;
 }
}
public static class DoubleStringComp implements Comparator<String>{
 @Override
 public int compare(String strA, String strB)
 {
	 double valA = 0.;
	 double valB = 0.;
	 try {
		valA = NumberUtils.coreStringToDouble(strA);
		valB = NumberUtils.coreStringToDouble(strB);
	 
	} catch (ParseException e) {
        Log.log(e);
	}
     return Double.compare(valA, valB);
 }
}
public static class NumericPrefixStringComp implements Comparator<String>{
	private int NumericPrefix( String str0){
		int ret = 0;
		int ii;
		for (ii= 0; ii < str0.length(); ++ii ){
			if( !Character.isDigit(str0.charAt(ii))) break;
		}
		if( 0 < ii)
			ret = Integer.valueOf(str0.substring(0,ii));
		return ret;
		
	}
   @Override
	 public int compare(String strA, String strB) {
	     return NumericPrefix(strA) - NumericPrefix(strB) ;
	 }
}

}
