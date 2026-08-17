#include "myostask.h"
volatile uint32_t ledTaskStackHighWaterWork = 0;
volatile STATE_MODE state_mode = IDLE;
volatile STATE_MODE last_mode = IDLE;
uint8_t buzzer_count = 0;
uint8_t is_beep_on = 0;
void LED_Task(void *argument) // pwm
{

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(250); // 250ms一循环
    for (;;)
    {
        if (state_mode == Pwm)
        {
            led_off(LED1_Pin | LED2_Pin);
            beep_off();
            led_on(LED1_Pin);

            if (last_mode != Pwm)
                pwm();

            last_mode = Pwm;
        }

        ledTaskStackHighWaterWork = uxTaskGetStackHighWaterMark(NULL) * sizeof(StackType_t);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void Beep_Task(void *argument)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(250);
    for (;;)
    {

        if (state_mode == Beep)
        {
            beep_off();
            led_off(LED1_Pin | LED2_Pin);
            led_on(LED2_Pin);
            if (last_mode != Beep)
            {
                buzzer_count = 0;
                is_beep_on = 0;
            }
            if (buzzer_count < buzzer_times)
            {
                if (is_beep_on == 0)
                {
                    beep_on();
                    is_beep_on = 1;
                }
                else
                {
                    beep_off();
                    is_beep_on = 0;
                    buzzer_count++;
                }
            }

            last_mode = Beep;
        }
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void Transmit_Task(void *argument)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(2); // 2ms一循环
    for (;;)
    {
        
        can_transmit();

        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void Idle_Task(void *argument)
{
    static const uint16_t led_queue[] = {LED1_Pin, LED2_Pin};
    uint8_t i = 0;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(500); // 10ms一循环
    for (;;)
    {
        if (state_mode == IDLE)
        {
            led_off(LED1_Pin | LED2_Pin);
            beep_off();
            led_on(led_queue[i]);
            i = (i + 1) % 2;
            if (last_mode == Pwm)
            {
                HAL_TIM_Base_Stop_IT(&htim3);
                HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
                HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);
            }
            last_mode = IDLE;
        }

        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}
void Vofa_Transmit(void *argument)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(10); // 10ms一循环
    for (;;)
    {
    vofa_transmit();
            

        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}