#!/bin/bash
ffmpeg -version
if [[ $? -eq 0 ]]
then
	exit 0
else
	echo "FFMPEG package not exist"
	zenity --question --title="ffmpeg package dont exist" --text="Without the ffmpeg package, you will not have access to the editor mode, you will only be able to use the program to watch videos. Do you want to install ffmpeg package?"
	if [[ $? -eq 0 ]]
	then 
		echo $(zenity --password --title="Enter sudo password") | sudo -S apt-get install ffmpeg
		exit $?
	else 
		zenity --warning --text="You will not have access to the video editor mode."
		exit 1
	fi
fi
