# Docker

Docker can be either used for development, or deploying and using Templex every day. This is mainly aimed for Windows users.

## Build

Something about multistage building... This command should be executed fromt the top of the SOURCE tree.

`docker build -t templex:dev -f docker/Dockerfile .`

## Running

When running the container we need to mount a volume of our file system to be synced with the container at all time.

* -it : to open an interactive shell with tty
* --rm : cleans up the file system when the container exists
* -v : this flags parameter is the volume we want to mount to the container

`docker run --rm -it -v D:\dev\templex\templex:/templex templex:dev /bin/bash`

