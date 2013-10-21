TOPDIR = ..

SRCS = $(wildcard *.cpp)
OBJS = $(addprefix ., ${SRCS:.cpp=.o})

BINARY = circuitsense

all: ${BINARY}

${BINARY}: ${OBJS}
	@echo "$(shell date +%H:%M:%S) Linking..."
	@g++ ${OBJS} -o ${BINARY} \
		-lopencv_core \
		-lopencv_highgui \
		-lopencv_imgproc \
		-ltesseract

.%.o: %.cpp
	@echo "$(shell date +%H:%M:%S) Building $?..."
	@g++ -c $? -I../include -o $@

clean:
	@rm -f ${OBJS} ${BINARY}
