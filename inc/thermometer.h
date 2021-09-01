#ifndef THERMOMETER_H_
#define THERMOMETER_H_

#include "bme280.h"

struct bme280_dev init_thermometer();
void user_delay_ms(uint32_t period);
void print_sensor_data(struct bme280_data *comp_data);
int8_t user_i2c_read(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len);
int8_t user_i2c_write(uint8_t id, uint8_t reg_addr, uint8_t *data,
                      uint16_t len);
float stream_sensor_data_forced_mode(struct bme280_dev *dev);

#endif /* THERMOMETER_H_ */