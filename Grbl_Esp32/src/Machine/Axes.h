#pragma once

/*
    Part of Grbl_ESP32
    2021 -  Stefan de Bruijn, Mitch Bradley

    Grbl_ESP32 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Grbl_ESP32 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Grbl_ESP32.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../Configuration/Configurable.h"
#include "Axis.h"

namespace Motors {
    class Motor;
}

namespace Machine {
    class Axes : public Configuration::Configurable {
        static constexpr const char* _names = "XYZABC";

        bool _switchedStepper = false;

        // During homing, this is used to stop stepping on motors that have
        // reached their limit switches, by clearing bits in the mask.
        MotorMask _motorLockoutMask = 0;

    public:
        Axes();

        // Bitmasks to collect information about axes that have limits and homing
        static MotorMask posLimitMask;
        static MotorMask negLimitMask;
        static MotorMask homingMask;
        static MotorMask limitMask;
        static MotorMask motorMask;

        inline char axisName(int index) { return index < MAX_N_AXIS ? _names[index] : '?'; }

        Pin _sharedStepperDisable;

        int   _numberAxis = 3;
        Axis* _axis[MAX_N_AXIS];

        // Some small helpers to find the axis index and axis ganged index for a given motor. This
        // is helpful for some motors that need this info, as well as debug information.
        size_t findAxisIndex(const Motors::Motor* const motor) const;
        size_t findAxisGanged(const Motors::Motor* const motor) const;

        inline bool hasSoftLimits() const {
            for (int i = 0; i < _numberAxis; ++i) {
                if (_axis[i]->_softLimits) {
                    return true;
                }
            }
            return false;
        }

        inline bool hasHardLimits() const {
            for (int axis = 0; axis < _numberAxis; ++axis) {
                auto a  = _axis[axis];
                auto ae = a->_endstops;
                if (ae && ae->_hardLimits) {
                    return true;
                }
                for (int gang = 0; gang < Axis::MAX_NUMBER_GANGED; ++gang) {
                    auto ge = a->_gangs[gang]->_endstops;
                    if (ge && ge->_hardLimits) {
                        return true;
                    }
                }
            }
            return false;
        }

        // These are used for setup and to talk to the motors as a group.
        void init();
        void read_settings();  // more like 'after read settings, before init'. Oh well...

        // These are used during homing cycles.
        // The return value is a bitmask of axes that can home
        MotorMask set_homing_mode(AxisMask homing_mask, bool isHoming);
        void      unlock_all_motors();     
        void      lock_motors(MotorMask motor_mask);
        void      unlock_motors(MotorMask motor_mask);

        void set_disable(int axis, bool disable);
        void set_disable(bool disable);
        void step(uint8_t step_mask, uint8_t dir_mask);
        void unstep();

        // Configuration helpers:
        void group(Configuration::HandlerBase& handler) override;
        void afterParse() override;

        ~Axes();
    };
}
