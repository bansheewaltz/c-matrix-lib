#!/bin/bash

# set prompt's colors
CLR="\033[38;5;117m"
RST="\033[0m"

# start docker if it's not running yet
if ! docker ps 2>&1 >/dev/null; then
  if [ "$OS" = 'macOS' ]; then
    echo "Starting Docker app... Delay is 17 sec"
    open -a Docker
    sleep 17
  else
    echo "Error: you should start Docker first"
    exit 0
  fi
fi

# set default image if another is not specified
if test -z "$image"; then
  image_os="ubuntu"
else
  image_os="$image"
fi

container_name="dondarri.${image_os}-temp"
docker rm -f $container_name 2> /dev/null
dockerfile="Dockerfile.$image_os"
image="dondarri/$image_os"
prompt="$CLR$image_os@container$RST:\W$ "
command="echo \"export PS1='$prompt'\" >> ~/.bashrc && bash"

docker build -t $image -f $SCRIPTS_DIR/$dockerfile .
# docker build -t $image -f ${TEST_DIR}/$dockerfile . --platform linux/x86_64
# docker run --platform linux/x86_64
docker run -it --rm\
  --name "$container_name" \
  -e PS1="$prompt" \
  -v $PWD/../:/usr/project \
  -w /usr/project/src \
  $image \
  bash -c "$command"

# docker rm $container_name > /dev/null
echo "docker: the used container has been deleted"
# docker rmi $image > /dev/null
# echo "docker: the used image has been removed"
# docker builder prune -f > /dev/null
# echo "docker: the build cache has been removed"
make fclean
