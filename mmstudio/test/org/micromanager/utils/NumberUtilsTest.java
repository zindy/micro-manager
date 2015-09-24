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

import org.junit.Before;
import org.junit.Test;
import static org.hamcrest.CoreMatchers.*;
import static org.junit.Assert.*;

public class NumberUtilsTest {
   @Before
   public void beforeMethod() {
      // For now, the tests in this calss are written with the assumption that
      // the decimal point is '.'. Skip if this is not true.
      java.util.Locale loc = java.util.Locale.getDefault();
      org.junit.Assume.assumeTrue(new java.text.DecimalFormatSymbols(loc).
            getDecimalSeparator() == '.');
   }

   @Test
   public void doubleToStringIsCorrect() {
      // Whether we should be displaying "-0" is questionable. For now, test
      // for the existing behavior.
      assertThat(NumberUtils.doubleToDisplayString(0.0), anyOf(is("0"), is("-0")));
      assertThat(NumberUtils.doubleToDisplayString(0.00004), anyOf(is("0"), is("-0")));
      assertThat(NumberUtils.doubleToDisplayString(-0.00004), anyOf(is("0"), is("-0")));

      assertEquals("0.0001", NumberUtils.doubleToDisplayString(0.00005));
      assertEquals("-0.0001", NumberUtils.doubleToDisplayString(-0.00005));

      assertEquals("1", NumberUtils.doubleToDisplayString(1.0));
      assertEquals("1", NumberUtils.doubleToDisplayString(1.00004));
      assertEquals("1", NumberUtils.doubleToDisplayString(0.99995));
      assertEquals("1.0001", NumberUtils.doubleToDisplayString(1.00005));
      assertEquals("0.9999", NumberUtils.doubleToDisplayString(0.99994));

      assertEquals("-1", NumberUtils.doubleToDisplayString(-1.0));
      assertEquals("-1", NumberUtils.doubleToDisplayString(-1.00004));
      assertEquals("-1", NumberUtils.doubleToDisplayString(-0.99995));
      assertEquals("-1.0001", NumberUtils.doubleToDisplayString(-1.00005));
      assertEquals("-0.9999", NumberUtils.doubleToDisplayString(-0.99994));
   }

   @Test
   public void doubleToCoreStringIsCorrect() {
      assertThat(NumberUtils.doubleToCoreString(0.0), anyOf(is("0.0000"), is("-0.0000")));
      assertThat(NumberUtils.doubleToCoreString(0.00004), anyOf(is("0.0000"), is("-0.0000")));
      assertThat(NumberUtils.doubleToCoreString(-0.00004), anyOf(is("0.0000"), is("-0.0000")));

      assertEquals("0.0001", NumberUtils.doubleToCoreString(0.00005));
      assertEquals("-0.0001", NumberUtils.doubleToCoreString(-0.00005));

      assertEquals("1.0000", NumberUtils.doubleToCoreString(1.0));
      assertEquals("1.0000", NumberUtils.doubleToCoreString(1.00004));
      assertEquals("1.0000", NumberUtils.doubleToCoreString(0.99995));
      assertEquals("1.0001", NumberUtils.doubleToCoreString(1.00005));
      assertEquals("0.9999", NumberUtils.doubleToCoreString(0.99994));

      assertEquals("-1.0000", NumberUtils.doubleToCoreString(-1.0));
      assertEquals("-1.0000", NumberUtils.doubleToCoreString(-1.00004));
      assertEquals("-1.0000", NumberUtils.doubleToCoreString(-0.99995));
      assertEquals("-1.0001", NumberUtils.doubleToCoreString(-1.00005));
      assertEquals("-0.9999", NumberUtils.doubleToCoreString(-0.99994));
   }

   @Test
   public void displayToDoubleIsCorrect() throws java.text.ParseException {
      final double delta = 0.0000001;

      // displayStringToDouble() doesn't perform rounding, but do a sanity
      // check

      assertEquals(0.0, NumberUtils.displayStringToDouble("0"), delta);
      assertEquals(0.0, NumberUtils.displayStringToDouble("0.0"), delta);
      assertEquals(0.0, NumberUtils.displayStringToDouble("-0"), delta);
      assertEquals(0.0, NumberUtils.displayStringToDouble("-0.0"), delta);

      assertEquals(0.00004, NumberUtils.displayStringToDouble("0.00004"), delta);
      assertEquals(0.00005, NumberUtils.displayStringToDouble("0.00005"), delta);
      assertEquals(-0.00004, NumberUtils.displayStringToDouble("-0.00004"), delta);
      assertEquals(-0.00005, NumberUtils.displayStringToDouble("-0.00005"), delta);

      assertEquals(1.00004, NumberUtils.displayStringToDouble("1.00004"), delta);
      assertEquals(1.00005, NumberUtils.displayStringToDouble("1.00005"), delta);
      assertEquals(0.99995, NumberUtils.displayStringToDouble("0.99995"), delta);
      assertEquals(0.99994, NumberUtils.displayStringToDouble("0.99994"), delta);

      assertEquals(-1.00004, NumberUtils.displayStringToDouble("-1.00004"), delta);
      assertEquals(-1.00005, NumberUtils.displayStringToDouble("-1.00005"), delta);
      assertEquals(-0.99995, NumberUtils.displayStringToDouble("-0.99995"), delta);
      assertEquals(-0.99994, NumberUtils.displayStringToDouble("-0.99994"), delta);
   }
}
