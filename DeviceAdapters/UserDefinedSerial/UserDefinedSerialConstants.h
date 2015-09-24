/*
 * Control devices using user-specified serial commands
 *
 * AUTHOR:
 * Mark Tsuchida
 *
 * Copyright (c) 2014 Regents of the University of California
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

#pragma once


// Make sure to add message to UserDefSerialBase::RegisterErrorMessages() when
// adding a new code.
const int ERR_BINARY_SERIAL_TIMEOUT = 107; // Use the famous SerialManager code
const int ERR_UNEXPECTED_RESPONSE = 2001;
const int ERR_QUERY_COMMAND_EMPTY = 2002;
const int ERR_ASCII_COMMAND_CONTAINS_NULL = 2003;
const int ERR_TRAILING_BACKSLASH = 2004;
const int ERR_UNKNOWN_ESCAPE_SEQUENCE = 2005;
const int ERR_EMPTY_HEX_ESCAPE_SEQUENCE = 2006;
const int ERR_CANNOT_GET_PORT_TIMEOUT = 2007;
const int ERR_CANNOT_QUERY_IN_IGNORE_MODE = 2008;
const int ERR_EXPECTED_RESPONSE_LENGTH_MISMATCH = 2009;
const int ERR_NO_RESPONSE_ALTERNATIVES = 2010;
const int ERR_VAR_LEN_RESPONSE_MUST_NOT_BE_EMPTY = 2011;


const char* const g_DeviceName_GenericDevice = "UserDefinedGenericDevice";
const char* const g_DeviceName_Shutter = "UserDefinedShutter";
const char* const g_DeviceName_StateDevice = "UserDefinedStateDevice";


const char* const g_PropName_CommandSendMode = "Command mode";
const char* const g_PropName_ResponseDetectionMethod = "Response detection";

const char* const g_PropName_InitializeCommand = "Initialize-command";
const char* const g_PropName_InitializeResponse = "Initialize-response";
const char* const g_PropName_ShutdownCommand = "Shutdown-command";
const char* const g_PropName_ShutdownResponse = "Shutdown-response";

const char* const g_PropName_OpenCommand = "Open-command";
const char* const g_PropName_OpenResponse = "Open-response";
const char* const g_PropName_CloseCommand = "Close-command";
const char* const g_PropName_CloseResponse = "Close-response";
const char* const g_PropName_QueryStateCommand = "QueryState-command";
const char* const g_PropName_QueryOpenResponse = "QueryState-open-response";
const char* const g_PropName_QueryCloseResponse =
   "QueryState-closed-response";

const char* const g_PropName_NumPositions = "Number of positions";
const char* const g_PropNamePrefix_SetPositionCommand = "SetPosition-command-";
const char* const g_PropNamePrefix_SetPositionResponse = "SetPosition-response-";
const char* const g_PropName_QueryPositionCommand = "QueryPosition-command";
const char* const g_PropNamePrefix_QueryPositionResponse =
   "QueryPosition-response-";

const char* const g_PropValue_ASCII_NoTerminator = "ASCII-no-terminator";
const char* const g_PropValue_ASCII_CRLF = "ASCII-CRLF-terminator";
const char* const g_PropValue_ASCII_CR = "ASCII-CR-terminator";
const char* const g_PropValue_ASCII_LF = "ASCII-LF-terminator";
const char* const g_PropValue_Binary = "Binary";

const char* const g_PropValue_ResponseIgnore = "Ignore responses";
const char* const g_PropValuePrefix_ResponseTerminated = "Terminator-";
const char* const g_PropValue_ResponseCRLFTerminated = "Terminator-CRLF";
const char* const g_PropValue_ResponseCRTerminated = "Terminator-CR";
const char* const g_PropValue_ResponseLFTerminated = "Terminator-LF";
const char* const g_PropValuePrefix_ResponseFixedByteCount =
   "Fixed-length binary-";
const char* const g_PropValue_ResponseVariableByteCount =
   "Variable-length binary";
