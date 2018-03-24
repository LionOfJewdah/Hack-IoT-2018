#!/bin/bash

alias take_picture="fswebcam --no-banner"
shopt -s expand_aliases

function TakePictureFromCamera {
	CAMERA="$1"
	CAM_ID=${CAMERA#/dev/video}
	CAM_DIR=~/webcam/camera${CAM_ID}
	TIME=$(date +"%Y-%m-%d_%H%M")
	DEST=${CAM_DIR}/$TIME.jpg
	take_picture -d $CAMERA $DEST
}

for device in $(ls /dev/video*)
do
	TakePictureFromCamera $device
done
