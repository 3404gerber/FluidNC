#pragma once

/*
  Stepper.h - stepper motor driver: executes motion plans of planner.c using the stepper motors
  Part of Grbl

  Copyright (c) 2011-2016 Sungeun K. Jeon for Gnea Research LLC
  Copyright (c) 2009-2011 Simen Svale Skogsrud

	2018 -	Bart Dring This file was modifed for use on the ESP32
					CPU. Do not use this with Grbl for atMega328P

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "EnumItem.h"

#include <cstdint>

enum stepper_id_t {
    ST_TIMED = 0,
    ST_RMT,
    ST_I2S_STATIC,
    ST_I2S_STREAM,
};

// esp32 work around for diable in main loop
extern uint64_t stepper_idle_counter;
extern bool     stepper_idle;

void stepper_init();
void stepper_switch(stepper_id_t new_stepper);

// Enable steppers, but cycle does not start unless called by motion control or realtime command.
void st_wake_up();

// Immediately disables steppers
void st_go_idle();

// Reset the stepper subsystem variables
void st_reset();

// Changes the run state of the step segment buffer to execute the special parking motion.
void st_parking_setup_buffer();

// Restores the step segment buffer to the normal run state after a parking motion.
void st_parking_restore_buffer();

// Reloads step segment buffer. Called continuously by realtime execution system.
void st_prep_buffer();

// Called by planner_recalculate() when the executing block is updated by the new plan.
void st_update_plan_block_parameters();

// Called by realtime status reporting if realtime rate reporting is enabled in config.h.
float st_get_realtime_rate();

// disable (or enable) steppers via STEPPERS_DISABLE_PIN
bool get_stepper_disable();  // returns the state of the pin

void set_stepper_pins_on(uint8_t onMask);
void set_direction_pins_on(uint8_t onMask);

extern EnumItem stepTypes[];
extern uint32_t step_count;
