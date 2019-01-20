// This file is part of servoarray.
//
// servoarray is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// servoarray is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with servoarray.  If not, see <http://www.gnu.org/licenses/>.

#ifndef SERVOARRAY_DRIVER_H
#define SERVOARRAY_DRIVER_H

#include <unistd.h>

namespace ServoArray {

// ABC for drivers
class Driver {
public:
  virtual void set(std::size_t, double) = 0;
  virtual double get(std::size_t) const = 0;

  virtual std::uint8_t size() const = 0;
};

//
// Driver .so must contain these symbols:
// Driver* servoarray_driver(void*);
// void servoarray_cleanup(Driver*);
//

}

#endif