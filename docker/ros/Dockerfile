FROM osrf/ros:kinetic-desktop-full

RUN apt-get update && apt-get -y install \
    libgl1-mesa-glx \
    libgl1-mesa-dri \
    python-catkin-tools \
    ros-kinetic-rosserial \
    ros-kinetic-rosserial-server && \
    rm -rf /var/lib/apt/lists/*