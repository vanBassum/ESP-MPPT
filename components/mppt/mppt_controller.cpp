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

        void MPPTController::set_duty_sensor(sensor::Sensor *duty_sensor)
        {
            this->duty_sensor_ = duty_sensor;
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
            float duty = this->mppt_algorithm_();

            if (this->output_pin_ != nullptr)
            {
                this->output_pin_->set_level(duty);
            }

            if (this->duty_sensor_ != nullptr)
            {
                this->duty_sensor_->publish_state(duty);
            }

            if (this->power_sensor_ != nullptr)
            {
                this->power_sensor_->publish_state(power);
            }
        }

        float MPPTController::mppt_algorithm_()
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

            float duty_cycle = voltage + (this->increase_ ? this->perturb_amount_ : -this->perturb_amount_);
            duty_cycle = std::max(0.0f, std::min(1.0f, duty_cycle));
            return duty_cycle;
        }

    }
}
