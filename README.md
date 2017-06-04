# Pseudo-multithreaded HTTP proxy
Simple dumb proxy with cache in RAM

- Uses only one thread and `poll`
- OOP-designed
- Only GET-requests are supported

## Build
	mkdir build/
	cd build/
	cmake ..
	make
	
## Run
	./ProxyOnPoll [port-to-listen]