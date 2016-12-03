CMD="./test5 2 3000000 50"
echo $CMD > output/seqThreshold.txt
{ time $CMD; } 2>> output/seqThreshold.txt

CMD="./test5 2 3000000 100"
echo $CMD >> output/seqThreshold.txt
{ time $CMD; } 2>> output/seqThreshold.txt

CMD="./test5 2 3000000 200"
echo $CMD >> output/seqThreshold.txt
{ time $CMD; } 2>> output/seqThreshold.txt

CMD="./test5 2 3000000 300"
echo $CMD >> output/seqThreshold.txt
{ time $CMD; } 2>> output/seqThreshold.txt

CMD="./test5 2 3000000 400"
echo $CMD >> output/seqThreshold.txt
{ time $CMD; } 2>> output/seqThreshold.txt

CMD="./test5 2 3000000 500"
echo $CMD >> output/seqThreshold.txt
{ time $CMD; } 2>> output/seqThreshold.txt

CMD="./test5 2 3000000 600"
echo $CMD >> output/seqThreshold.txt
{ time $CMD; } 2>> output/seqThreshold.txt

CMD="./test5 2 3000000 700"
echo $CMD >> output/seqThreshold.txt
{ time $CMD; } 2>> output/seqThreshold.txt

CMD="./test5 2 3000000 800"
echo $CMD >> output/seqThreshold.txt
{ time $CMD; } 2>> output/seqThreshold.txt

CMD="./test5 2 3000000 900"
echo $CMD >> output/seqThreshold.txt
{ time $CMD; } 2>> output/seqThreshold.txt

CMD="./test5 2 3000000 1000"
echo $CMD >> output/seqThreshold.txt
{ time $CMD; } 2>> output/seqThreshold.txt
