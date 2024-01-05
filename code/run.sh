if [ $# -gt 0 ];
then
    max_k=$1
else
    max_k=5
fi

if [ ! -d "./build" ];then
  mkdir build 
fi
cd build
cmake .. && make

./lab1 -r lru -d ../data/data-5w-50w-zipf.txt
./lab1 -r clock -d ../data/data-5w-50w-zipf.txt

for ((i=2; i<=$max_k; i++))
do
    ./lab1 -r lru-k -k $i -d ../data/data-5w-50w-zipf.txt
done
