# ssd1306_comp

This project demonstrates a simple mechanism to compress images and send them to an SSD1306 without really using any RAM.

The variant RLE compression is often good for doing simple full screen animations, or just static screens.

## Usage

Prepare some images that are the same size as your screen. I recommend saving them as monochrome to avoid anti-aliasing, but it will work with anything.

Run the cigen application passing the name of each image. This will generate a header to stdout

Since the header is geared for QMK rather than Arduino, you'll probably want to just copy the generated arrays out of the file and paste them into the arduino project.

If your screen is 128x64 then you need to change the SSD1306_HEIGHT define to reflect that. It's currently configured for 32

Empty project
RAM: used 18242 bytes from 327680 bytes
Flash: used 234057 bytes from 3342336 bytes

Just Wire lib
RAM: used 18628 bytes from 327680 bytes
Flash: used 257077 bytes from 3342336 bytes

Animations
RAM: used 18652 bytes from 327680 bytes
Flash: used 277765 bytes from 3342336 bytes