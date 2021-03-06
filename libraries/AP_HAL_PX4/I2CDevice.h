/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
/*
 * Copyright (C) 2015-2016  Intel Corporation. All rights reserved.
 *
 * This file is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <inttypes.h>

#include <AP_HAL/HAL.h>
#include <AP_HAL/I2CDevice.h>
#include <AP_HAL/utility/OwnPtr.h>
#include <AP_HAL_Empty/AP_HAL_Empty.h>
#include <AP_HAL_Empty/AP_HAL_Empty_Private.h>
#include "I2CWrapper.h"

namespace PX4 {

class I2CDevice : public AP_HAL::I2CDevice {
public:
    static I2CDevice *from(AP_HAL::I2CDevice *dev)
    {
        return static_cast<I2CDevice*>(dev);
    }

    I2CDevice(uint8_t bus, uint8_t address);
    ~I2CDevice();

    /* See AP_HAL::I2CDevice::set_address() */
    void set_address(uint8_t address) override { _address = address; }

    /* See AP_HAL::I2CDevice::set_retries() */
    void set_retries(uint8_t retries) override { _retries = retries; }

    /* See AP_HAL::Device::set_speed(): Empty implementation, not supported. */
    bool set_speed(enum Device::Speed speed) override { return true; }

    /* See AP_HAL::Device::transfer() */
    bool transfer(const uint8_t *send, uint32_t send_len,
                  uint8_t *recv, uint32_t recv_len) override;

    bool read_registers_multiple(uint8_t first_reg, uint8_t *recv,
                                 uint32_t recv_len, uint8_t times) override;

    /* See AP_HAL::Device::register_periodic_callback() */
    AP_HAL::Device::PeriodicHandle *register_periodic_callback(
        uint32_t period_usec, AP_HAL::MemberProc) override
    {
        /* Not implemented yet */
        return nullptr;
    };

    // this makes no sense on PX4 
    int get_fd() override { return -1; }

    AP_HAL::Semaphore* get_semaphore() override { return &semaphore; }

private:
    // we use an empty semaphore as the underlying I2C class already has a semaphore
    Empty::Semaphore semaphore;
    PX4_I2C _bus;
    uint8_t _address;
    uint8_t _retries = 0;
};

class I2CDeviceManager : public AP_HAL::I2CDeviceManager {
public:
    friend class I2CDevice;

    static I2CDeviceManager *from(AP_HAL::I2CDeviceManager *i2c_mgr)
    {
        return static_cast<I2CDeviceManager*>(i2c_mgr);
    }

    AP_HAL::OwnPtr<AP_HAL::I2CDevice> get_device(uint8_t bus, uint8_t address) override;
};

}
