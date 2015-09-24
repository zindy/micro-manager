/*
 * Mightex Polygon400 plugin.
 *
 * AUTHOR:
 * Wayne Liao, mightexsystem.com, 05/15/2015
 *
 * Copyright (c) 2015 Mightex Systems
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

package org.micromanager.polygon;

import com.google.common.io.Files;
import java.io.File;
import java.io.IOException;

/**
 *
 * @author Wayne
 */
public class UnDo {
    private int first_;
    private int last_;
    private int state_;
    
    private File path_;
    
    private LoadSave ls_;
    
    public boolean create( LoadSave ls )
    {
        ls_ = ls;
        first_ = last_ = state_ = 0;
        path_ = Files.createTempDir();
        return path_.exists();
    }
    
    public void destory()
    {
        if( path_ != null ){
            Utility.DeleteFolder( path_ );
            path_ = null;
        }
    }
    
    public boolean CanUnDo()
    {
        return first_ != state_;
    }
    
    public boolean CanReDo()
    {
        return first_ != last_ && state_ != last_ -1;
    }
    
    public boolean UnDo()
    {
        if( !CanUnDo() )
            return false;
        return LoadState( state_ - 1 );
    }
    
    public boolean ReDo()
    {
        if( !CanReDo() )
            return false;
        return LoadState( state_ + 1 );
    }
    
    public boolean SaveState()
    {
        if( CanReDo() )
            DupState();
        return SaveState( last_ );
    }
    
    private boolean SaveState( int state )
    {
        String fn = path_ + "\\" + state + ".undo";
        if( !ls_.Save( fn ) )
            return false;
        
        state_ = state;
        last_ = state_ + 1;
        return true;
    }
    
    private boolean LoadState( int state )
    {
        String fn = path_ + "\\" + state + ".undo";
        if( !ls_.Load( fn ) )
            return false;
        
        state_ = state;
        return true;
    }
    
    private boolean DupState()
    {
        String fn = path_ + "\\" + state_ + ".undo";
        String fn2 = path_ + "\\" + last_ + ".undo";
        try{
            Files.copy( new File(fn), new File(fn2) );
            last_++;
            return true;
        }
        catch( IOException ex ){
            return false;
        }
    }
}
