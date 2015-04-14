# CircuitSense Project

Welcome to the CircuitSense project. CircuitSense is an application that
converts a picture of a circuit schematic to a netlist. CircuitSense was
originally developed as an iPhone application, and has since been migrated here
and transformed into a more general purpose tool.

# CircuitSense Usage

## Installation

On Linux, the following apt libraries are required:

```
python-opencv
libcv-dev
libopencv-highgui-dev
libopencv-imgproc-dev
libtesseract-dev
```

On mac, the following brew libraries are required:

```
tesseract
opencv
```

## Training

To operate, CircuitSense requires a training data file, `cir.traindata`. Such
training files are currently not provided in this project. To run this
software, you will need to create this file as per the tesseract guidelines.

Pull requests in this area are welcome.

# Compatibility

CircuitSense is only compatible with Tesseract 2.0.4, and will not work with
the newest versions of the Tesseract library (3+). I may add support for this
in the future, if time allows.

# License

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org>
