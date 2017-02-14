///////////////////////////////////////////////////////////////////////////////
// FILE:          PrecisExcite.cpp
// PROJECT:       Micro-Manager
// SUBSYSTEM:     DeviceAdapters
//-----------------------------------------------------------------------------
// DESCRIPTION:   PrecisExcite controller adapter
// COPYRIGHT:     University of California, San Francisco, 2009
//
// AUTHOR:        Arthur Edelstein, arthuredelstein@gmail.com, 3/17/2009
//
// LICENSE:       This file is distributed under the BSD license.
//                License text is included with the source distribution.
//
//                This file is distributed in the hope that it will be useful,
//                but WITHOUT ANY WARRANTY; without even the implied warranty
//                of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
//                IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//                CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//                INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES.
//
// CVS:           
//



#ifdef WIN32
   #include <windows.h>
   #define snprintf _snprintf 
#endif


#include "../../MMDevice/MMDevice.h"
#include "PrecisExcite.h"
#include <string>
#include <math.h>
#include "../../MMDevice/ModuleInterface.h"
#include "../../MMDevice/DeviceUtils.h"
#include <sstream>

// Controller
const char* g_ControllerName = "PrecisExcite";
const char* g_Keyword_Intensity = "Intensity";
const char* g_Keyword_CssString = "CssString";
const char* g_Keyword_Trigger = "Trigger";
const char* g_Keyword_Trigger_Sequence = "TriggerSequence";
const char* g_Keyword_ChannelLabel = "ChannelLabel";
const char * carriage_return = "\r";
const char * line_feed = "\n";

// static lock
MMThreadLock Controller::lock_;



///////////////////////////////////////////////////////////////////////////////
// Exported MMDevice API
///////////////////////////////////////////////////////////////////////////////
MODULE_API void InitializeModuleData()
{
   RegisterDevice(g_ControllerName, MM::ShutterDevice, "PrecisExcite LED illuminator");
}

MODULE_API MM::Device* CreateDevice(const char* deviceName)
{
   if (deviceName == 0)
      return 0;

   if (strcmp(deviceName, g_ControllerName) == 0)
   {
      // create Controller
      Controller* pController = new Controller(g_ControllerName);
      return pController;
   }

   return 0;
}

MODULE_API void DeleteDevice(MM::Device* pDevice)
{
   delete pDevice;
}

///////////////////////////////////////////////////////////////////////////////
// Controller implementation
// ~~~~~~~~~~~~~~~~~~~~

Controller::Controller(const char* name) :
   initialized_(false), 
   intensity_(0),
   state_(0),
   name_(name), 
   busy_(false),
   error_(0),
   changedTime_(0.0),
   nChannels_(0),
   mThread_(0),
   hasUpdated_(false)
{
   assert(strlen(name) < (unsigned int) MM::MaxStrLength);

   InitializeDefaultErrorMessages();

   // create pre-initialization properties
   // ------------------------------------

   // Name
   CreateProperty(MM::g_Keyword_Name, name_.c_str(), MM::String, true);

   // Description
   CreateProperty(MM::g_Keyword_Description, "PrecisExcite LED Illuminator", MM::String, true);

   // Port
   CPropertyAction* pAct = new CPropertyAction (this, &Controller::OnPort);
   CreateProperty(MM::g_Keyword_Port, "Undefined", MM::String, false, pAct, true);

   EnableDelay(); // signals that the delay setting will be used
   UpdateStatus();
}

Controller::~Controller()
{
   Shutdown();
}

bool Controller::Busy()
{
   MM::MMTime interval = GetCurrentMMTime() - changedTime_;
   MM::MMTime delay(GetDelayMs()*1000.0);
   if (interval < delay)
      return true;
   else
      return false;
}

void Controller::GetName(char* name) const
{
   assert(name_.length() < CDeviceUtils::GetMaxStringLength());
   CDeviceUtils::CopyLimitedString(name, name_.c_str());
}


int Controller::Initialize()
{

   // set property list
   // -----------------
   /*
   MM::Device* serialDevice = GetCoreCallback()->GetDevice(this, port_.c_str());
   if (serialDevice == NULL) {
      LogMessage("Serial port not found");
      return DEVICE_SERIAL_COMMAND_FAILED;
   }
*/
   this->LogMessage("Controller::Initialize()");

   currentChannel_ = 0;

   ReadGreeting();
   int result = ReadChannelLabels();
   if (result != DEVICE_OK)
	   return result;

   channelIntensities_.reserve(nChannels_);
   GenerateChannelChooser();
   GeneratePropertyIntensity();
   GeneratePropertyState();
   GeneratePropertyTrigger();
   GeneratePropertyTriggerSequence();

   //This one is for debugging
   CPropertyAction* pAct = new CPropertyAction (this, &Controller::OnCssString);
   CreateProperty(g_Keyword_CssString, "", MM::String, true, pAct);

   GetUpdate();
   //GetState(state_);
   
   mThread_ = new PollingThread(*this);
   mThread_->Start();

   initialized_ = true;
   return HandleErrors();

}

void Controller::ReadGreeting()
{
   MMThreadGuard myLock(lock_);
   do {
      ReceiveOneLine();
   } while (! buf_string_.empty());
}

void Controller::GetUpdate()
{
   this->LogMessage("Controller::GetUpdate()",true);

   MMThreadGuard myLock(lock_);
   {
      Purge();
      Send("CSS?");
      do {
         ReceiveOneLine();
      } while (0 != buf_string_.compare(0,3,"CSS",0,3));

      //Record intensities and first LED on
      long stateTmp = 0;
      for (unsigned int i=0;i<nChannels_;i++) {
         //Read the intensity
         channelIntensities_[i] = atol(buf_string_.substr(6+i*6,3).c_str());

         //Check if LED is on
         if (buf_string_[i*6+5]=='N' && stateTmp == 0) {
            stateTmp = 1;
            currentChannel_ = i;

            std::ostringstream ss;
            ss << "debug: Found channel " << channelLetters_[i];
            ss << " is ON!";
            LogMessage(ss.str().c_str(), true);
         }
      }

      //Maybe the LEDs are off. Can we still find a selected channel?
      if (stateTmp == 0) {
         for (unsigned int i=0;i<nChannels_;i++) {
            if (buf_string_[i*6+4]=='S') {
               currentChannel_ = i;
               break;
            }
         }
      }

      //the debug string
      StripString(buf_string_);
      cssString_ = buf_string_;

      //SetProperty(g_Keyword_CssString, cssString_.c_str());

      //record state
      state_ = stateTmp;
   }
}

int Controller::ReadChannelLabels()
{
   buf_tokens_.clear();
   string label;

   {
      MMThreadGuard myLock(lock_);
      Purge();

      Send("LAMS");
      do {
         ReceiveOneLine();
         buf_tokens_.push_back(buf_string_);
      }
      while(! buf_string_.empty());
   }
   
   nChannels_ = 0;
   for (unsigned int i=0;i<buf_tokens_.size();i++)
   {
      if (buf_tokens_[i].compare(0,3,"LAM",0,3)==0) {
         string label = buf_tokens_[i].substr(6);
         StripString(label);

         //This skips invalid channels.
         //Invalid names seem to have a different number of dashes.
         //pe2: First invalid is called ----, then second is -----
         if (label.compare(0,4,"----",0,4) == 0)
            continue;

         channelLetters_.push_back(buf_tokens_[i][4]); // Read 4th character
         // This is a temporary log entry to debug an issue with channel labels
         // that appear to contain an invalid character at the end.
         std::ostringstream ss;
         ss << "debug: last char of stripped label is: \'" <<
            static_cast<int>(label[label.size()]) << "\' (as decimal int)";
         LogMessage(ss.str().c_str(), true);

         channelLabels_.push_back(label);
         nChannels_ += 1;
      }
   }

   if (channelLabels_.size() == 0)
	   return DEVICE_ERR;
   else
	   return DEVICE_OK;
}


/////////////////////////////////////////////
// Property Generators
/////////////////////////////////////////////

void Controller::GeneratePropertyState()
{
   CPropertyAction* pAct = new CPropertyAction (this, &Controller::OnState);
   CreateProperty(MM::g_Keyword_State, "0", MM::Integer, false, pAct);
   AddAllowedValue(MM::g_Keyword_State, "0");
   AddAllowedValue(MM::g_Keyword_State, "1");
}

void Controller::GeneratePropertyTrigger()
{
   CPropertyAction* pAct = new CPropertyAction (this, &Controller::OnTrigger);
   CreateProperty("Trigger", "Off", MM::String, false, pAct);
   for (TriggerType i=OFF;i<=FOLLOW_PULSE;i=TriggerType(i+1))
      AddAllowedValue("Trigger", TriggerLabels[i].c_str());
   SetProperty("Trigger","Off");
}

void Controller::GenerateChannelChooser()
{
   if (! channelLabels_.empty()) {   
      CPropertyAction* pAct;
      pAct = new CPropertyAction (this, &Controller::OnChannelLabel);
      CreateProperty(g_Keyword_ChannelLabel, channelLabels_[0].c_str(), MM::String, false, pAct);

      SetAllowedValues(g_Keyword_ChannelLabel, channelLabels_);
      SetProperty(g_Keyword_ChannelLabel, channelLabels_[0].c_str());
            
   }
}

void Controller::GeneratePropertyIntensity()
{
   string intensityName;
   CPropertyActionEx* pAct; 
   for (unsigned i=0;i<nChannels_;i++)
   {
      pAct = new CPropertyActionEx(this, &Controller::OnIntensity, i);
      intensityName = g_Keyword_Intensity;
      intensityName.push_back(channelLetters_[i]);
      CreateProperty(intensityName.c_str(), "0", MM::Integer, false, pAct);
      SetPropertyLimits(intensityName.c_str(), 0, 100);
   }
}



int Controller::Shutdown()
{
   if (initialized_)
   {
      initialized_ = false;
      delete(mThread_);
   }
   return HandleErrors();
}



void Controller::GeneratePropertyTriggerSequence()
{
   int ret;
   CPropertyAction* pAct = new CPropertyAction (this, &Controller::OnTriggerSequence);
   ret = CreateProperty(g_Keyword_Trigger_Sequence, "ABCD0", MM::String, false, pAct);
   SetProperty(g_Keyword_Trigger_Sequence, "ABCD0");
}


///////////////////////////////////////////////////////////////////////////////
// String utilities
///////////////////////////////////////////////////////////////////////////////


void Controller::StripString(string& StringToModify)
{
   if(StringToModify.empty()) return;

   const char* spaces = " \f\n\r\t\v";
   size_t startIndex = StringToModify.find_first_not_of(spaces);
   size_t endIndex = StringToModify.find_last_not_of(spaces);
   string tempString = StringToModify;
   StringToModify.erase();

   StringToModify = tempString.substr(startIndex, (endIndex-startIndex+ 1) );
}





///////////////////////////////////////////////////////////////////////////////
// Action handlers
///////////////////////////////////////////////////////////////////////////////


int Controller::OnPort(MM::PropertyBase* pProp, MM::ActionType eAct)
{
   if (eAct == MM::BeforeGet)
   {
      pProp->Set(port_.c_str());
   }
   else if (eAct == MM::AfterSet)
   {
      if (initialized_)
      {
         // revert
         pProp->Set(port_.c_str());
         return ERR_PORT_CHANGE_FORBIDDEN;
      }

      pProp->Get(port_);
   }

   return HandleErrors();
}

int Controller::OnIntensity(MM::PropertyBase* pProp, MM::ActionType eAct, long index)
{

   long intensity;
   if (eAct == MM::BeforeGet)
   {
      //GetIntensity(intensity,index);
      GetUpdate();
      intensity = channelIntensities_[index];
      pProp->Set(intensity);
   }
   else if (eAct == MM::AfterSet)
   {
      pProp->Get(intensity);
      SetIntensity(intensity, index);
      GetUpdate();
   }
   hasUpdated_ = true;
   

   return HandleErrors();
}


int Controller::OnChannelLabel(MM::PropertyBase* pProp, MM::ActionType eAct)
{
   if (eAct == MM::BeforeGet)
   {
      currentChannelLabel_ = channelLabels_[currentChannel_];
      pProp->Set(currentChannelLabel_.c_str());
   }
   else if (eAct == MM::AfterSet)
   {
      pProp->Get(currentChannelLabel_);
      for (unsigned int i=0;i<nChannels_;i++)
         if (channelLabels_[i].compare(currentChannelLabel_) == 0)
         {
            std::ostringstream ss;
            ss << "Controller::OnChannelLabel(" << i <<")";
            this->LogMessage(ss.str().c_str(), true);
            currentChannel_ = i;
            break;
         }
      Illuminate();

   }

   return HandleErrors();
}



int Controller::OnState(MM::PropertyBase* pProp, MM::ActionType eAct)
{
   if (eAct == MM::BeforeGet)
   {
      pProp->Set(state_);
   }
   else if (eAct == MM::AfterSet)
   {
      pProp->Get(state_);
      SetState(state_);
   }
   
   hasUpdated_ = true;
   return HandleErrors();
}


int Controller::OnTrigger(MM::PropertyBase* pProp, MM::ActionType eAct)
{

   if (eAct == MM::BeforeGet)
   {
      pProp->Set(trigger_.c_str());
   }
   else if (eAct == MM::AfterSet)
   {
      char cmd=0;
      pProp->Get(trigger_);
      for (int i=0;i<5;i++)
      {
         if (trigger_.compare(TriggerLabels[i])==0)
         {
            cmd = TriggerCmd[i];
            triggerMode_ = (TriggerType) i;
         }
      }
      
      SetTrigger();

   }
   return HandleErrors();
}


int Controller::OnTriggerSequence(MM::PropertyBase* pProp, MM::ActionType eAct)
{
   if (eAct == MM::BeforeGet)
   {
      pProp->Set(triggerSequence_.c_str());
   }
   else if (eAct == MM::AfterSet)
   { 
      pProp->Get(triggerSequence_);
      SetTrigger();
   }
   return HandleErrors();
}

int Controller::OnCssString(MM::PropertyBase* pProp, MM::ActionType eAct)
{
   if (eAct == MM::BeforeGet)
   {
      pProp->Set(cssString_.c_str());
   }
   else if (eAct == MM::AfterSet)
   {
   }
   return DEVICE_OK;
}



///////////////////////////////////////////////////////////////////////////////
// Utility methods
///////////////////////////////////////////////////////////////////////////////

void Controller::SetTrigger()
{
   stringstream msg;

   msg << "SQX" << carriage_return;

   for (unsigned int i=0;i<triggerSequence_.size();i++)
   {
      msg << "SQ" << triggerSequence_[i] << carriage_return;
   }

   triggerMessage_ = msg.str();

   Illuminate();

}

void Controller::Illuminate()
{
   this->LogMessage("Controller::Illuminate()",true);
   MMThreadGuard myLock(lock_);
   Purge();

   stringstream msg;
   if (state_==0)
   {
      if (triggerMode_ == OFF) {
         for (int i=0; i<channelLetters_.size(); i++) {
            msg.str("");
            msg << "C" << channelLetters_[i] << "F";
            Send(msg.str());
            ReceiveOneLine();
        }

      }
   }
   else if (state_==1)
   {
      if (triggerMode_ == OFF) {
         for (int i=0; i<channelLetters_.size(); i++) {
            msg.str("");
            msg << "C" << channelLetters_[i];

            if (i == currentChannel_)
                msg << "N";
            else
                msg << "F";

            Send(msg.str());
            ReceiveOneLine();
        }

      }
   }
}

void Controller::SetIntensity(long intensity, long index)
{
   this->LogMessage("Controller::SetIntensity()",true);
   stringstream msg;
   msg << "C" << channelLetters_[index] << "I" << intensity;

   {
      MMThreadGuard myLock(lock_);
      Purge();
      Send(msg.str());
      ReceiveOneLine();
   }
}

void Controller::GetIntensity(long& intensity, long index)
{
   this->LogMessage("Controller::GetIntensity()",true);
   stringstream msg;
   string ans;
   msg << "C" << channelLetters_[index] << "?";

   {
      MMThreadGuard myLock(lock_);
      Purge();
      Send(msg.str());
      ReceiveOneLine();
   }

   if (! buf_string_.empty())
      if (0 == buf_string_.compare(0,2,msg.str(),0,2))
      {
         intensity = atol(buf_string_.substr(2,3).c_str());
      }

}

void Controller::SetState(long state)
{
   std::ostringstream ss;
   ss << "Controller::SetState(" << state <<") (" << ((state==0)?"closed)":"open)");
   this->LogMessage(ss.str().c_str(), true);

   {
      MMThreadGuard myLock(lock_);
      state_ = state;
      Illuminate();
   }

   // Set timer for the Busy signal
   changedTime_ = GetCurrentMMTime();
}

void Controller::GetState(long &state)
{
   this->LogMessage("Controller::GetState()",true);
   state = state_;
}

int Controller::HandleErrors()
{
   int lastError = error_;
   error_ = 0;
   return lastError;
}



/////////////////////////////////////
//  Communications
/////////////////////////////////////


void Controller::Send(string cmd)
{
   int ret = SendSerialCommand(port_.c_str(), cmd.c_str(), line_feed);
   if (ret!=DEVICE_OK)
      error_ = DEVICE_SERIAL_COMMAND_FAILED;
}


void Controller::ReceiveOneLine()
{
   buf_string_ = "";
   GetSerialAnswer(port_.c_str(), line_feed, buf_string_);

}

void Controller::Purge()
{
   int ret = PurgeComPort(port_.c_str());
   if (ret!=0)
      error_ = DEVICE_SERIAL_COMMAND_FAILED;
}

//********************
// Shutter API
//********************

int Controller::SetOpen(bool open)
{
   this->LogMessage("Controller::SetOpen()",true);
   SetState((long) open);
   return HandleErrors();
}

int Controller::GetOpen(bool& open)
{
   this->LogMessage("Controller::GetOpen()",true);

   long state;
   GetState(state);
   if (state==1)
      open = true;
   else if (state==0)
      open = false;
   else
      error_ = DEVICE_UNKNOWN_POSITION;

   std::ostringstream ss;
   ss << "Controller::GetOpen() sends back: " << open << ((state==0)?" (closed)":" (open)");
   this->LogMessage(ss.str().c_str(), true);
   return HandleErrors();
}

int Controller::Fire(double deltaT)
{
   deltaT=0; // Suppress warning
   error_ = DEVICE_UNSUPPORTED_COMMAND;
   return HandleErrors();
}

PollingThread::PollingThread(Controller& aController) :
   state_(0),
   aController_(aController)
{
}

PollingThread::~PollingThread()
{
   Stop();
   wait();
}

int PollingThread::svc() 
{
   long state;
   long oldState;
   long oldChannel;

   aController_.GetUpdate();
   oldChannel = aController_.currentChannel_;
   while (!stop_)
   {
      //trying to catch changes generated from within MMStudio
      //no need to fire back an event to MMStudio in that case
      //FIXME Does not seem to work for state!
      if (aController_.hasUpdated_) {
         aController_.hasUpdated_ = false;
         oldChannel = aController_.currentChannel_;
         oldState = aController_.state_;
         continue;
      }

      aController_.GetUpdate();
      if (aController_.state_!=oldState)
      {
         oldState = aController_.state_;
         aController_.OnPropertyChanged(MM::g_Keyword_State, CDeviceUtils::ConvertToString(state));
      }
      if (aController_.currentChannel_ != oldChannel)
      {
         oldChannel = aController_.currentChannel_;
         aController_.OnPropertyChanged(g_Keyword_ChannelLabel, aController_.channelLabels_[aController_.currentChannel_].c_str());
      }

      //aController_.OnPropertyChanged(g_Keyword_CssString, aController_.cssString_.c_str());
      CDeviceUtils::SleepMs(500);
   }
   return DEVICE_OK;
}


void PollingThread::Start()
{
   stop_ = false;
   activate();
}


