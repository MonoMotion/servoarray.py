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

#include "servoarray/servoarray.h"
#include "servoarray/subscript_wrapper.h"

#include <cassert>

namespace ServoArray {

ServoArray::ServoArray(std::shared_ptr<Driver> driver, const std::vector<double>& offsets) : driver_(driver), offsets_(offsets) {
  this->cache_.resize(this->size());
  this->offsets_.resize(this->size());
}

ServoArray::ServoArray(const std::string& name, const DriverParams& params, DriverManager& manager) : driver_(manager.load(name, params)) {
  this->cache_.resize(this->size());
  this->offsets_.resize(this->size());

  for (const auto& p : manager.config().offset().offsets()) {
    if (p.first >= this->size()) {
      // TODO: ignore this with warning
      throw std::runtime_error("Offset index out of range");
    }

    this->offsets_[p.first] = p.second;
  }
}

void ServoArray::write(std::size_t index, double rad) {
  auto const value = this->offsets_[index] + rad;
  this->driver_->write(index, value);
  this->cache_[index] = value;
}

double ServoArray::read(std::size_t index) {
  switch(this->read_mode_) {
    case ReadMode::Cached:
      return this->cache_[index] - this->offsets_[index];
    case ReadMode::Direct:
      return this->driver_->read(index) - this->offsets_[index];
    default:
      assert(false); // unreachable
  }
}

SubscriptWrapper ServoArray::operator[](std::size_t index) {
  return {this, index};
}

void ServoArray::set_read_mode(ReadMode mode) {
  this->read_mode_ = mode;
  // TODO: Print warning if mode == Direct and the driver doesn't support read
}

ReadMode ServoArray::read_mode() const {
  return this->read_mode_;
}

double ServoArray::offset(std::size_t idx) const {
  return this->offsets_[idx];
}

void ServoArray::set_offset(std::size_t idx, double value) {
  this->offsets_[idx] = value;
}

std::size_t ServoArray::size() const {
  return this->driver_->size();
}

}
