#include "mppt_controller.h"
#include "esphome.h"

namespace esphome
{
    namespace mppt
    {

        void MPPTController::setup()
        {
            ESP_LOGI("MPPTController", "Setting up MPPT Controller...");
        }

        void MPPTController::set_voltage_sensor(sensor::Sensor *voltage_sensor)
        {
            this->voltage_sensor_ = voltage_sensor;
        }

        void MPPTController::set_current_sensor(sensor::Sensor *current_sensor)
        {
            this->current_sensor_ = current_sensor;
        }

        void MPPTController::set_output_pin(output::FloatOutput *output_pin)
        {
            this->output_pin_ = output_pin;
        }

        void MPPTController::set_power_sensor(sensor::Sensor *power_sensor)
        {
            this->power_sensor_ = power_sensor;
        }

        void MPPTController::update()
        {
            if (this->voltage_sensor_ == nullptr || this->current_sensor_ == nullptr)
            {
                ESP_LOGW("MPPTController", "Sensors not configured properly.");
                return;
            }

            float voltage = this->voltage_sensor_->state;
            float current = this->current_sensor_->state;
            float power = voltage * current;

            if (this->power_sensor_ != nullptr)
            {
                this->power_sensor_->publish_state(power);
            }

            this->mppt_algorithm_();
        }

        void MPPTController::mppt_algorithm_()
        {
            float voltage = this->voltage_sensor_->state;
            float current = this->current_sensor_->state;
            float power = voltage * current;

            if (power > this->last_power_)
            {
                this->last_power_ = power;
                this->last_voltage_ = voltage;
                this->increase_ ? this->perturb_amount_ += 0.01 : this->perturb_amount_ -= 0.01;
            }
            else
            {
                this->increase_ = !this->increase_;
            }

            if (this->output_pin_ != nullptr)
            {
                float duty_cycle = voltage + (this->increase_ ? this->perturb_amount_ : -this->perturb_amount_);
                duty_cycle = std::max(0.0f, std::min(1.0f, duty_cycle));
                this->output_pin_->set_level(duty_cycle);
            }
        }

    }
}
