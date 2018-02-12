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
