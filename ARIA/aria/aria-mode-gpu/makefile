CC=g++
NVCC=nvcc
CXXFLAGS= -fopenmp -O3 -Wextra -std=c++11
CUDAFLAGS= -std=c++11 -c -arch=compute_35 -rdc=true # compute_61(>35), rdc mode : call global in global
# CUDAFLAGS= -std=c++11 -c -arch=sm_70 -rdc=true # compute_61(>35), rdc mode : call global in global
LIBS= -lopenblas -lpthread -lcudart -lcublas
LIBDIRS=-L/usr/local/cuda-7.5/lib64
INCDIRS=-I/usr/local/cuda-7.5/include


# # aria_ctr_parallel.o: aria_ctr_parallel.cu
# # 	$(NVCC) $(CUDAFLAGS) aria_ctr_parallel.cu
# # all: aria_ctr_parallel.o
# # 	$(CC) -o aria_ctr_parallel aria_ctr_parallel.cu aria_ctr_parallel.o $(LIBDIRS) $(INCDIRS) $(LIBS) $(CXXFLAGS)
# # clean:
# # 	rm -rf aria_ctr_parallel *.o

# all:
# 	$(NVCC) aria_ctr_parallel.cu -o aria_ctr_parallel $(CUDAFLAGS) $(LIBDIRS) $(INCDIRS) $(LIBS) $(CXXFLAGS)

# clean:
# 	rm -rf aria_ctr_parallel *.o





############
PROJECT_NAME = aria_ctr_parallel


BUILD_DIR = build
# note that nvcc defaults to 32-bit architecture. thus, force C/LFLAGS to comply.
# you could also force nvcc to compile 64-bit with -m64 flag. (and remove -m32 instances)

# CFLAGS = -c -m32 -I$(CUDAPATH)/include
# NVCCFLAGS = -c -I$(CUDAPATH)/include

# this is mac only. only linux system, remove -Xlinker and add in -lGL -lGLUT?
# LFLAGS = -m32 -L$(CUDAPATH)/lib -lcuda -lcudart -lm -Xlinker -framework,OpenGL,-framework,GLUT

all: build

build: build_dir gpu
	$(NVCC) $(CUDAFLAGS) $(LIBDIRS) $(INCDIRS) $(LIBS) -o $(BUILD_DIR)/$(PROJECT_NAME) *.o

build_dir:
	mkdir -p $(BUILD_DIR)

gpu:
	$(NVCC) $(CUDAFLAGS) *.cu



run:
	./$(BUILD_DIR)./$(PROJECT_NAME)
clean:
	rm *.o aria_ctr_parallel