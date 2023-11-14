folder="/home/vicente/Desktop/TP_SO_2023/TP_SO_2023/src"

#INSCRICAO=300
#NPLAYERS=2
#DURACAO=300
#DECREMENTO=10



# Check if the folder exists
if [ -d "$folder" ]; then
  # Use the find command to locate and remove named pipes
  find "$folder" -type p -exec rm -f {} \;
  echo "Named pipes removed from $folder."
else
  echo "Folder '$folder' does not exist."
fi


cd ..
 
cd src

gnome-terminal --full-screen -- bash -c "./motor; exit"
sleep 0.5
gnome-terminal --full-screen -- bash -c "./jogoUI nome1; exit"
sleep 0.1
gnome-terminal --full-screen -- bash -c "./jogoUI nome2; exit"
sleep 0.1
gnome-terminal --full-screen -- bash -c "./jogoUI nome3; exit"
sleep 0.1
gnome-terminal --full-screen -- bash -c "./jogoUI nome4; exit"
sleep 0.1
gnome-terminal --full-screen -- bash -c "./jogoUI nome5; exit"

