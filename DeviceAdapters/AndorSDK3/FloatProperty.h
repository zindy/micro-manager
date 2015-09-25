/*
 * Copyright (c) 2007, Regents of the University of California
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

#ifndef _FLOATPROPERTY_H_
#define _FLOATPROPERTY_H_

#include "atcore++.h"
#include "MMDeviceConstants.h"
#include "Property.h"

class ICallBackManager;

class TFloatProperty : public andor::IObserver
{
public:
   TFloatProperty(const std::string & MM_name,
                  andor::IFloat* float_feature,
                  ICallBackManager* callback,
                  bool readOnly, bool needsCallBack);
   ~TFloatProperty();

protected:
   void Update(andor::ISubject* Subject);
   int OnFloat(MM::PropertyBase* pProp, MM::ActionType eAct);
   typedef MM::Action<TFloatProperty> CPropertyAction;

private:
   void setFeatureWithinLimits(double new_value);

private:
   andor::IFloat* float_feature_;
   ICallBackManager* callback_;
   std::string MM_name_;
   bool callbackRegistered_;
   static const int DEC_PLACES_ERROR = 4;
};

class TFloatStringProperty : public andor::IObserver
{
public:
   TFloatStringProperty(const std::string & MM_name,
                  andor::IFloat* float_feature,
                  ICallBackManager* callback,
                  bool readOnly, bool needsCallBack);
   ~TFloatStringProperty();

   void Update(andor::ISubject* Subject);
   int OnFStrChangeRefresh(MM::PropertyBase* pPropBase, MM::ActionType eAct);
protected:
   typedef MM::Action<TFloatStringProperty> CPropertyAction;

private:
   andor::IFloat* float_feature_;
   ICallBackManager* callback_;
   std::string MM_name_;
   std::string displayStrValue_;
   bool callbackRegistered_;
};



class TAndorFloatFilter : public andor::IFloat
{
public:
   TAndorFloatFilter(andor::IFloat* _float):m_float(_float){}
   virtual ~TAndorFloatFilter() {};
   double Get() {return m_float->Get();}
   void Set(double Value){m_float->Set(Value);}
   double Max() {return m_float->Max();}
   double Min() {return m_float->Min();}
   bool IsImplemented(){return m_float->IsImplemented();}
   bool IsReadable(){return m_float->IsReadable();}
   bool IsWritable(){return m_float->IsWritable();}
   bool IsReadOnly(){return m_float->IsReadOnly();}
   void Attach(andor::IObserver* _observer){m_float->Attach(_observer);}
   void Detach(andor::IObserver* _observer){m_float->Detach(_observer);}

protected:
   andor::IFloat* m_float;
};

class TAndorFloatValueMapper : public TAndorFloatFilter
{
public:
   TAndorFloatValueMapper(andor::IFloat* _float, double _factor)
      :TAndorFloatFilter(_float), m_factor(_factor)
   {
   }
   ~TAndorFloatValueMapper() {}

   double Get() {return m_float->Get()*m_factor;}
   void Set(double Value) {m_float->Set(Value/m_factor);}
   double Max() {return m_float->Max()*m_factor;}
   double Min() {return m_float->Min()*m_factor;}

protected:
   double m_factor;
};

class TAndorFloatCache : public TAndorFloatFilter
{
public:
   TAndorFloatCache(andor::IFloat* _float)
      : TAndorFloatFilter(_float),
        cached_float(0.00)
   {
      cached_float = m_float->Get();
   }

   double Get(bool sscPoised)
   {
      if (sscPoised) 
      {
         return cached_float;
      }
      else 
      {
         return m_float->Get();
      }
   }

   void Set(double Value)
   {
      cached_float = Value;
      m_float->Set(Value);
   }

   void SetCache(double Value)
   {
      cached_float = Value;
   }

private:
   double cached_float;
};


#endif // _FLOATPROPERTY_H_
