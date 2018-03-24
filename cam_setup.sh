#!/bin/bash

function TouchCameraDir {
	CAMERA="$1"
	CAM_ID=${CAMERA#/dev/video}
	CAM_DIR=${WEBCAM_HOME}/camera${CAM_ID}
	mkdir -p $CAM_DIR
}

for device in $(ls /dev/video*)
do
	TouchCameraDir $device
done
