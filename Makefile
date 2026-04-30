default:
	mkdir -p bin
	g++ src/glad.c src/*.cpp -I include -I/opt/assimp/include -I/opt/assimp/build/include -o bin/model-loading -L/opt/assimp/build/bin -lglfw -lassimp
	./bin/model-loading

clean:
	rm -rf bin
