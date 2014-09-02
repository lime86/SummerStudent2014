#! /bin/bash

echo "Will run "$2" events at \sqrt{s} = "$3", mediator width M_med / "$7" (queue "$4") with "$5", analysis "$6" for all the mass points in the grid and output into folder structure under "$1

export MEMORY=`echo "$2/2" | bc`

if [ -d "$1" ]; then
  echo "Output directory $1 already exists!"
  exit
fi

sleep 5

mkdir $1

for i in 1000
do
  for j in 10 100 
  do
    #if [ "$i" -ge "$j" ]; then
      export WIDTH=`echo "$i/$7" | bc -l`
      bsub -R "rusage[pool=$MEMORY]" -R "type=SLC6_64" -q 1nw submitMedDM$5.sh $i $j $6 $1/Med${i}_DM${j} $2 $3 $WIDTH
    #fi
  done
done

exit
