CMD="./test5 1 3000000 200"
echo $CMD > output/numThreads.txt
{ time $CMD; } 2>> output/numThreads.txt

CMD="./test5 2 3000000 200"
echo $CMD >> output/numThreads.txt
{ time $CMD; } 2>> output/numThreads.txt

CMD="./test5 4 3000000 200"
echo $CMD >> output/numThreads.txt
{ time $CMD; } 2>> output/numThreads.txt

CMD="./test5 6 3000000 200"
echo $CMD >> output/numThreads.txt
{ time $CMD; } 2>> output/numThreads.txt

CMD="./test5 8 3000000 200"
echo $CMD >> output/numThreads.txt
{ time $CMD; } 2>> output/numThreads.txt

CMD="./test5 10 3000000 200"
echo $CMD >> output/numThreads.txt
{ time $CMD; } 2>> output/numThreads.txt

CMD="./test5 12 3000000 200"
echo $CMD >> output/numThreads.txt
{ time $CMD; } 2>> output/numThreads.txt

CMD="./test5 14 3000000 200"
echo $CMD >> output/numThreads.txt
{ time $CMD; } 2>> output/numThreads.txt

CMD="./test5 16 3000000 200"
echo $CMD >> output/numThreads.txt
{ time $CMD; } 2>> output/numThreads.txt

CMD="./test5 18 3000000 200"
echo $CMD >> output/numThreads.txt
{ time $CMD; } 2>> output/numThreads.txt
    
CMD="./test5 20 3000000 200"
echo $CMD >> output/numThreads.txt
{ time $CMD; } 2>> output/numThreads.txt
