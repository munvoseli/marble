gcc main.c -lGL -lSDL2
if [ "$1" = "nr" ]
then
	echo "no run, build only"
else
	./a.out
fi
