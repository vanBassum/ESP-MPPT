#pragma once
#include "esphome.h"

namespace esphome
{
    namespace mppt
    {

        class MPPTController : public PollingComponent
        {
        public:
            MPPTController(uint8_t address, uint32_t update_interval = 1000);

            void setup() override;
            void update() override;

            void set_voltage_sensor(sensor::Sensor *voltage_sensor);
            void set_current_sensor(sensor::Sensor *current_sensor);
            void set_output_pin(output::FloatOutput *output_pin);
            void set_power_sensor(sensor::Sensor *power_sensor);
            void set_duty_sensor(sensor::Sensor *duty_sensor);

            float get_duty_cycle() const { return this->duty_cycle_ * 100.0f; }

        private:
            uint8_t address_;
            sensor::Sensor *voltage_sensor_{nullptr};
            sensor::Sensor *current_sensor_{nullptr};
            output::FloatOutput *output_pin_{nullptr};
            sensor::Sensor *power_sensor_{nullptr};
            sensor::Sensor *duty_sensor_{nullptr};

            float last_power_{0};
            float last_voltage_{0};
            float perturb_amount_{0.01};
            float duty_cycle_{0};
            bool increase_{true};

            float mppt_algorithm_(); // Fixed signature here
        };

    }
} 

