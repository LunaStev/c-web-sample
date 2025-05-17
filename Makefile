build:
	cd frontend && npx tsc
	cp frontend/static/index.html static/

run:
	gcc backend/main.c backend/webserver.c -o server
	sudo ./server

clean:
	rm -rf static/*.js static/*.html server static/
