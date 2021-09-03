#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>

#include "bme280.h"

int8_t user_i2c_read(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len) {
  write(i2c_filestream, &reg_addr, 1);
  read(i2c_filestream, data, len);

  return 0;
}

void user_delay_ms(uint32_t period) { usleep(period * 1000); }

int8_t user_i2c_write(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len) {
  int8_t *buf;

  buf = malloc(len + 1);
  buf[0] = reg_addr;
  memcpy(buf + 1, data, len);
  if (write(i2c_filestream, buf, len + 1) < len) {
    return BME280_E_COMM_FAIL;
  }

  free(buf);

  return BME280_OK;
}

float get_current_temperature(struct bme280_dev *dev) {
  uint8_t settings_sel = 0;
  uint32_t req_delay;
  struct bme280_data comp_data;

  dev->settings.osr_h = BME280_OVERSAMPLING_1X;
  dev->settings.osr_p = BME280_OVERSAMPLING_16X;
  dev->settings.osr_t = BME280_OVERSAMPLING_2X;
  dev->settings.filter = BME280_FILTER_COEFF_16;

  settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL |
                 BME280_OSR_HUM_SEL | BME280_FILTER_SEL;

  bme280_set_sensor_settings(settings_sel, dev);

  req_delay = bme280_cal_meas_delay(&dev->settings);
  bme280_set_sensor_mode(BME280_FORCED_MODE, dev);

  dev->delay_ms(req_delay);
  bme280_get_sensor_data(BME280_ALL, &comp_data, dev);
  return comp_data.temperature;
}

struct bme280_dev connect_bme() {
  struct bme280_dev dev;

  int8_t rslt = BME280_OK;
  char i2c_file[] = "/dev/i2c-1";

  dev.dev_id = BME280_I2C_ADDR_PRIM;
  dev.intf = BME280_I2C_INTF;
  dev.read = user_i2c_read;
  dev.write = user_i2c_write;
  dev.delay_ms = user_delay_ms;

  if ((i2c_filestream = open(i2c_file, O_RDWR)) < 0) {
    exit(1);
  }

  if (ioctl(i2c_filestream, I2C_SLAVE, dev.dev_id) < 0) {
    fprintf(stderr, "Failed to acquire bus access and/or talk to slave.\n");
    exit(1);
  }

  /* Initialize the bme280 */
  rslt = bme280_init(&dev);
  if (rslt != BME280_OK) {
    fprintf(stderr, "Failed to initialize the device (code %+d).\n", rslt);
    exit(1);
  }
  return dev;
}