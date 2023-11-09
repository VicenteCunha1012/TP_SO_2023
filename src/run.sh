folder="/home/vicente/Desktop/TP_SO_2023/TP_SO_2023/src"

# Check if the folder exists
if [ -d "$folder" ]; then
  # Use the find command to locate and remove named pipes
  find "$folder" -type p -exec rm -f {} \;
  echo "Named pipes removed from $folder."
else
  echo "Folder '$folder' does not exist."
fi


cd ..

make

cd src


gnome-terminal -- bash -c "./motor; exit"
sleep 1
gnome-terminal -- bash -c "./jogoUI nome1; exec bash"
sleep 0.3
gnome-terminal -- bash -c "./jogoUI nome2; exit"
sleep 0.3
gnome-terminal -- bash -c "./jogoUI nome3; exit"
sleep 0.3
gnome-terminal -- bash -c "./jogoUI nome4; exit"
sleep 0.3
gnome-terminal -- bash -c "./jogoUI nome5; exit"
