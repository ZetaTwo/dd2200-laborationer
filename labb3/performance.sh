#Compiling stock version
echo -n "Compiling with stock malloc... "
gcc -Wall -o performance performancetest1.c
echo "done"

echo "Starting benchmark"
#Run and measure time
for REALLOC in {0..5}
do
  RES_FILE_T=results/stock_t_$REALLOC.txt
  RES_FILE_MEM=results/stock_mem_$REALLOC.txt
  echo "Stock malloc $((REALLOC))0% realloc"
  echo -e "#\t1\t10\t100\t1000\t10000\t100000\t1000000\t" > $RES_FILE_T
  echo -e "#\t1\t10\t100\t1000\t10000\t100000\t1000000\t" > $RES_FILE_MEM
  for i in {1..10}
  do
    echo -n -e "$((i))\t" >> $RES_FILE_T
    echo -n -e "$((i))\t" >> $RES_FILE_MEM
    for ITER in 1 10 100 1000 10000 100000 1000000
    do
      STARTTIME=10#$(date +%s%6N)
      ./performance $ITER $REALLOC >> $RES_FILE_MEM
      ENDTIME=10#$(date +%s%6N)

      echo -n -e "$((ENDTIME-STARTTIME))\t" >> $RES_FILE_T
      echo -n -e "\t" >> $RES_FILE_MEM
    done
    echo "" >> $RES_FILE_T
    echo "" >> $RES_FILE_MEM
  done
done

#Compiling my version with strategy 1
echo -n "Compiling with my malloc... "
gcc -Wall -c -O4 malloc.c -DSTRATEGY=1
gcc -Wall -o performance -DMY_MALLOC malloc.o performancetest1.c
echo "done"

echo "Starting benchmark"
#Run and measure time
for REALLOC in {0..5}
do
  RES_FILE_T=results/my1_t_$REALLOC.txt
  RES_FILE_MEM=results/my1_mem_$REALLOC.txt
  echo "My malloc $((REALLOC))0% realloc"
  echo -e "#\t1\t10\t100\t1000\t10000\t100000\t1000000\t" > $RES_FILE_T
  echo -e "#\t1\t10\t100\t1000\t10000\t100000\t1000000\t" > $RES_FILE_MEM
  for i in {1..10}
  do
    echo -n -e "$((i))\t" >> $RES_FILE_T
    echo -n -e "$((i))\t" >> $RES_FILE_MEM
    for ITER in 1 10 100 1000 10000 100000 1000000
    do
      STARTTIME=10#$(date +%s%6N)
      ./performance $ITER $REALLOC >> $RES_FILE_MEM
      ENDTIME=10#$(date +%s%6N)

      echo -n -e "$((ENDTIME-STARTTIME))\t" >> $RES_FILE_T
      echo -n -e "\t" >> $RES_FILE_MEM
    done
    echo "" >> $RES_FILE_T
    echo "" >> $RES_FILE_MEM
  done
done

#Compiling my version with strategy 2
echo -n "Compiling with my malloc... "
gcc -Wall -c -O4 malloc.c -DSTRATEGY=2
gcc -Wall -o performance -DMY_MALLOC malloc.o performancetest1.c
echo "done"

echo "Starting benchmark"
#Run and measure time
for REALLOC in {0..5}
do
  RES_FILE_T=results/my2_t_$REALLOC.txt
  RES_FILE_MEM=results/my2_mem_$REALLOC.txt
  echo "My malloc $((REALLOC))0% realloc"
  echo -e "#\t1\t10\t100\t1000\t10000\t100000\t1000000\t" > $RES_FILE_T
  echo -e "#\t1\t10\t100\t1000\t10000\t100000\t1000000\t" > $RES_FILE_MEM
  for i in {1..10}
  do
    echo -n -e "$((i))\t" >> $RES_FILE_T
    echo -n -e "$((i))\t" >> $RES_FILE_MEM
    for ITER in 1 10 100 1000 10000 100000 1000000
    do
      STARTTIME=10#$(date +%s%6N)
      ./performance $ITER $REALLOC >> $RES_FILE_MEM
      ENDTIME=10#$(date +%s%6N)

      echo -n -e "$((ENDTIME-STARTTIME))\t" >> $RES_FILE_T
      echo -n -e "\t" >> $RES_FILE_MEM
    done
    echo "" >> $RES_FILE_T
    echo "" >> $RES_FILE_MEM
  done
done