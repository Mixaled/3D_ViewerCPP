RM = rm -rf 

all: install

install:
	cd ../include/tinyobjloader && git checkout 9d9e987
	cd ../ && mkdir build && cd build && cmake .. && make -j4
uninstall:
	${RM} ../build

update:
	sudo apt-get install imagemagick byzanz
	pip3 install pygetwindow pyautogui