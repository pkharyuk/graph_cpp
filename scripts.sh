#!/bin/bash

docker_image_name="graph-cpp"
docker_image_tag="latest"

function help() {
    echo -e "\t\t\tCommands:"
    echo
    echo -e "\t\t Docker"
    echo -e "\t docker_build : Build docker image"
    echo -e "\t docker_run : Run docker container"
    echo -e "\t docker_clean : Clean related docker files"
    echo
    echo -e "\t\t Building from sources"
    echo -e "\t compile : Compile modules and the main demo"
    echo -e "\t run_demo_graph : Run graph demonstration"
    echo -e "\t run_hex_game : Run hex game"
    echo -e "\t clean : Remove all object files and compilation results"
    echo
}


# build docker container
function docker_build() {
    docker build -t $docker_image_name:$docker_image_tag ./
}
# run docker
function docker_run() {
    docker run -it --rm $docker_image_name:$docker_image_tag bash
}

# delete docker files
function docker_clean() {
    docker rmi $docker_image_name:$docker_image_tag
}


function compile() {
    make compile
}
function run_demo_graph() {
    make run_demo_graph
}
function run_hex_game() {
    make run_hex_game
}
function clean() {
    make clean
}

case $1 in
    docker_build)
        docker_build ;;
    docker_run)
        docker_run ;;
    docker_clean)
        docker_clean ;;
    compile)
        compile ;;
    run_demo_graph)
        run_demo_graph ;;
    run_hex_game)
        run_hex_game ;;
    clean)
        clean ;;
    *)
        help ;;
esac
