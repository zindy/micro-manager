/*
 * Galvo device instance wrapper
 *
 * AUTHOR:
 * Mark Tsuchida
 *
 * Copyright (c) 2014 Regents of the University of California,
 * All Rights reserved
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "GalvoInstance.h"


int GalvoInstance::PointAndFire(double x, double y, double time_us) { return GetImpl()->PointAndFire(x, y, time_us); }
int GalvoInstance::SetSpotInterval(double pulseInterval_us) { return GetImpl()->SetSpotInterval(pulseInterval_us); }
int GalvoInstance::SetPosition(double x, double y) { return GetImpl()->SetPosition(x, y); }
int GalvoInstance::GetPosition(double& x, double& y) { return GetImpl()->GetPosition(x, y); }
int GalvoInstance::SetIlluminationState(bool on) { return GetImpl()->SetIlluminationState(on); }
double GalvoInstance::GetXRange() { return GetImpl()->GetXRange(); }
double GalvoInstance::GetXMinimum() { return GetImpl()->GetXMinimum(); }
double GalvoInstance::GetYRange() { return GetImpl()->GetYRange(); }
double GalvoInstance::GetYMinimum() { return GetImpl()->GetYMinimum(); }
int GalvoInstance::AddPolygonVertex(int polygonIndex, double x, double y) { return GetImpl()->AddPolygonVertex(polygonIndex, x, y); }
int GalvoInstance::DeletePolygons() { return GetImpl()->DeletePolygons(); }
int GalvoInstance::RunSequence() { return GetImpl()->RunSequence(); }
int GalvoInstance::LoadPolygons() { return GetImpl()->LoadPolygons(); }
int GalvoInstance::SetPolygonRepetitions(int repetitions) { return GetImpl()->SetPolygonRepetitions(repetitions); }
int GalvoInstance::RunPolygons() { return GetImpl()->RunPolygons(); }
int GalvoInstance::StopSequence() { return GetImpl()->StopSequence(); }

std::string GalvoInstance::GetChannel()
{
   DeviceStringBuffer nameBuf(this, "GetChannel");
   int err = GetImpl()->GetChannel(nameBuf.GetBuffer());
   ThrowIfError(err, "Cannot get current channel name");
   return nameBuf.Get();
}
