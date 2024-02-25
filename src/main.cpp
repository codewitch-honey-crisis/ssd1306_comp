#include <Arduino.h>
#include <Wire.h>
#ifdef ESP32
#define I2C_SDA 16
#define I2C_SCL 17
#endif

#define SSD1306_HEIGHT 32
const uint8_t output_frame_1[] PROGMEM = {
    0xff, 131, 0x3f, 0x9f, 0xcf, 0xef,
    0xe7, 0xe7, 0xf3, 0xf3, 0xfb, 0xfb,
    0xf9, 0xf9, 0xf9, 0xf9, 0xf9, 0xf9,
    0xf9, 0xfb, 0xfb, 0xf3, 0xf3, 0xe7,
    0xe7, 0xef, 0xcf, 0x9f, 0x3f, 0x7f,
    0xff, 98, 0x00, 2, 0xff, 6, 0x83,
    0x01, 0x83, 0xc7, 0xff, 7, 0x83,
    0x01, 0x83, 0xc7, 0xff, 7, 0x00, 2,
    0xff, 97, 0xfc, 0xf9, 0xf3, 0xe7,
    0xef, 0xcf, 0xcf, 0x9f, 0x9f, 0xbf,
    0xbf, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f,
    0x3f, 0x3f, 0xbf, 0xbf, 0x9f, 0x9f,
    0xcf, 0xcf, 0xef, 0xe7, 0xf3, 0xf9,
    0xfc, 0xfe, 0xff, 96};
const int output_frame_1_compression = 3;

// [Compressed to 16.40625% of original. Len = 84 vs 512]

const uint8_t output_frame_2[] PROGMEM = {
    0xff, 166, 0x3f, 0x3f, 0xbf, 0x9f,
    0x9f, 0xdf, 0xdf, 0xdf, 0xcf, 0xcf,
    0xcf, 0xcf, 0xcf, 0xdf, 0xdf, 0xdf,
    0x9f, 0x9f, 0xbf, 0x3f, 0x3f, 0x7f,
    0xff, 101, 0x03, 0xf1, 0xfc, 0xfe,
    0xfe, 0xff, 3, 0x0f, 0x07, 0x0f,
    0x9f, 0xff, 7, 0x0f, 0x07, 0x0f,
    0x9f, 0xff, 4, 0xfe, 0xfc, 0xf1,
    0x03, 0x0f, 0xff, 97, 0xf8, 0xf3,
    0xf7, 0xe7, 0xcf, 0xcf, 0xdf, 0x9f,
    0x9e, 0xbf, 0xbf, 0xbf, 0x3f, 0x3f,
    0x3f, 0x3f, 0x3f, 0xbf, 0xbf, 0xbe,
    0x9f, 0x9f, 0xdf, 0xcf, 0xcf, 0xe7,
    0xf7, 0xf3, 0xf8, 0xfc, 0xff, 64};
const int output_frame_2_compression = 3;

// [Compressed to 16.40625% of original. Len = 84 vs 512]

const uint8_t output_frame_3[] PROGMEM = {
    0xff, 73, 0x7f, 0x3f, 0xbf, 0xbf,
    0x9f, 0x9f, 0x9f, 0x9f, 0x9f, 0x9f,
    0x9f, 0xbf, 0xbf, 0x3f, 0x7f, 0x7f,
    0xff, 105, 0x8f, 0xe7, 0xf3, 0xf9,
    0xfc, 0xfe, 0xfe, 0xff, 2, 0xff, 11,
    0xff, 3, 0xfe, 0xfc, 0xf9, 0xf3,
    0xe7, 0x8f, 0x1f, 0xff, 97, 0x00, 2,
    0xff, 6, 0xe0, 0xc0, 0xe0, 0xf1,
    0xff, 7, 0xe0, 0xc0, 0xe0, 0xf1,
    0xff, 7, 0x00, 2, 0xff, 98, 0xfc,
    0xf9, 0xf3, 0xe7, 0xef, 0xcf, 0xdf,
    0x9f, 0x9f, 0xbf, 0x3f, 0x3f, 0x3f,
    0x3f, 0x3f, 0x3f, 0x3f, 0xbf, 0x9f,
    0x9f, 0xdf, 0xcf, 0xef, 0xe7, 0xf3,
    0xf9, 0xfc, 0xfe, 0xff, 33};
const int output_frame_3_compression = 3;

// [Compressed to 17.96875% of original. Len = 92 vs 512]

const uint8_t output_frame_4[] PROGMEM = {
    0xff, 239, 0x7f, 0x7f, 0x7f, 0x7f,
    0xff, 110, 0x0f, 0xe7, 0xe7, 0xf3,
    0xf9, 0xf9, 0xfd, 0x3c, 0x1c, 0x1e,
    0x1e, 0x3e, 0xfe, 0xfe, 0xfe, 0xfe,
    0xfe, 0xfe, 0x3e, 0x1e, 0x1e, 0x1e,
    0x3c, 0xfc, 0xfd, 0xf9, 0xf9, 0xf3,
    0xe7, 0xe7, 0x0f, 0xff, 97, 0xf8,
    0xf3, 0xf3, 0xe7, 0xcf, 0xcf, 0xdf,
    0x9e, 0x9c, 0xbc, 0xbc, 0xbe, 0xbf,
    0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0xbe,
    0xbc, 0xbc, 0xbc, 0x9e, 0x9f, 0xdf,
    0xcf, 0xcf, 0xe7, 0xf3, 0xf3, 0xf8};
const int output_frame_4_compression = 3;

// [Compressed to 14.0625% of original. Len = 72 vs 512]

const uint8_t *output_images[] = {
    output_frame_1,
    output_frame_2,
    output_frame_3,
    output_frame_4};
const int output_images_compression[] = {
    output_frame_1_compression,
    output_frame_2_compression,
    output_frame_3_compression,
    output_frame_4_compression};

#if SSD1306_HEIGHT == 32
const uint8_t ssd1306_init[] PROGMEM = {
    17,
    0xAE, 0,
    0xA8, 1, 0x1F,
    0x20, 1, 0x00,
    0x40, 0,
    0xD3, 1, 0x00,
    0xA1, 0,
    0xC8, 0,
    0xDA, 1, 0x02,
    0x81, 1, 0x7F,
    0xA4, 0,
    0xA6, 0,
    0xD5, 1, 0x80,
    0xD9, 1, 0xc2,
    0xDB, 1, 0x20,
    0x8D, 1, 0x14,
    0x2E, 0,
    0xAF, 0};
#endif
#if SSD1306_HEIGHT == 64
const uint8_t ssd1306_init[] PROGMEM = {
    17,
    0xAE, 0,
    0xA8, 1, 0x3F,
    0x20, 1, 0x00,
    0x40, 0,
    0xD3, 1, 0x00,
    0xA1, 0,
    0xC8, 0,
    0xDA, 1, 0x12,
    0x81, 1, 0x7F,
    0xA4, 0,
    0xA6, 0,
    0xD5, 1, 0x80,
    0xD9, 1, 0xc2,
    0xDB, 1, 0x20,
    0x8D, 1, 0x14,
    0x2E, 0,
    0xAF, 0};
#endif

void ssd1306_send_screen(int index)
{
    const uint8_t *data = output_images[index];
    int comp = output_images_compression[index];
    Wire.beginTransmission(0x3C);

    Wire.write(0x00);
    Wire.write(0x22);
    Wire.write(0x00);
    Wire.write(0xFF);

    Wire.write(0x00);
    Wire.write(0x21);
    Wire.write(0x00);
    Wire.write(0x7F);

    Wire.endTransmission();

    size_t rem = I2C_BUFFER_LENGTH - 1;
    int len = 0;
    Wire.beginTransmission(0x3C);
    Wire.write(0x40);
    while (len < (SSD1306_HEIGHT * 16))
    {
        uint8_t b = pgm_read_byte(data++);
        uint8_t count = 1;
        if (((comp == 1 || comp == 3) && b == 0) ||
            ((comp == 2 || comp == 3) && b == 255))
        {
            count = pgm_read_byte(data++);
        }
        while (count--)
        {
            Wire.write(b);
            ++len;
            --rem;
            if (rem == 0)
            {
                rem = I2C_BUFFER_LENGTH - 1;
                Wire.endTransmission();
                Wire.beginTransmission(0x3C);
                Wire.write(0x40);
            }
        }
    }
    Wire.endTransmission();
}
void setup()
{
#ifdef ESP32
    Wire.begin(I2C_SDA, I2C_SCL, 800 * 1000);
#else
    Wire.begin();
#endif
    Serial.begin(115200);
    Wire.beginTransmission(0x3C);
    const uint8_t *init = ssd1306_init;
    uint8_t len = pgm_read_byte(init);
    const uint8_t *p = init + 1;
    while (len--)
    {
        Wire.write(0x00);
        Wire.write(pgm_read_byte(p++));
        uint8_t arglen = pgm_read_byte(p++);
        while (arglen--)
            Wire.write(pgm_read_byte(p++));
    }
    Wire.endTransmission();
}

void loop()
{
    static int index = 0;
    ssd1306_send_screen(index++);
    delay(100);
    if (index == 4)
    {
        index = 0;
    }
}
